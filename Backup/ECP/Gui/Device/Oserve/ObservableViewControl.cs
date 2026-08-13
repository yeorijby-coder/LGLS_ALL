using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Device.Observe;
using ECP.Global;
using ECP.Global.Device;


namespace ECP.Gui.Device.Observe
{

    public delegate void DelegateSelectedItemChanged(object sender, ObservableViewControl.ObservableViewItem item);

    public enum ObservableViewAccessMode { Spy, Simulation }

    public partial class ObservableViewControl : Control
    {
        public enum ObservableViewItemType { ObservableCategory, Observable }

        public class ObservableViewItem
        {
            private int level = 0;
            public int Level
            {
                get { return this.level; }
                set { this.level = value; }
            }

            private string ownerName;

            public string OwnerName
            {
                get { return ownerName; }
                set { ownerName = value; }
            }

            private string name;
            public string Name
            {
                get { return this.name; }
                set { this.name = value; }

            }

            private string key;
            public string Key
            {
                get { return this.key; }
                set { this.key = value; }
            }

            private ObservableViewItemType itemType;
            public ObservableViewItemType ItemType
            {
                get { return this.itemType; }
                set { this.itemType = value; }
            }

            private Observable observable;
            public Observable Observable
            {
                get { return this.observable; }
                set { this.observable = value; }
            }

            private bool expanded = true;
            public bool Expanded
            {
                get { return this.expanded; }
                set { this.expanded = value; }
            }

            private bool treeLineVisible = true;
            public bool TreeLineVisible
            {
                get { return treeLineVisible; }
                set { treeLineVisible = value; }
            }

        }
    

        private ObservableViewAccessMode accessMode = ObservableViewAccessMode.Spy;
        public ObservableViewAccessMode AccessMode
        {
            get { return this.accessMode; }
            set { this.accessMode = value; }
        }

        public event DelegateSelectedItemChanged selectedItemChanged = null;

        //private int categoryLevel = -1;

        // Observable view item 리스트, 이것은 Observable Path(즉 Category)또는 Observabel 자체일 수 있음
        private List<ObservableViewItem> observableViewItemList = null;   
        // Observable의 리스트
        private List<Observable> observableList=null;
        
        /// <summary>
        /// 동일 한 Guid을 가진 Observable과 이에 대한 Viewer의 인덱스의 리스트를 가진 맵
        /// key 는 동일한 이름의 Observable Guid, 값은 Viewer내의 특정 인덱스의 리스트 
        /// 이 값은 주로 특정 Observable이 변경되었을때, Viewer내의 해당 위치만 Invalidate하기 위하여 사용되어짐
        /// </summary>
        private Dictionary<string, List<int>> observableViewItemMap =null;


        private Dictionary<string, Observable> observables;
        [Browsable(false)]
        public Dictionary<string, Observable> Observables
        {
            get { return this.observables; }
            set
            {
                this.observables = value;
                if (this.observables != null)
                {
                    // update observable item list and observable list
                    this.UpdateObservableItemList(false);
                    this.UpdateObservableMapList();
                    this.UpdateScrollBar();
                    Invalidate();
                }
            }
        }

        Dictionary<Guid, ObservableChangeEventWrapper> observableChangeEventWrapperMap =
            new Dictionary<Guid, ObservableChangeEventWrapper>();

        /// <summary>
        ///  GUI Thread와 Cross Threading을 회피하기 위한 Delegate
        /// </summary>
        /// <param name="observable"></param>
        private delegate void DelegateUpdateObservable(Observable observable);

        /// <summary>
        ///  이 Method는 Subscribe한 Observable의 값이 변경되었음을 통지 받은 경우 호출 되어짐
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="value"></param>
        private void ObservableValueChanged(object sender, object value)
        {
            try
            {
                this.BeginInvoke(new DelegateUpdateObservable(UpdateObservable), sender);
            }
            catch (InvalidOperationException ex)
            {
                System.Console.WriteLine("ObservableValueChanged:" + ex.StackTrace);
            }
        }


        public void UpdateObservable(Observable observable)
        {
            if (observableViewItemMap.ContainsKey(observable.ObservableGuid.ToString()))
            {
                List<int> oIndexList = observableViewItemMap[observable.ObservableGuid.ToString()];
                for (int i = 0; i < oIndexList.Count; i++)
                {
                    InvalidateIndex(oIndexList[i]);
                }
            }
        }

        /// <summary>
        ///  Veiewer에서 첫줄에 나타나는 Item의 Index
        /// </summary>
        private int startLine;

        /// <summary>
        ///  Viewer Control의 Size을 변경 하고 있는 중인지의 여부
        /// </summary>
        private bool isSizing;

        /// <summary>
        ///  선택되어진 Viewer Item의 No.
        /// </summary>
        private int selectedLine;

        private const int INDENTATION = 10;

        private bool hexMode;
        public bool HexMode
        {
            get { return this.hexMode; }
            set { this.hexMode = value; }
        }


        private int nameWidth;
        public int NameWidth
        {
            get { return this.nameWidth; }
            set 
            {
                this.nameWidth = value;
                Invalidate(); 
            }
        }
        /// <summary>
        ///  한개의 Item의 높이
        /// </summary>
        [Browsable(false)]
        public int ItemHeight
        {
            get { return Font.Height + 4; }
        }
        /// <summary>
        ///  Viewer Control에 보여 주는 Item의 수
        /// </summary>
        [Browsable(false)]
        public int VisibleLines
        {
            get { return Height / ItemHeight; }
        }


        /// <summary>
        /// Viwer Control에서 실제 정보가 보여질수 있는 전체 폭
        /// </summary>
        [Browsable(false)]
        public int ClientWidth
        {
            get { return vsScroll.Visible ? this.Width - vsScroll.Width : this.Width; }

        }

        /// <summary>
        ///  사용자가 Data를 편집 중인경우
        /// </summary>
        [Browsable(false)]
        public bool IsEditing
        {
            get { return this.tbInPlaceEdit.Visible; }
        }


        public ObservableViewControl()
        {
            InitializeComponent();

            SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            SetStyle(ControlStyles.UserPaint, true);

            DoubleBuffered = true;

            this.observableViewItemList = new List<ObservableViewItem>();
            this.observableViewItemMap = new Dictionary<string, List<int>>();
            this.observableList = new List<Observable>();
           

            this.startLine =0;
            this.nameWidth = 150;
            this.isSizing = false;
            SetSelectedLine(-1);
        }

        private void SetSelectedLine(int iIndex)
        {
            selectedLine = iIndex;
            if (this.selectedItemChanged != null)
            {
                if (iIndex >= 0 && iIndex < this.observableViewItemList.Count)
                {
                    this.selectedItemChanged(this, this.observableViewItemList[selectedLine]);
                }
            }
        }

        private void SetFitNameWidth()
        {
            string strMaxLength = string.Empty;
            for (int i = 0; i < observableList.Count; i++)
            {
                if (observableList[i].ObservableId.Length > strMaxLength.Length)
                    strMaxLength = observableList[i].ObservableId;
            }

            nameWidth = (int)CreateGraphics().MeasureString(strMaxLength, Font).Width + 28;

            if (nameWidth > ClientWidth) nameWidth = ClientWidth - 70;
        }

        /// <summary>
        ///  Update ObservableItemList that has items which is category (observable path) and observable
        ///  If it is observable path, we create observable category view Item that has  observable path 
        ///  and type which is category
        ///  If it is observable itshef we create observable view item that has observable id and observable itself.
        /// </summary>
        private void UpdateObservableItemList(bool isCategoryOnly)
        {
            if (this.observables == null) return;

            this.observableViewItemList.Clear();
            this.observableList.Clear();
                        

            //this.categoryLevel = -1;

            // obsMap contain key : pathname + obserableId, value: observable object itself
            Dictionary<string, Observable> observableDictionary = new Dictionary<string, Observable>();

            // category catain, key : observable path, value observable id list which has same observable path
            Dictionary<string, List<string>> categoryHavingObservableIds = new Dictionary<string, List<string>>();

            // temporary list that has observable id list
            List<string> observableNameList;

            // temporay list hat has obervable path name 
            List<string> categoryNameList;

            foreach (Observable obs in this.observables.Values)
            {
                // add observable obejct as value when key is  observable path + observable id 
                observableDictionary.Add(string.Format("{0}:{1}", obs.ObservablePath, obs.ObservableId), obs);
                if (categoryHavingObservableIds.ContainsKey(obs.ObservablePath))
                {
                    observableNameList = categoryHavingObservableIds[obs.ObservablePath];
                }
                else
                {
                    // this is case when we have brand new observable path, 
                    // create list that will contain observable ids
                    observableNameList = new List<string>();
                    categoryHavingObservableIds.Add(obs.ObservablePath, observableNameList);
                }
                observableNameList.Add(obs.ObservableId);
            }

            categoryNameList = new List<string>();

            // put pathname list to category list
            foreach (string elemKey in categoryHavingObservableIds.Keys)
            {
                categoryNameList.Add(elemKey);
            }
            categoryNameList.Sort();

            for (int i = 0; i < categoryNameList.Count; i++)
            {
                string categoryName = categoryNameList[i];
                if (!categoryHavingObservableIds.ContainsKey(categoryName))
                {
                    continue;
                }
                //get the observable id list that has same observable path
                observableNameList = categoryHavingObservableIds[categoryName];

                ObservableViewItem elemItem = new ObservableViewItem();
                if (isCategoryOnly)
                {
                    elemItem.Expanded = false;
                }
                else
                {
                    elemItem.Expanded = true;
                }
                // this is category name that means observable path name
                elemItem.Name = categoryName;
                elemItem.OwnerName = "Category";
                elemItem.ItemType = ObservableViewItemType.ObservableCategory;

                observableViewItemList.Add(elemItem);    // Add Category First
                // sort observable ids that has same observable path
                observableNameList.Sort();

                if (!isCategoryOnly)
                {
                    for (int j = 0; j < observableNameList.Count; j++)
                    {
                        ObservableViewItem item = new ObservableViewItem();

                        item.Name = observableNameList[j].ToString();  // put the observable id to item's name
                        item.OwnerName = categoryName;

                        item.ItemType = ObservableViewItemType.Observable;
                        // get the observable that has same path and id and put this to item's observable variable
                        item.Observable = observableDictionary[string.Format("{0}:{1}", categoryName, item.Name)];

                        observableViewItemList.Add(item); // Add Observable Itshef, Next

                        // put the observable to global reference observable list and subscribe the event
                        if (!observableList.Contains(item.Observable))
                        {
                            if (observableChangeEventWrapperMap.ContainsKey(item.Observable.ObservableGuid))
                            {
                                try
                                {
                                    ObservableChangeEventWrapper ew = observableChangeEventWrapperMap[item.Observable.ObservableGuid];
                                    item.Observable.ObservableValueChanged -= new DelegateObservableValueChanged(ew.WrapperObservableValueChanged);
                                    ew.WrappedObservableValueChanged -= new DelegateObservableValueChanged(ObservableValueChanged);
                                }
                                catch (Exception ex)
                                {
                                    System.Console.WriteLine("UpdateObservableItemList:" + ex.StackTrace);
                                }
                                observableChangeEventWrapperMap.Remove(item.Observable.ObservableGuid);
                            }

                            ObservableChangeEventWrapper ew2 = new ObservableChangeEventWrapper();
                            ew2.WrappedObservableValueChanged += new DelegateObservableValueChanged(ObservableValueChanged);
                            item.Observable.ObservableValueChanged += new DelegateObservableValueChanged(ew2.WrapperObservableValueChanged);
                            observableList.Add(item.Observable);
                            
                            observableChangeEventWrapperMap.Add(item.Observable.ObservableGuid, ew2);
                        }
                    }
                }
            }
        }

        public void Unsubscribe()
        {
            foreach (Observable obs in this.observables.Values)
            {
                try
                {
                    ObservableChangeEventWrapper ew = observableChangeEventWrapperMap[obs.ObservableGuid];
                    obs.ObservableValueChanged -= new DelegateObservableValueChanged(ew.WrapperObservableValueChanged);
                    ew.WrappedObservableValueChanged -= new DelegateObservableValueChanged(ObservableValueChanged);
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("Unsubscribe():" + ex.StackTrace);
                }
            }
        }


        private void UpdateObservableMapList()
        {
            observableViewItemMap.Clear();

            ObservableViewItem viewItem = null;

            string strUniqueID = string.Empty;

            for (int i = 0; i < observableViewItemList.Count; i++)
            {
                viewItem = observableViewItemList[i];
                if (viewItem.ItemType == ObservableViewItemType.Observable)
                {
                    strUniqueID = viewItem.Observable.ObservableGuid.ToString();
                }
                else
                {
                    continue;
                }
                // observable id can be same with different element like equipmement, subsystem, module
                // we put the index that has same observable id to observable map

                if ( !observableViewItemMap.ContainsKey(strUniqueID))
                {
                    List<int> oIndexList = new List<int>();
                    oIndexList.Add(i);
                    observableViewItemMap.Add(strUniqueID, oIndexList);
                }
                else
                {
                    List<int> oIndexList = observableViewItemMap[strUniqueID];
                    oIndexList.Add(i); // ObservableVeieItems의 i번째 숫자를 ObservableViewItemMap에 추가.
                }
            }
        }

        public void ExpandAll()
        {
            UpdateObservableItemList(false);
            UpdateObservableMapList();
            UpdateScrollBar();
            SetSelectedLine(-1);
            Invalidate();
        }

        public void CollapseAll()
        {
            UpdateObservableItemList(true);
            UpdateObservableMapList();
            UpdateScrollBar();
            SetSelectedLine(-1);
            Invalidate();
        }

        private void ToggleExpandCollapse(int iIndex)
        {
            ObservableViewItem parentItem =  this.observableViewItemList[iIndex]; // 선택 된 Viewer Item을 가져옴.

            // 만약 Cate gory가 아닐 경우 처리 하지 않음.
            if (parentItem.ItemType == ObservableViewItemType.Observable) return;

            parentItem.Expanded = !parentItem.Expanded; // 현재 Expanded 상태를 반대값으로 변경함.

            if (parentItem.Expanded) // 만약 확장 모드이면
            {

                // temporary list that has observable id list
                List<string> observableNameList = new List<string>();

                // obsMap contain key : pathname + obserableId, value: observable object itself
                Dictionary<string, Observable> observableDictionary = new Dictionary<string, Observable>();


                foreach (Observable obs in this.observables.Values)
                {
                    // add observable obejct as value when key is  observable path + observable id 
                    observableDictionary.Add(string.Format("{0}:{1}", obs.ObservablePath, obs.ObservableId), obs);
  
                    if(obs.ObservablePath.Equals(parentItem.Name))
                    {
                        observableNameList.Add(obs.ObservableId);
                    }
                }

                observableNameList.Sort();

                for (int i = 0; i < observableNameList.Count; i++)
                {
                    ObservableViewItem item = new ObservableViewItem();

                    item.Name = observableNameList[i].ToString();  // put the observable id to item's name
                    item.OwnerName = parentItem.Name;

                    item.ItemType = ObservableViewItemType.Observable;
                    // get the observable that has same path and id and put this to item's observable variable
                    item.Observable = observableDictionary[string.Format("{0}:{1}", item.OwnerName, item.Name)];

                    observableViewItemList.Insert(iIndex + 1 + i, item);
                    
                 }
            }
            else  // Collapse 모드의 경우
            {
                int i = iIndex +1;
                while( i < this.observableViewItemList.Count)
                {
                    ObservableViewItem childItem = observableViewItemList[i];
                    if (observableViewItemList[i].OwnerName.Equals(parentItem.Name) &&
                        childItem.ItemType == ObservableViewItemType.Observable)
                    {
                        this.observableViewItemList.RemoveAt(i);
                    }
                    else
                    {
                        break;
                    }
                }
            }

            UpdateObservableMapList();
            UpdateScrollBar();
            Invalidate();

        }


        private void UpdateScrollBar()
        {
            // count number left to scroll
            int scrollMax = observableViewItemList.Count - this.VisibleLines;

            if (startLine > scrollMax) startLine = scrollMax;

            if (scrollMax > 0)
            {
                vsScroll.Visible = true;
                vsScroll.LargeChange = VisibleLines;
                vsScroll.SmallChange = 1;
                vsScroll.Maximum = scrollMax + VisibleLines - 1;
                vsScroll.Value = startLine;
               
            }
            else
            {
                startLine = 0;
                vsScroll.Visible = false;
                vsScroll.Maximum = 0;
            }


        }

        private void UpdateEdit()
        {
            ObservableViewItem item = observableViewItemList[selectedLine];

            tbInPlaceEdit.Update();

            tbInPlaceEdit.SetBounds((nameWidth + INDENTATION) + 4 + INDENTATION * item.Level,
                (selectedLine - startLine) * ItemHeight + 2,
                ClientWidth - (nameWidth + INDENTATION) - vsScroll.Width - 8,
                (selectedLine - startLine + 1) * ItemHeight + 2);



            switch (item.Observable.DataType.Value())
            {
                case DataType._BOOLEAN:
                    break;
                case DataType._BYTE:
                case DataType._INTEGER:
                case DataType._DOUBLE:
                case DataType._SHORT:
                    tbInPlaceEdit.Text = item.Observable.Value.ToString();
                    break;
                case DataType._STRING:
                    tbInPlaceEdit.Text = item.Observable.Value.ToString();
                    break;
                case DataType._SHORT_ARRAY:
                    tbInPlaceEdit.Text = item.Observable.ToString();
                    break;
                case DataType._BOOL_ARRAY:
                    tbInPlaceEdit.Text = item.Observable.ToString();
                    break;
            }

            tbInPlaceEdit.Show();
            tbInPlaceEdit.SelectAll();
            tbInPlaceEdit.Focus();


        }



        private void EndEdit()
        {
            Observable obs = observableViewItemList[selectedLine].Observable;
            try
            {

                switch (obs.DataType.Value())
                {
                    case DataType._BOOLEAN:
                        break;
                    case DataType._BYTE:
                        obs.AsByte = Convert.ToByte(tbInPlaceEdit.Text); break;
                    case DataType._INTEGER:
                        obs.AsInteger = Convert.ToInt32(tbInPlaceEdit.Text); break;
                    case DataType._DOUBLE:
                        obs.AsDouble = Convert.ToDouble(tbInPlaceEdit.Text); break;
                    case DataType._SHORT:
                        obs.AsShort = Convert.ToInt16(tbInPlaceEdit.Text); break;
                    case DataType._STRING:
                        obs.AsString = tbInPlaceEdit.Text; break;
                    case DataType._SHORT_ARRAY:
                        {
                            List<short> a = new List<short>();
                            int length = obs.ConnectionInfo["LENGTH", 0];
                            for (int i = 0; i < length; i++)
                            {
                                a.Add(Convert.ToInt16(tbInPlaceEdit.Text));
                            }
                            obs.AsShortArray = a;
                        }
                        break;
                    case DataType._BOOL_ARRAY:
                        {
                            List<bool> a = new List<bool>();
                            int length = obs.ConnectionInfo["LENGTH", 0];
                            for (int i = 0; i < length; i++)
                            {
                                a.Add(tbInPlaceEdit.Text.Equals("0") ? false : true);
                            }
                            obs.AsBoolArray = a;
                        }
                        break;

                }
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("EndEdit:" + ex.StackTrace);
                CancelEdit();
            }
            CancelEdit();

        }

        private void CancelEdit()
        {
            tbInPlaceEdit.Hide();
            Focus();
        }

        /// <summary>
        ///  선택되어진 Item행의 사각형 영역을 Refresh함
        /// </summary>
        /// <param name="iIndex"></param>
        private void InvalidateIndex(int iIndex)
        {
            if (iIndex >= startLine && iIndex <= startLine + VisibleLines)
            {
                Rectangle rect = new Rectangle(0, (iIndex - startLine) * ItemHeight, ClientWidth, (iIndex - startLine + 1) * ItemHeight);
                Invalidate(rect);
            }
        }

        private bool IsLineVisible(int iIndex)
        {
            return (startLine <= iIndex) && ((startLine + VisibleLines) > iIndex);
        }



        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics gs = e.Graphics;

            /// Viewer Control의 내부를 Window 색깔로 채운다.
            gs.FillRectangle(new SolidBrush(SystemColors.Window), this.ClientRectangle);

            ObservableViewItem Item;

            // 시작 라인에서 Item을 보여 줄수 있는 라인 까지.
            for (int iIndex = startLine; iIndex <= startLine + VisibleLines; iIndex++)
            {
                if (iIndex >= observableViewItemList.Count) break; // 만약 더이상 보여줄 내용이 없을 경우, Break.
                Item = observableViewItemList[iIndex]; // 보여줄 Item을 가져옴.

                int nItemHeadSize = 16; /// 보여줄 라인에서 가장 앞 부분.
                switch (Item.ItemType)
                {
                    // 만약 Item이 카테고리인 경우.
                    case ObservableViewItemType.ObservableCategory:
                    {
                        // 지정된 위치(X : 0, Y: Item들중 Control에서 보여주여주기 시작한 라인에서 상대적 위치)에
                        // Control View 폭과, 아이템의  높이 만큼 사각형을 그린다.
                        Rectangle rectDraw = new Rectangle(0, (iIndex - startLine) * ItemHeight, ClientWidth, ItemHeight);                        
                        gs.FillRectangle(new SolidBrush(Color.PowderBlue), rectDraw.Left, rectDraw.Top, rectDraw.Width, rectDraw.Height -2);
                        
                        // 위에서 그린 사각형아래에 가로선을 긋는다.
                        gs.DrawLine(new Pen(SystemColors.Control), new Point(rectDraw.Left, rectDraw.Bottom - 2), new Point(rectDraw.Right, rectDraw.Bottom - 2));
                        
                        // 카테고리 사각형 내의 좌측에 작은 사각형을 그림
                        Rectangle rectIcon = new Rectangle(4 + INDENTATION * Item.Level, rectDraw.Top + (ItemHeight - 9) / 2, 8, 8);
                        gs.FillRectangle(new SolidBrush(SystemColors.Window), rectIcon);

                        Pen pen = new Pen(SystemColors.ControlDarkDark);
                        gs.DrawRectangle(pen, rectIcon);

                        if (Item.Expanded)
                        {
                            // draw - 
                            gs.DrawLine(pen, rectIcon.Left + 2, rectIcon.Top + rectIcon.Height / 2, rectIcon.Left + rectIcon.Width - 2, rectIcon.Top + rectIcon.Height / 2);
                        }
                        else
                        {
                            // draw +
                            gs.DrawLine(pen, rectIcon.Left + 2, rectIcon.Top + rectIcon.Height / 2, rectIcon.Left + rectIcon.Width - 2, rectIcon.Top + rectIcon.Height / 2);
                            gs.DrawLine(pen, rectIcon.Left + rectIcon.Width / 2, rectIcon.Top + 2, rectIcon.Left + rectIcon.Width / 2, rectIcon.Top + rectIcon.Height - 2);
                        }
                        
                        // Category Name을 표시
                        gs.DrawString(Item.Name, new Font(Font, FontStyle.Bold), new SolidBrush(Color.DarkSlateBlue),
                            rectDraw.Left + rectIcon.Right + 6, rectDraw.Top + (ItemHeight - Font.Height) / 2 - 1);
                        
                        //Tree 연결선
                        
                        //가로 선
                        //gs.DrawLine(pen, INDENTATION * (Item.Level - 1) + (nItemHeadSize / 2) - 1, rectIcon.Top + rectIcon.Height / 2, rectDraw.Left + INDENTATION * Item.Level, rectIcon.Top + rectIcon.Height / 2);
                        
                        //세로 선
                        //if (Item.TreeLineVisible)
                        //{
                        //   // gs.DrawLine(pen, INDENTATION * (Item.Level - 1) + (nItemHeadSize / 2) - 1, rectDraw.Bottom, INDENTATION * (Item.Level - 1) + (nItemHeadSize / 2) - 1, rectDraw.Top);
                        //}
                        //else
                        //{
                        //   // gs.DrawLine(pen, INDENTATION * (Item.Level - 1) + (nItemHeadSize / 2) - 1, rectIcon.Top + rectIcon.Height / 2, INDENTATION * (Item.Level - 1) + (nItemHeadSize / 2) - 1, rectDraw.Top);
                        //}


                        // if this is selected line draw focused box
                        if (iIndex == selectedLine)
                        {
                            rectDraw.Offset(nItemHeadSize - 2, -1);
                            rectDraw.Inflate(0, -1);
                            rectDraw.Size = new Size(rectDraw.Size.Width - (nItemHeadSize + 2), rectDraw.Size.Height);
                            ControlPaint.DrawFocusRectangle(gs, rectDraw);
                            
                        }
                    }
                    break;

                    case ObservableViewItemType.Observable:
                    {
                        // Observable 을 표시하는 Item행의 맨 처음 앞머리를 회색 부분으로 처리
                        Rectangle rectDraw = new Rectangle(0 + INDENTATION * Item.Level, (iIndex - startLine) * ItemHeight, nItemHeadSize, ItemHeight);
                        gs.FillRectangle(new SolidBrush(SystemColors.Control), rectDraw);
                        
                        // ElementTag Name
                        if (iIndex == selectedLine)
                        {
                            // 선택되어진 Observable의 Item 바탕색을 High Light하고, Item Tag 명을 High Light 내에서 표시가능하도록 색깔 변경
                            gs.FillRectangle(new SolidBrush(SystemColors.Highlight), nItemHeadSize + INDENTATION * Item.Level, rectDraw.Top, (nameWidth + INDENTATION) - nItemHeadSize, rectDraw.Height - 1);
                            if (Item.Observable.AccessType != AccessType.IN_OUT)
                                gs.DrawString(Item.Name, new Font(this.Font, FontStyle.Regular), new SolidBrush(SystemColors.HighlightText),
                                    rectDraw.Left + nItemHeadSize + imlType.ImageSize.Width + 8, rectDraw.Top + (ItemHeight - Font.Height) / 2);
                            else
                                gs.DrawString(Item.Name, new Font(this.Font, FontStyle.Regular), new SolidBrush(SystemColors.HighlightText),
                                    rectDraw.Left + nItemHeadSize + imlType.ImageSize.Width + 2, rectDraw.Top + (ItemHeight - Font.Height) / 2);
                        }
                        else
                        {
                            if (Item.Observable.AccessType != AccessType.IN_OUT)
                                gs.DrawString(Item.Name, new Font(this.Font, FontStyle.Regular), new SolidBrush(this.ForeColor),
                                rectDraw.Left + nItemHeadSize + imlType.ImageSize.Width + 8, rectDraw.Top + (ItemHeight - Font.Height) / 2);
                            else
                                gs.DrawString(Item.Name, new Font(this.Font, FontStyle.Regular), new SolidBrush(this.ForeColor),
                                       rectDraw.Left + nItemHeadSize + imlType.ImageSize.Width + 2, rectDraw.Top + (ItemHeight - Font.Height) / 2);
                        }

                        // 만약 Observble이 모니터링 상태일 경우 Image List의 3번째 Image로 표시함
                        if (Item.Observable.IsMonitoring)
                        {
                            gs.DrawImage(imlType.Images[3], 3 + INDENTATION * Item.Level, rectDraw.Top + (ItemHeight - imlType.ImageSize.Height) / 2);
                        }

                        // Observable의 Type(In, or Out)에 따른 Image표시
                        gs.DrawImage(imlType.Images[Item.Observable.AccessType.Value()], nItemHeadSize + 4 + INDENTATION * Item.Level, rectDraw.Top + (ItemHeight - imlType.ImageSize.Height) / 2);

                        // Observable의 Name과 값의 분리선을 그린다.
                        Pen pen = new Pen(SystemColors.Control);
                        gs.DrawLine(pen, (nameWidth + INDENTATION) + INDENTATION * Item.Level, rectDraw.Top, (nameWidth + INDENTATION) + INDENTATION * Item.Level, rectDraw.Bottom - 1);

                        // Observable Item간의 가로 분리선을 그린다.
                        gs.DrawLine(pen, nItemHeadSize + INDENTATION * Item.Level, rectDraw.Bottom - 1, ClientWidth, rectDraw.Bottom - 1);

                        //Tree 연결 세로 선
                        //if (Item.TreeLineVisible)
                        //{
                        //    pen.Color = SystemColors.ControlDarkDark;
                        //    gs.DrawLine(pen, 7, rectDraw.Top, 7, rectDraw.Bottom);
                        //}

                        // ElementTag Value
                        gs.FillRectangle(new SolidBrush(SystemColors.Window),
                                      rectDraw.Left + NameWidth + 1 + INDENTATION, rectDraw.Top + 1, ClientWidth - NameWidth - 1, rectDraw.Height - 2);

                        string strValue = "";

                        switch (Item.Observable.DataType.Value())
                        {
                            // Item 값이 논리 값일 경우
                            case DataType._BOOLEAN:
                                Brush brush = null;
                                if (((bool)Item.Observable.Value))
                                    brush = new SolidBrush(Color.Lime); // True일 경우 사각형을 채움
                                else
                                    brush = new SolidBrush(SystemColors.Window);  // False 일 경우 사각형을 비움

                                rectDraw.Inflate(0, -4);
                                gs.FillRectangle(brush, (nameWidth + INDENTATION) + 5 + INDENTATION * Item.Level, rectDraw.Top - 1, ItemHeight - 8, ItemHeight - 8);
                                gs.DrawRectangle(new Pen(SystemColors.ButtonShadow), (nameWidth + INDENTATION) + 5 + INDENTATION * Item.Level, rectDraw.Top - 1, ItemHeight - 8, ItemHeight - 8);
                                break;
                            case DataType._BYTE:
                            case DataType._INTEGER:
                            case DataType._DOUBLE:
                            case DataType._SHORT:
                                if (this.hexMode)
                                {
                                    strValue = String.Format("0x{0:X}", Item.Observable.Value);
                                }
                                else
                                {
                                    strValue = Item.Observable.Value.ToString();
                                }
                                break;
                            case DataType._STRING:
                                strValue = Item.Observable.Value.ToString();
                                break;
                            case DataType._SHORT_ARRAY:
                                strValue = Item.Observable.ToString();
                                break;
                            case DataType._BOOL_ARRAY:
                                strValue = Item.Observable.ToString();
                                break;


                        }
                        // Observable 이 논리값이 아닐 경우, String 형태로 표시함
                        gs.DrawString(strValue, new Font(this.Font, FontStyle.Regular), new SolidBrush(this.ForeColor),
                         (nameWidth + INDENTATION) + 3 + INDENTATION * Item.Level, rectDraw.Top + (ItemHeight - Font.Height) / 2);
                    }
                    break;
                }
                
            }//for
        }

        private void ObservableViewControl_MouseWheel(object sender, MouseEventArgs e)
        {
            if (e.Delta < 0)
            {
                if (vsScroll.Value < ( observableViewItemList.Count - VisibleLines))
                    vsScroll.Value = vsScroll.Value + 1;
            }
            else
            {
                if (vsScroll.Value > vsScroll.Minimum)
                    vsScroll.Value = vsScroll.Value - 1;
            }
        }

        private void ObservableViewControl_Resize(object sender, EventArgs e)
        {
            this.UpdateScrollBar();
        }

        private void ObservableViewControl_MouseDown(object sender, MouseEventArgs e)
        {
            int iIndex = startLine + (e.Y / ItemHeight);
            if (iIndex >= observableViewItemList.Count)
                return;

            ObservableViewItem item = observableViewItemList[iIndex];

            this.Focus();
            if (e.Button == MouseButtons.Left)
            {
                if (e.X >= (nameWidth + INDENTATION) - 1 + INDENTATION * item.Level &&
                    e.X <= (nameWidth + INDENTATION) + 1 + INDENTATION * item.Level)
                {
                    CancelEdit();
                   // CancelComboBox();
                    isSizing = true;
                }
                else
                {
                    CancelEdit();
                    //CancelComboBox();
                    InvalidateIndex(selectedLine);
                    SetSelectedLine(iIndex);
                    InvalidateIndex(selectedLine);

                    //OBJECT, ObservableCATEGORY의 +- 표시 Box
                    Rectangle rectPlusMinus = new Rectangle(2 + INDENTATION * item.Level, (iIndex - startLine) * ItemHeight + (ItemHeight - 9) / 2, 8, 8);

                    switch (item.ItemType)
                    {
                         case ObservableViewItemType.ObservableCategory:
                            if (rectPlusMinus.IntersectsWith(new Rectangle(e.X, e.Y, 1, 1)))
                                ToggleExpandCollapse(iIndex);
                            break;
                        case ObservableViewItemType.Observable:
                            if (item.Observable.DataType != DataType.BOOLEAN)
                            {
                                if (accessMode == ObservableViewAccessMode.Simulation || item.Observable.AccessType != AccessType.IN)
                                {
                                    UpdateEdit();
                                }
                            }
                            break;         
                    }
                }
            }

        }

        private void ObservableViewControl_MouseMove(object sender, MouseEventArgs e)
        {
            int iIndex = startLine + (e.Y / ItemHeight);
            if (iIndex >= observableViewItemList.Count)
                return;

            ObservableViewItem item = observableViewItemList[iIndex];

            if (isSizing)
            {
                int indentLength = INDENTATION + INDENTATION * item.Level;
                int width = e.X - indentLength;

                if (width < 5)
                {
                    this.NameWidth = 5;
                }
                else if (width > this.ClientWidth - indentLength - vsScroll.Width + 13)
                {
                    this.NameWidth = this.ClientWidth - indentLength - vsScroll.Width + 13;
                }
                else
                {
                    this.NameWidth = width;
                }

                //this.NameWidth =  (width > 0) ? width : 0;
            }
            else
            {
                if (e.X >= (nameWidth + INDENTATION) - 1 + INDENTATION * item.Level && e.X <= (nameWidth + INDENTATION) + 1 + INDENTATION * item.Level)
                    Cursor = Cursors.VSplit;
                else
                    Cursor = Cursors.Default;
            }
        }

        private void ObservableViewControl_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (isSizing)
                    isSizing = false;
            }
        }

        private void ObservableViewControl_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            Focus();
            int iIndex = startLine + (e.Y / ItemHeight); // Double Click 되어진 Item의 행 번호 취출
            if (iIndex >= observableViewItemList.Count) // 만약 행번호가 실제 보여줄 내용 보다 큰경우 처리안함.
                return;

            ObservableViewItem item = observableViewItemList[iIndex]; // 행번호에 해당하는 Item 취출
            if (e.Button == MouseButtons.Left)
            {
                switch (item.ItemType)
                {
                    case ObservableViewItemType.ObservableCategory:
                        this.ToggleExpandCollapse(iIndex);
                        break;
                    case ObservableViewItemType.Observable:
                        if (item.Observable.DataType == DataType.BOOLEAN)
                        {                            
                            if (accessMode == ObservableViewAccessMode.Simulation ||
                                item.Observable.AccessType != AccessType.IN)
                            {
                                // Observabel이 논리 값이고, 만약 Out 또는 In/Out일 경우 Observable에 현재의 반대값을 설정한다.
                                item.Observable.AsBoolean = !(bool)item.Observable.Value;
                            }
                        }
                        break;
                }
            }
            else if (e.Button == MouseButtons.Right)
            {
                if (item.ItemType == ObservableViewItemType.Observable && !item.Observable.IsMonitoring)
                {
                    // 라이트 Click이고 이값이 모니터링 되는 값이 아닐 경우, Observable에서 값을 읽어옴.
                    item.Observable.ReadValueSync(false);
                }
            }

        }

        private void ObservableViewControl_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            int iNewLine = selectedLine;
            switch (e.KeyCode)
            {
                case Keys.PageUp:
                    iNewLine -= VisibleLines;
                    break;
                case Keys.PageDown:
                    iNewLine += VisibleLines;
                    break;
                case Keys.Up:
                    iNewLine--;
                    break;
                case Keys.Down:
                    iNewLine++;
                    break;
                case Keys.Return:
                    if (selectedLine >= 0)
                    {
                        ObservableViewItem item = observableViewItemList[selectedLine];
                        if (item.ItemType == ObservableViewItemType.Observable)
                        {
                            if (accessMode == ObservableViewAccessMode.Simulation ||
                                item.Observable.AccessType != AccessType.IN)
                            {
                                if (item.Observable.DataType != DataType.BOOLEAN)
                                {
                                    UpdateEdit();
                                }
                                else
                                {
                                    item.Observable.AsBoolean = !(bool)item.Observable.Value;
                                }
                            }
                        }
                    }
                    break;
            }
            if (iNewLine != selectedLine)
            {
                if (iNewLine < 0) iNewLine = 0;
                if (iNewLine > observableViewItemList.Count - 1) iNewLine = observableViewItemList.Count - 1;

                if (!IsLineVisible(iNewLine))
                {
                    int iOldTopLine = startLine;
                    startLine = startLine + (iNewLine - selectedLine);

                    if (startLine < 0) startLine = 0;
                    if (startLine != iOldTopLine)
                    {
                        UpdateScrollBar();
                        Invalidate();
                    }
                }

                if (iNewLine != selectedLine)
                {
                    InvalidateIndex(selectedLine);
                    selectedLine = iNewLine;
                    InvalidateIndex(selectedLine);
                }
            }
            //Focus();

        }

        private void VsScroll_ValueChanged(object sender, EventArgs e)
        {
            this.CancelEdit();
            startLine = vsScroll.Value;
            Invalidate();
        }



        private void TbInPlaceEdit_KeyDown(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.PageUp:
                case Keys.PageDown:
                case Keys.Up:
                case Keys.Down:
                    CancelEdit();
                    ObservableViewControl_PreviewKeyDown(this, new PreviewKeyDownEventArgs(e.KeyData));
                    break;
                case Keys.Escape:
                    CancelEdit();
                    break;
                case Keys.Return:
                    EndEdit();
                    break;
            }

        }

        /// <summary>
        /// Overrider Method임
        /// 특정 DialogKey의 동작을 Disable하여 Preview Key의 오동작 방지 위함.
        /// </summary>
        /// <param name="keyData">누른 Dialog 키 값</param>
        /// <returns></returns>
        protected override bool ProcessDialogKey(Keys keyData)
        {
            bool bRet = false;
            switch (keyData)
            {
                case Keys.Up:
                case Keys.Down:
                    bRet = true;
                    break;
                case Keys.Return:
                    if (this.IsEditing)
                    {
                        bRet = base.ProcessDialogKey(keyData);
                    }
                    else
                    {
                        bRet = true;
                    }
                    break;
                default:
                    bRet = base.ProcessDialogKey(keyData);
                    break;
            }

            return bRet;
        }

        private void menuExpandAll_Click(object sender, EventArgs e)
        {
            this.ExpandAll();
        }

        private void menuCollapseAll_Click(object sender, EventArgs e)
        {
            this.CollapseAll();
        }
    }
}
