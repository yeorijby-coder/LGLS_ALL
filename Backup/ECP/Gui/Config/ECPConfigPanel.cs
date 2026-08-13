using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Reflection;
using System.Runtime.Remoting;

using ECP.Gui.Widget;
using ECP.Global.Gui;
using ECP.Service.Logger;
using ECP.Gui.Util;
using ECP.Global;
using ECP.Gui.Config.Dialogs.Property;
using ECP.Util;
using ECP.Util.Client;
using ECP.Service.User.PermissionTarget;
using ECP.Service.User;
using ECP.Gui.Config.Dialogs;

namespace ECP.Gui.Config
{
    [Serializable]
    public class ECPConfigPanel : Panel
    {

        #region Constant
        //사용자 권한 문자열 상수
        const string PERMISSION_ELEMENT_ADD = "Client.AddElement";
        const string PERMISSION_ELEMENT_DELETE = "Client.DeleteElement";
        const string PERMISSION_PROPERTY_ACCESS = "Client.Property";
        const string PERMISSION_WIDGET_DROW = "Client.DrowWidget"; 

        private const string CLASS = "ECPConfigPanel";

        public const int RESIZE_TOLERANCE = 4;

        public const int MIN_WIDGET_SIZE = 15;

        public const int SCROLL_SPEED = 0;

        
        private const string DELETE = "Delete";

        private const string CUT_SELECTION = "Cut Selection";

        private const string COPY_SELECTION = "Copy Selection";

        private const string COPY_PROPERTY = "Copy Property";

        private const string PASTE = "Paste";

        private const string PASTE_PROPERTY = "Paste Property";

        private const string PASTE_UNCONFIGURED = "Paste Unconfigured";

        private const string SELECT_ALL = "Select All";

        private const string SHOW_GRID = "Show Grid";

        private const string ENABLE_SNAPPING = "Enable Snapping";

        private const string DISABLE_SNAPPING = "Disable Snapping";

        private const string ALIGN_LEFT = "Align Left";

        private const string ALIGN_RIGHT = "Align Right";

        private const string ALIGN_TOP = "Align Top";

        private const string ALIGN_BOTTOM = "Align Bottom";

        private const string CENTER_VERTICAL = "Center Vertical";

        private const string CENTER_HORIZONTAL = "Center Horizontal";

        private const string SNAP_TO_GRID = "Snap To Grid";

        private const string SAME_SIZE = "Same Size";

        private const string SAME_DISTANCE = "Same Distance";

        private const string EDIT_SIZE = "Edit Size";

        private const string MOVE_TO_FRONT = "Move To Front";

        private const string MOVE_TO_BACK = "Move To Back";

        private const string GROUP = "Group";

        private const string UNGROUP = "Ungroup";

        private const string EDIT = "Edit";


        private const string HIDE_GRID = "Hide Grid";

        private const string SEARCH = "Search";

        private const string SWITCH_DIRECTION =
            "Switch Direction";

        private const string TRACK_CHECK =
            "Validate Track";

        private const string ADJUST_DISTANCES =
            "Adjust Distances";

        private const string ADJUST_SPEEDS =
            "Adjust Speeds";
        private const string GRID_SIZE =
            "Grid Size";

        private const string UNDO = "Undo";

        private const string REDO = "Redo";

        private const string WIDGET_PROPERTIES = "Properties";


        private const string WIDGET_UDPROPERTIES = "UDProperties";

        private const int DEFAULT_GRID_SIZE = 5;

        public const  int TEXTFIELD_WIDTH = 150;

        public const  int TEXTFIELD_HEIGHT = 20;

        #endregion

        #region Attribute
        private int gridSize = DEFAULT_GRID_SIZE;

        public int GridSize
        {
            get { return gridSize; }
            set { gridSize = value; }
        }

        /**
        * Are we dragging a rectangle
        */

        protected bool rubberSelectDrag = false;

        /**
         * Where did this mouse action start
         */
        protected Point pStart = new Point();

        /**
         * Where was the mouse last.  Null means no old position
         */
        protected Point pOld = Point.Empty;

        /**
         * Where is the mouse now
         */
        protected Point pNew = new Point();

        private int editingPoint = LineWidget.POINT_1;

        private Color gridColor = Color.LightBlue;

        private int defaultWidgetSize = 80;

        private bool showGrid = true; // Grid initally on or off
        private bool snappingEnabled = false;

        ECPConfigForm owner = null;

        public ECPConfigForm Owner
        {
            get { return owner; }
            set { owner = value; }
        }

        private bool rubberSelected = false;

        bool snappingToLine = false;

        Point mousePosition = new Point(0, 0);

        public List<BaseWidget> selection = new List<BaseWidget>();
        public List<BaseWidget> clipboard = new List<BaseWidget>();

        TextWidget label;

        TextBox labelTextField;

        private bool textEditMode = false;
        protected bool dragFlag = false;
        protected bool widgetPressed = false;
        protected bool panelPressed = false;
        private bool resizeActive = false;
        private bool lineDrawFlag = false;

        bool isControlKeyPressed = false;
        bool isShiftKeyPressed = false;

        private Rectangle resizeRect = new Rectangle();
        private Rectangle widgetOldBounds = new Rectangle();

        private WidgetListForm widgetListForm = null;
        private ContextMenuStrip globalMenu;
        private System.ComponentModel.IContainer components;
        private ToolStripMenuItem gloCutItem;
        private ToolStripMenuItem gloCopyItem;
        private ToolStripMenuItem gloPasteItem;
        private ContextMenuStrip selectMenu;
        private ToolStripMenuItem gloDeleteItem;
        private ToolStripMenuItem gloSelectAll;
        private ToolStripMenuItem globSearch;
        private ToolStripSeparator gloSeparator1;
        private ToolStripMenuItem gloShowGridItem;
        private ToolStripMenuItem gloSnappingItem;
        private ToolStripMenuItem selEditItem;
        private ToolStripMenuItem selCutItem;
        private ToolStripMenuItem selCopyItem;
        private ToolStripMenuItem selPasteItem;
        private ToolStripMenuItem selDeleteItem;
        private ToolStripMenuItem selSelectAllItem;
        private ToolStripMenuItem selAlignLeftItem;
        private ToolStripMenuItem selAlignRightItem;
        private ToolStripMenuItem selAlignTopItem;
        private ToolStripMenuItem selCenterVItem;
        private ToolStripMenuItem selCenterHItem;
        private ToolStripMenuItem selSnapToGridItem;
        private ToolStripMenuItem selSameSizeItem;
        private ToolStripSeparator selSeparator;
        private ToolStripMenuItem selMoveFrontItem;
        private ToolStripSeparator gloSeparator2;
        private ToolStripMenuItem gloGridSize;
        private ToolStripMenuItem selAlignBottomItem;
        private OpenFileDialog imageChooser;
        private ToolStripMenuItem selPropertyItem;
        private ToolStripSeparator toolStripSeparator1;
        private ToolStripMenuItem selMoveBackItem;
        private ToolStripMenuItem selUDPropertyItem;
        private ToolStripMenuItem selPasteUnconfiguredItem;
        private ToolStripMenuItem gloPasteUnconfigured;
        private ToolStripMenuItem selCopyPropertyItem;
        private ToolStripMenuItem selPastePropertyItem;
        private ToolStripMenuItem gloCopyPropertyItem;
        private ToolStripMenuItem gloPastePropertyItem;
        private ToolStripMenuItem selEditSizeItem;
        private ToolStripMenuItem selSameDistanceItem;
        private ToolStripMenuItem gloUndo;
        private ToolStripMenuItem gloRedo;

        private GlobalKeyboardHook globalKeyHooker = new GlobalKeyboardHook();
        
        #endregion

        #region Constructor
        public ECPConfigPanel()
        {
            this.InitializeComponent();
            this.InitializePermission();

            this.SetStyle(ControlStyles.OptimizedDoubleBuffer |
                          ControlStyles.AllPaintingInWmPaint |
                          ControlStyles.UserPaint, true);

            this.GlobalKeyBInding();
            this.ContextMenuStrip = this.globalMenu;

            this.Focus();
        } 
        #endregion

        private void InitializePermission()
        {
            PermissionTarget target;
            target = new TargetTypeConstructor();

            List<IPermission> targets = new List<IPermission>();
            targets.Add(target.CreateType(PERMISSION_ELEMENT_DELETE, this.gloDeleteItem));
            targets.Add(target.CreateType(PERMISSION_ELEMENT_DELETE, this.selDeleteItem));
            targets.Add(target.CreateType(PERMISSION_ELEMENT_DELETE, this.gloCutItem));
            targets.Add(target.CreateType(PERMISSION_ELEMENT_DELETE, this.selCutItem));

            targets.Add(target.CreateType(PERMISSION_ELEMENT_ADD, this.gloPasteItem));
            targets.Add(target.CreateType(PERMISSION_ELEMENT_ADD, this.gloCopyItem));
            targets.Add(target.CreateType(PERMISSION_ELEMENT_ADD, this.selPasteItem));
            targets.Add(target.CreateType(PERMISSION_ELEMENT_ADD, this.selCopyItem));
            targets.Add(target.CreateType(PERMISSION_ELEMENT_ADD, this.gloPasteUnconfigured));
            targets.Add(target.CreateType(PERMISSION_ELEMENT_ADD, this.selPasteUnconfiguredItem));

            targets.Add(target.CreateType(PERMISSION_PROPERTY_ACCESS, this.selPropertyItem));
            targets.Add(target.CreateType(PERMISSION_PROPERTY_ACCESS, this.gloCopyPropertyItem));
            targets.Add(target.CreateType(PERMISSION_PROPERTY_ACCESS, this.gloPastePropertyItem));
            targets.Add(target.CreateType(PERMISSION_PROPERTY_ACCESS, this.selCopyPropertyItem));
            targets.Add(target.CreateType(PERMISSION_PROPERTY_ACCESS, this.selPastePropertyItem));

            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.gloGridSize));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.gloShowGridItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.gloSnappingItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selAlignBottomItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selAlignLeftItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selAlignRightItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selAlignTopItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selSameDistanceItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selSameSizeItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selCenterHItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selCenterVItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selEditSizeItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selSnapToGridItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selMoveBackItem));
            targets.Add(target.CreateType(PERMISSION_WIDGET_DROW, this.selMoveFrontItem));

            UserPermission.AddTarget(CLASS, targets);
            UserPermission.GetInstance().DisableFuncationByUserPermission();
        }

        private void GlobalKeyBInding()
        {
            this.globalKeyHooker.KeyDown +=new KeyEventHandler(globalKeyHooker_KeyDown);
            this.globalKeyHooker.KeyUp += new KeyEventHandler(globalKeyHooker_KeyUp);
        }

        void globalKeyHooker_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == (Keys.RButton | Keys.Space | Keys.F17))  this.isControlKeyPressed = true;                
            if (e.KeyCode == (Keys.Space | Keys.F17)) this.isShiftKeyPressed = true;

            if (e.KeyCode == Keys.Up)
            {
                int nudgeSize = 1;

                NudgeSelection(0, -nudgeSize);

            }
            else if (e.KeyCode == Keys.Down)
            {
                int nudgeSize = 1;

                NudgeSelection(0, nudgeSize);
            }
            else if (e.KeyCode == Keys.Left)
            {
                int nudgeSize = 1;

                NudgeSelection(-nudgeSize, 0);
            }
            else if (e.KeyCode == Keys.Right)
            {
                int nudgeSize = 1;
                NudgeSelection(nudgeSize, 0);

            }
        }

        void globalKeyHooker_KeyUp(object sender, KeyEventArgs e)
        {
          //  if (e.KeyCode == (Keys.Space | Keys.F17 )) this.isControlKeyPressed = false;
          //  if (e.KeyCode == (Keys.Space | Keys.F17 )) this.isShiftKeyPressed = false;   
            this.isControlKeyPressed = false;
            this.isShiftKeyPressed = false;
        }

        public BaseWidget CreateWidget(WidgetData data, bool undoEnabled, bool addIt)
        {
            const string METHOD = CLASS + ".CreateWidget";

            BaseWidget newWidget = null;

            ObjectHandle objHandle = null;

            try
            {
                objHandle = Activator.CreateInstance(data.widgetAssemblyName, data.widgetClassName);
                newWidget = (BaseWidget)objHandle.Unwrap();
            }
            catch (Exception ex)
            {
                try
                {
                    objHandle = Activator.CreateInstance("Stocker", data.widgetClassName);
                    newWidget = (BaseWidget)objHandle.Unwrap();
                    data.widgetAssemblyName = "Stocker";
                }
                catch (Exception ex2)
                {
                    try
                    {
                        objHandle = Activator.CreateInstance("ECP", data.widgetClassName);
                        newWidget = (BaseWidget)objHandle.Unwrap();
                        data.widgetAssemblyName = "ECP";
                    }
                    catch (Exception ex3)
                    {
                        Log.log(5, METHOD, "error", Category.ERROR, "", "config panel",
                        "failed to reflect widget : " + data.elementId +
                        "\n" + ex3.Message.ToString());
                        return null;
                    }
                }
            }

            data.x = Snap(data.x);
            data.y = Snap(data.y);



            newWidget.WidgetData = data;

            if (!(newWidget is LineWidget) && !(newWidget is LineWidgetWithArrow))
            {
                newWidget.SetBounds(data.x + this.AutoScrollPosition.X, data.y + this.AutoScrollPosition.Y,
                    data.width, data.height);
            }
            else if ((newWidget is LineWidget) || (newWidget is LineWidgetWithArrow))
            {
                newWidget.WidgetData.x = data.x - this.AutoScrollPosition.X;
                newWidget.WidgetData.y = data.y - this.AutoScrollPosition.Y;
                newWidget.WidgetData.width = data.width - this.AutoScrollPosition.X;
                newWidget.WidgetData.height = data.height - this.AutoScrollPosition.Y;
            }           


            if (newWidget is PictureWidget)
            {
                PictureWidget pw = (PictureWidget)newWidget;
                if (!pw.IsImageConfigured())
                {

                    this.Cursor = Cursors.WaitCursor;
                    try
                    {
                        string res = null;
                        if (pw.PicturePath == null ||
                            pw.PicturePath == string.Empty)
                        {
                            DialogResult result = this.imageChooser.ShowDialog();

                            if (result == DialogResult.OK)
                            {
                                res = this.imageChooser.FileName;

                            }
                            else
                            {
                            }
                        }
                        else
                        {
                            res = pw.PicturePath;
                        }

                        if (res == null || res == string.Empty)
                        {
                            // bail out
                            return null;
                        }
                        //pw.SetImage(res);
                        pw.PicturePath = res;
                    }
                    finally
                    {
                        this.Cursor = Cursors.Arrow;
                    }
                }
            }

            if (addIt)
            {
                newWidget.MouseUp += new MouseEventHandler(this.MouseReleasedHandler);
                newWidget.MouseDown += new MouseEventHandler(this.MousePressedHandler);
                newWidget.MouseLeave += new EventHandler(this.MouseExitHandler);
                if (UserPermission.GetInstance().HasPermission(PERMISSION_WIDGET_DROW))
                {
                    newWidget.MouseMove += new MouseEventHandler(this.MouseMoveHandler);
                }

                newWidget.KeyUp += new KeyEventHandler(this.KeyUpHandler);
                newWidget.KeyDown += new KeyEventHandler(KeyDownHandler);
                newWidget.SetElementIDAtDesignMode(newWidget.ElementId);
                this.Controls.Add(newWidget);
            }
         
            return newWidget;
        }

        void KeyDownHandler(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ControlKey) this.isControlKeyPressed = true;
            if (e.KeyCode == Keys.ShiftKey) this.isShiftKeyPressed = true;
            
        }

        void KeyUpHandler(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.ControlKey) this.isControlKeyPressed = false;
            if (e.KeyCode == Keys.ShiftKey) this.isShiftKeyPressed = false;            
        }  

        void NudgeSelection(int x, int y)
        {
            BaseWidget widget;
            bool isFocused = false;
            for (int i = 0; i < this.selection.Count; i++)
            {
                widget = (BaseWidget)selection[i];
                if (widget.Focused)
                {
                    isFocused = true;
                    break;
                }
            }
            if (!isFocused) return;
            this.SuspendLayout();
            for (int i = 0; i < this.selection.Count; i++)
            {
                widget = (BaseWidget)selection[i];
                Rectangle b = widget.Bounds;

                int nextX = b.X + x;
                int nextY = b.Y + y;
                

                if (nextX < 0)
                {
                    if(this.AutoScrollPosition.X ==0)
                        x -= nextX;  // x = -b.X
                }
                if (nextY < 0)
                {
                    if(this.AutoScrollPosition.Y ==0)
                        y -= nextY;  // y = -b.Y
                }
            }


            for (int i = 0; i < selection.Count; i++)
            {
                widget = (BaseWidget)selection[i];
                Rectangle b = widget.Bounds;
                int nextX = b.X + x;
                int nextY = b.Y + y;

                widget.SetBounds(nextX, nextY, widget.Width, widget.Height);

                widget.WidgetData.x = nextX - this.AutoScrollPosition.X;
                widget.WidgetData.y = nextY - this.AutoScrollPosition.Y;

                if (!this.ClientRectangle.Contains(nextX, nextY) ||
                    !this.ClientRectangle.Contains(nextX + widget.Width,
                     nextY + widget.Height))
                {
                    this.SetDisplayRectLocation(this.AutoScrollPosition.X - x, this.AutoScrollPosition.Y-y);
                }
                //owner.Text = " Width" + this.Width + " Height" + this.Height;
            }
            this.Update();
            this.ResumeLayout();
        }

        protected override bool ProcessDialogKey(Keys keyData)
        {
            bool bRet = false;
            switch (keyData)
            {
                case Keys.Up:
                case Keys.Down:
                case Keys.Left:
                case Keys.Right:
                    bRet = true;
                    break;
                default:
                    bRet = base.ProcessDialogKey(keyData);
                    break;
            }
            return bRet;
        }
       
        public List<WidgetData> GetWidgetDatas()
        {
            List<WidgetData> wdList = new List<WidgetData>();

            Point p = this.AutoScrollPosition;
    
            BaseWidget widget = null;
            foreach (Control c in this.Controls)
            {
                if (c is BaseWidget)
                {
                    widget = (BaseWidget)c;
                    wdList.Add(widget.WidgetData);
                }
            }
     
            return wdList;
        }

        void MouseEnterHandler(object sender, EventArgs e)
        {

        }
        void MouseExitHandler(object sender, EventArgs e)
        {

        }
        void MouseMoveHandler(object sender, MouseEventArgs e)
        {

          //  owner.Text ="Sc X"+this.AutoScrollPosition.X+" Sc Y"+this.AutoScrollPosition.Y+
           //     "Mo X"+e.X+" Mo Y"+e.Y;

          //  owner.Text ="Sc X"+this.AutoScrollOffset.X+" Sc Y"+this.AutoScrollOffset.Y+
          //      "Mo X"+e.X+" Mo Y"+e.Y;
            
            
            Object obj = sender;

            if (!lineDrawFlag)
            {
                if (obj is BaseWidget)
                {
                    BaseWidget widget = (BaseWidget)obj;
                    this.mousePosition = new Point(widget.Location.X + e.X, widget.Location.Y + e.Y);
                }
                else
                {
                    this.mousePosition = e.Location;
                }

                if (obj is BaseWidget && owner.SelectedButtionId == 0)
                {
                    if (this.widgetPressed) // dragging
                    {

                        BaseWidget widget = (BaseWidget)obj;

                        if (selection.Contains(widget))
                        {
                            Widget_MouseDrag(sender, e);
                        }
                        else
                        {

                            if (!this.isShiftKeyPressed && !this.isControlKeyPressed)
                            {
                                this.ClearSelection();
                            }

                            this.SelectWidget(widget, true);
                            //Widget_MouseDrag(sender, e); //Layout 창을 걸치는 widget을 클릭하게되면 widget이 이동됨..

                        }
                    }
                    else
                    {
                        this.CheckResize(sender, e);
                    }
                }
            }
            if (!(obj is BaseWidget) || (lineDrawFlag && obj is BaseWidget)) //&& 
            //   // !this.isShiftKeyPressed && !this.isControlKeyPressed)
            //     !this.isControlKeyPressed)
            {
                if (this.panelPressed)
                {
                    if (obj is BaseWidget)
                    {
                        BaseWidget w = (BaseWidget)obj;
                        e = new MouseEventArgs(e.Button, e.Clicks, w.Location.X + e.X, w.Location.Y+e.Y, e.Delta);
                    }

                    Panel_MouseDragged(sender, e);
                }
                else
                {
                    this.Panel_MouseMoved(sender, e);
                }
            }    
        }
        void MousePressedHandler(object sender, MouseEventArgs e)
        {

            if ((owner.SelectedButtionId != ECPConfigForm.ARROW)
                /*&& !(sender is BaseWidget)*/)
            {
                if (sender.GetType() != this.GetType() && sender is BaseWidget)
                {
                    BaseWidget w = (BaseWidget)sender;
                    e = new MouseEventArgs(e.Button, e.Clicks, e.X + w.Location.X, e.Y + w.Location.Y, e.Delta);
                }
                this.Panel_MousePressed(sender, e);
                return;
            }

            Object obj = sender;

            //if (this.resizeActive && selection.Count == 1)
            //{
            //    obj = selection[0];
            //}

            if (obj is BaseWidget &&
                 owner.SelectedButtionId == 0)
            {
                this.Widget_MousePress(obj, e);

            }
            else if (!(obj is BaseWidget))
            {
                this.Panel_MousePressed(obj, e);
            }
        }
        void MouseReleasedHandler(object sender, MouseEventArgs e)
        {
            if (sender is BaseWidget && owner.SelectedButtionId == 0)
            {
                Widget_MouseReleased(sender, e);
            }
            if (sender is BaseWidget && owner.SelectedButtionId != ECPConfigForm.ARROW)
            {
                BaseWidget w = (BaseWidget)sender;
                e = new MouseEventArgs(e.Button, e.Clicks,w.Location.X+e.X, w.Location.Y+e.Y, e.Delta);
                this.Panel_MouseReleased(sender, e);
            }
            else if (!(sender is BaseWidget))
            {
                this.Panel_MouseReleased(sender, e);
            }
        }

        protected void Panel_MouseDragged(object sender, MouseEventArgs e)
        {
            this.dragFlag = true;

            if (lineDrawFlag)
            {
                pNew = e.Location;

                this.Cursor = Cursors.Cross;

                if (this.isShiftKeyPressed)
                {
                    int dx = pNew.X - pStart.X;
                    int dy = pNew.Y - pStart.Y;

                    if (Math.Abs(Math.Abs(dx) - Math.Abs(dy)) < Math.Abs(dx) / 2)
                    {
                        if (dx * dy < 0)
                        {
                            pNew.X = pStart.X - dy;
                        }
                        else
                        {
                            pNew.X = pStart.X + dy;
                        }

                    }
                    else if (Math.Abs(dx) > Math.Abs(dy))
                    {
                        pNew.Y = pStart.Y;
                    }
                    else
                    {
                        pNew.X = pStart.X;
                    }
                }
                PaintLine();
                pOld = pNew;
            }
            else if (this.rubberSelectDrag)
            {
                pNew = e.Location;
                this.PaintRect();
                pOld = pNew;

            }
            else if (selection.Count == 1 && resizeActive == true)
            {
                //// They are resizing the widgets.
                BaseWidget widget = this.selection[0];
                Rectangle b = widget.GetConfigBounds();

                if (widget is LineWidget)
                {
                    pNew = e.Location;


                    this.Cursor = Cursors.Cross;

                    if (this.isShiftKeyPressed)
                    {
                        int dx = pNew.X - pStart.X;
                        int dy = pNew.Y - pStart.Y;

                        if (Math.Abs(Math.Abs(dx) - Math.Abs(dy)) < Math.Abs(dx) / 2)
                        {
                            if (dx * dy < 0)
                            {
                                pNew.X = pStart.X - dy;
                            }
                            else
                            {
                                pNew.X = pStart.X + dy;
                            }

                        }
                        else if (Math.Abs(dx) > Math.Abs(dy))
                        {
                            pNew.Y = pStart.Y;
                        }
                        else
                        {
                            pNew.X = pStart.X;
                        }
                    }
                    // this.owner.Text = "Panel Dragging : editing point  : " + this.editingPoint.ToString()
                    //     + " new Point " + pNew.ToString();

                    ((LineWidget)widget).SetPoint(this.editingPoint, pNew);
                    pOld = pNew;

                }
                else
                {
                    Rectangle newBounds;

                    int dx = Snap(e.X) - pStart.X;
                    int dy = Snap(e.Y) - pStart.Y;
                    //this.owner.Text = "Panel Dragging : dx : " + dx + "dy :" + dy +
                    //"pStartX : " + pStart.X + " pStartY : " + pStart.Y +
                    //    "e.X : " + e.X + " e.Y : " + e.Y;
                    newBounds = new Rectangle(
                            widgetOldBounds.X + resizeRect.X * dx,
                            widgetOldBounds.Y + resizeRect.Y * dy,
                            widgetOldBounds.Width + resizeRect.Width * dx,
                            widgetOldBounds.Height + resizeRect.Height * dy);

                    int xExcess = MIN_WIDGET_SIZE - newBounds.Width;
                    int yExcess = MIN_WIDGET_SIZE - newBounds.Height;
                    if (xExcess > 0)
                    {
                        newBounds.X = newBounds.X - resizeRect.X * xExcess;
                        newBounds.Width = newBounds.Width -
                            (resizeRect.X == 0 ? -1 : 1) * resizeRect.Width * xExcess;
                    }
                    if (yExcess > 0)
                    {
                        newBounds.Y = newBounds.Y - resizeRect.Y * yExcess;
                        newBounds.Height = newBounds.Height -
                            (resizeRect.Y == 0 ? -1 : 1) * resizeRect.Height * yExcess;
                    }
                    widget.Bounds = newBounds;
                    widget.WidgetData.x = newBounds.X - this.AutoScrollPosition.X;
                    widget.WidgetData.y = newBounds.Y - this.AutoScrollPosition.Y;
                    widget.WidgetData.width = newBounds.Width;
                    widget.WidgetData.height = newBounds.Height;

                    widget.Invalidate();
                }
            }

        }
        protected void Panel_MouseMoved(object sender, MouseEventArgs e)
        {
            if (owner.SelectedButtionId == ECPConfigForm.LINEDRAW ||
               owner.SelectedButtionId == ECPConfigForm.ARROWLINEDRAW)
                this.Cursor = Cursors.Cross;
            else
                this.Cursor = Cursors.Arrow;
            this.CheckResize(sender, e);

        }
        protected void Panel_MousePressed(object sender, MouseEventArgs e)
        {
            if (textEditMode)
            {
                this.Focus();
            }
            if ((!this.isShiftKeyPressed) && (!this.isControlKeyPressed))
            {
                this.ClearSelection();
            }
            if (owner.SelectedButtionId != ECPConfigForm.ARROW)
            {

                if ((owner.SelectedButtionId == ECPConfigForm.LINEDRAW ||
                    owner.SelectedButtionId == ECPConfigForm.ARROWLINEDRAW) && !lineDrawFlag)
                {
                    pOld = Point.Empty;
                    pStart = e.Location;
                    this.lineDrawFlag = true;
                    this.Cursor = Cursors.Cross;
                }
                else if (owner.SelectedButtionId == ECPConfigForm.TEXTWRITE)
                {
                    if (!textEditMode)
                    {
                        // Allow anything but single quote
                        labelTextField = new TextBox();
                        labelTextField.SetBounds(Snap(e.X), Snap(e.Y), TEXTFIELD_WIDTH, TEXTFIELD_HEIGHT);

                        labelTextField.LostFocus += new EventHandler(labelTextField_LostFocus);
                        labelTextField.KeyUp += new KeyEventHandler(labelTextField_KeyUp);


                        this.Controls.Add(labelTextField);

                        this.label = (TextWidget)CreateWidget(new WidgetData(
                            Guid.NewGuid(), owner.GetSelectedButtonWidgetAssemblyName(), owner.GetSelectedButtonWidgetClassName(), "",
                            Snap(e.X) - this.AutoScrollPosition.X, Snap(e.Y) - this.AutoScrollPosition.Y, TEXTFIELD_WIDTH,
                            TEXTFIELD_HEIGHT, new UserDefinedPropertyInfo[0]), true, true);


                        label.AutoSizing = true;
                        label.Visible = false;

                        labelTextField.Visible = true;
                        labelTextField.Focus();
                        textEditMode = true;
                    }

                }
                else if (owner.SelectedButtionId == ECPConfigForm.FIXEDLABEL)
                {
                    int defaultSize = this.defaultWidgetSize;

                    Point p = e.Location;

                    BaseWidget widget = this.CreateWidget(
                        new WidgetData(Guid.NewGuid(),
                        owner.GetSelectedButtonWidgetAssemblyName(),
                        owner.GetSelectedButtonWidgetClassName(),
                        "",
                        p.X - this.AutoScrollPosition.X,
                        p.Y - this.AutoScrollPosition.Y,
                        defaultSize,
                        defaultSize), true, true);

                    widget.Visible = true;
                    widget.WidgetText = "EDIT NAME";
                    widget.WidgetTextAlignment = ContentAlignment.BottomCenter;
                    widget.WidgetBorderStyle = BorderStyle.FixedSingle;
                    widget.WidgetBackColor = Color.Cyan;
                    if (widget != null)
                    {
                        widget.Location = new Point(Snap(p.X), Snap(p.Y));
                        Invalidate(widget.Bounds);
                    }
                    owner.ResetButtons();
                    return;
                }
                else
                {

                    int defaultSize = this.defaultWidgetSize;

                    Point p = e.Location;

                    BaseWidget widget = this.CreateWidget(
                        new WidgetData(Guid.NewGuid(),
                        owner.GetSelectedButtonWidgetAssemblyName(),
                        owner.GetSelectedButtonWidgetClassName(),
                        "",
                        p.X - this.AutoScrollPosition.X,
                        p.Y - this.AutoScrollPosition.Y,
                        defaultSize,
                        defaultSize), true, true);

                    // Now snap it to the right place                    
                    if (widget != null)
                    {
                        //if(widget.ElementId.Equals("")) widget.ElementId = owner.GetUniqueWidgetElemantId(widget.GetHashCode());
                        widget.Location = new Point(Snap(p.X), Snap(p.Y));
                        Invalidate(widget.Bounds);
                    }
                    owner.ResetButtons();
                    //Invalidate();
                    return;
                }
                //owner.ResetButtons();
                // Invalidate();
            }
            else
            {
                if (e.Button == MouseButtons.Right)
                {
                    this.ShowContextMenu(sender, e);
                }
                else
                {
                    pOld = Point.Empty;
                    pStart = e.Location;

                    if (this.resizeActive && this.selection.Count == 1)
                    {
                        BaseWidget widget = this.selection[0];

                        if (widget is LineWidget)
                        {
                            pStart = ((LineWidget)widget).
                                GetPoint(this.editingPoint == LineWidget.POINT_1 ?
                                LineWidget.POINT_2 : LineWidget.POINT_1);
                            pOld = Point.Empty;
                        }
                        else
                        {

                            this.widgetOldBounds = widget.GetConfigBounds();
                        }
                    }
                    else
                    {
                        rubberSelectDrag = true;
                    }
                }
            }
            this.panelPressed = true;
        }
        protected void Panel_MouseReleased(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                this.ShowContextMenu(sender, e);
            }
            else if (lineDrawFlag)
            {
                pNew = e.Location;

                this.Cursor = Cursors.Cross;

                if (this.isShiftKeyPressed)
                {
                    int dx = pNew.X - pStart.X;
                    int dy = pNew.Y - pStart.Y;

                    if (Math.Abs(Math.Abs(dx) - Math.Abs(dy)) < Math.Abs(dx) / 2)
                    {
                        if (dx * dy < 0)
                        {
                            pNew.X = pStart.X - dy;
                        }
                        else
                        {
                            pNew.X = pStart.X + dy;
                        }

                    }
                    else if (Math.Abs(dx) > Math.Abs(dy))
                    {
                        pNew.Y = pStart.Y;
                    }
                    else
                    {
                        pNew.X = pStart.X;
                    }
                }

                if ((Math.Abs(pNew.X - pStart.X) > MIN_WIDGET_SIZE) ||
                    (Math.Abs(pNew.Y - pStart.Y) > MIN_WIDGET_SIZE))
                {

                    // Create Line Widget
                    BaseWidget w = CreateWidget(new WidgetData(Guid.NewGuid(), owner.GetSelectedButtonWidgetAssemblyName(),
                         Owner.GetSelectedButtonWidgetClassName(), "",
                         Snap(pNew.X),
                         Snap(pNew.Y),
                         Snap(pStart.X),
                         Snap(pStart.Y)), true, true);



                    Invalidate(((LineWidget)w).GetLineBounds());

                    owner.ResetButtons();
                }

                lineDrawFlag = false;
                this.Cursor = Cursors.Arrow;
            }
            else if (this.rubberSelectDrag)
            {
                if ((!dragFlag) && (!this.isShiftKeyPressed) && (!this.isControlKeyPressed))
                {
                    this.ClearSelection();
                }
                else
                {
                    int x = Math.Min(e.X, pStart.X);
                    int y = Math.Min(e.Y, pStart.Y);

                    int w = Math.Abs(e.X - pStart.X);
                    int h = Math.Abs(e.Y - pStart.Y);

                    Rectangle rect = new Rectangle(x, y, w, h);

                    if (!this.isShiftKeyPressed && !this.isControlKeyPressed)
                    {
                        this.ClearSelection();
                        this.rubberSelected = true;
                    }

                    for (int i = 0; i < this.Controls.Count; i++)
                    {
                        Control c = this.Controls[i];
                        if (c is BaseWidget)
                        {
                            BaseWidget widget = (BaseWidget)c;

                            Rectangle b;

                            if (widget is LineWidget)
                                b = ((LineWidget)widget).GetLineBounds();
                            else
                                b = widget.GetConfigBounds();

                            x = b.X;
                            y = b.Y;
                            int x2 = b.Width + b.X;
                            int y2 = b.Height + b.Y;

                            if (rect.Contains(x, y) &&
                                rect.Contains(x, y2) &&
                                rect.Contains(x2, y) &&
                                rect.Contains(x2, y2))
                            {
                                //if (!widget.Selected)
                                //{
                                //    SelectWidget(widget, true);
                                //}
                                SelectWidget(widget, !widget.Selected);
                            }


                        }
                    }
                    foreach (BaseWidget widget in this.selection)
                    {
                        widget.Focus();
                        break;
                    }
                    Invalidate(rect);
                }
                pOld = Point.Empty;
            }
            dragFlag = false;
            rubberSelectDrag = false;
            this.panelPressed = false;
            this.widgetPressed = false;
            //Invalidate();
        }
        protected void PaintLine()
        {
            Graphics gra = Graphics.FromHwnd(this.Handle);

            Rubberbandline rbl = new Rubberbandline();

            if(!pNew.Equals(pOld))
            {
                if(pOld != Point.Empty)
                {
                     rbl.XorLine(gra,pStart.X, pStart.Y,pOld.X,pOld.Y);
                }

                rbl.XorLine(gra, pStart.X, pStart.Y,pNew.X, pNew.Y);
            }
        }

        protected void PaintRect()
        {           
            Graphics gra = Graphics.FromHwnd(this.Handle);

            RubberbandRectangle rbr = new RubberbandRectangle();

            if (!pNew.Equals(pOld))
            {         

                // Erase the old rect and draw the new one
                if (pOld != Point.Empty)
                {
                    rbr.DrawXORRectangle(gra, pStart.X, pStart.Y, pOld.X, pOld.Y);                 

                }

                rbr.DrawXORRectangle(gra,pStart.X, pStart.Y, pNew.X, pNew.Y);                
            }
        }

        private void ClearSelection()
        {
            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget w = (BaseWidget)selection[i];
                w.Selected = false;
                Invalidate(w.Bounds);
            }

            selection.Clear();
            rubberSelected = false;
            //Invalidate();
        }

        protected void Widget_MouseDrag(object sender, MouseEventArgs e)
        {
            this.SuspendLayout();

            BaseWidget widget;
            if (selection.Count == 1 && resizeActive == true)
            {
                //// They are resizing the widgets.
                widget = this.selection[0];
                Rectangle b = widget.GetConfigBounds();

                if (widget is LineWidget)
                {
                                
                }
                else
                {
                    Rectangle newBounds;

                    int dx = Snap(b.X + e.X) - pStart.X;
                    int dy = Snap(b.Y + e.Y) - pStart.Y;

                    ////this.owner.Text = "Wiget Dragging : dx : " + dx + "dy :" + dy +
                    ////"pStartX : " + pStart.X + " pStartY : " + pStart.Y +
                    ////    "e.X : " + e.X + " e.Y : " + e.Y;

                    newBounds = new Rectangle(
                            widgetOldBounds.X + resizeRect.X * dx,
                            widgetOldBounds.Y + resizeRect.Y * dy,
                            widgetOldBounds.Width + resizeRect.Width * dx,
                            widgetOldBounds.Height + resizeRect.Height * dy);

                    int xExcess = MIN_WIDGET_SIZE - newBounds.Width;
                    int yExcess = MIN_WIDGET_SIZE - newBounds.Height;
                    if (xExcess > 0)
                    {
                        newBounds.X = newBounds.X - resizeRect.X * xExcess;
                        newBounds.Width = newBounds.Width -
                            (resizeRect.X == 0 ? -1 : 1) * resizeRect.Width * xExcess;
                    }
                    if (yExcess > 0)
                    {
                        newBounds.Y = newBounds.Y - resizeRect.Y * yExcess;
                        newBounds.Height = newBounds.Height -
                            (resizeRect.Y == 0 ? -1 : 1) * resizeRect.Height * yExcess;
                    }
                    widget.Bounds = newBounds;
                    widget.WidgetData.x = newBounds.X -this.AutoScrollPosition.X;
                    widget.WidgetData.y = newBounds.Y - this.AutoScrollPosition.Y;
                    widget.WidgetData.width = newBounds.Width;
                    widget.WidgetData.height = newBounds.Height;

                    widget.Invalidate();
                }

                
            }
            else 
            {
                // They are moving the widgets.

                BaseWidget masterWidget = (BaseWidget)sender;
                Rectangle b = masterWidget.Bounds;

                pNew.X = e.X;
                pNew.Y = e.Y;

                int dx = Snap(pNew.X) - pStart.X;
                int dy = Snap(pNew.Y) - pStart.Y;

                // Keep the widgets from going off the top left of the panel

                for (int i = 0; i < selection.Count; i++)
                {
                    widget = (BaseWidget)selection[i];
                    b = widget.Bounds;

                    int nextX = b.X + dx;
                    int nextY = b.Y + dy;

                    if (nextX < 0)
                    {
                        dx -= nextX;
                    }
                    if (nextY < 0)
                    {
                        dy -= nextY;
                    }
                }
                for (int i = 0; i < selection.Count; i++)
                {
                    widget = (BaseWidget)selection[i];
                    b = widget.Bounds;
                    int nextX = Snap(b.X) + dx;
                    int nextY = Snap(b.Y) + dy;
                  
                    widget.SetBounds(nextX, nextY, widget.Width, widget.Height);
                    
                    if (widget is LineWidget)
                    {
                        widget.WidgetData.x += dx;
                        widget.WidgetData.y += dy;
                        widget.WidgetData.width += dx;
                        widget.WidgetData.height += dy;
                    }
                    else
                    {
                        widget.WidgetData.x = nextX - this.AutoScrollPosition.X;
                        widget.WidgetData.y = nextY - this.AutoScrollPosition.Y;
                    }
                }
                this.Update();
            }
            dragFlag = true;
            this.ResumeLayout(true);
        }
        protected void Widget_MousePress(object sender, MouseEventArgs e)
        {
            if (textEditMode)
            {
                this.Focus();
            }

            BaseWidget widget = (BaseWidget)sender;

            if (e.Button == MouseButtons.Right)
            {
                //      this.ClearSelection();
                //      this.SelectWidget(widget, true);
                this.ShowContextMenu(sender, e);
            }
            else if (e.Clicks > 1 &&
                    !isControlKeyPressed && !isShiftKeyPressed)
            {
                this.ClearSelection();
                this.SelectWidget(widget, true);

                this.SelEditItem_Action();
                // show the editor
            }
            else
            {
                if (this.resizeActive && selection.Count == 1)
                {

                    pStart.X = Snap(widget.Location.X + e.X);
                    pStart.Y = Snap(widget.Location.Y + e.Y);
                    this.widgetPressed = true;
                    this.widgetOldBounds = widget.GetConfigBounds();
                }
                else
                {
                    pStart.X = Snap(e.X);
                    pStart.Y = Snap(e.Y);
                    this.widgetPressed = true;
                    //this.widgetOldBounds = widget.GetConfigBounds();
                }
            }
        }
        protected void Widget_MouseReleased(object sender, MouseEventArgs e)
        {

            Object obj = sender;
            BaseWidget widget = (BaseWidget)obj;
            if (!dragFlag)
            {
                if (e.Button == MouseButtons.Right)
                {
                    this.ShowContextMenu(sender, e);
                }
                else if (this.isShiftKeyPressed || this.isControlKeyPressed)
                {
                    this.SelectWidget(widget, !widget.Selected);
                }
                else
                {
                    ClearSelection();
                    SelectWidget(widget, true);
                }
            }
            else
            {
                //widget 위치 검사
                int validX, validY;
                validX = widget.Location.X;
                validY = widget.Location.Y;

                if (!this.ClientRectangle.Contains(widget.Location))
                {
                    if (widget.Location.X < this.AutoScrollPosition.X)
                    {
                        validX = this.AutoScrollPosition.X;
                    }
                    if (widget.Location.Y < this.AutoScrollPosition.Y)
                    {
                        validY = this.AutoScrollPosition.Y;
                    }
                    widget.SetBounds(validX, validY, widget.Width, widget.Height);
                }
            }

            this.dragFlag = false;
            this.widgetPressed = false;

        }

        private void SelectWidget(BaseWidget widget, bool selected)
        {
            widget.Selected = selected;
            if (selected)
            {
                if (!selection.Contains(widget))
                {
                    selection.Add(widget);
                }               
            }
            else
            {
                selection.Remove(widget);
                //080930 김성원 widget 해제 시 선택한 widget의 focus를 유지 하기위해 사용
                if (selection.Count >= 1)
                {
                    foreach (BaseWidget checkWidget in selection)
                    {
                        if (checkWidget == null) continue;
                        if (checkWidget.Bounds.X > this.Bounds.X && checkWidget.Bounds.Y > this.Bounds.Y)
                        {
                            checkWidget.Focus();
                            break;
                        }
                    }
                }
            }           

        }

        void labelTextField_KeyUp(object sender, KeyEventArgs e)
        {            
            if (e.KeyData == Keys.Return)
            {
                textEditMode = false;

                this.labelTextField_focusLost();

            }            
        }

        void labelTextField_LostFocus(object sender, EventArgs e)
        {

            if (textEditMode)
            {
                this.textEditMode = false;

                this.labelTextField_focusLost();

            }
          
        }

        private void labelTextField_focusLost()
        {
            string text = labelTextField.Text;

            if ((text != string.Empty) 
                //&& (!text.Trim().Equals(""))
                )
            {
                labelTextField.Visible = false;

                labelTextField.Invalidate();

                label.WidgetText = text;
                label.Visible = true;

      
                label.SetBounds(labelTextField.Location.X, labelTextField.Location.Y, label.Width, label.Height);
                label.WidgetData.x = labelTextField.Location.X-this.AutoScrollPosition.X;
                label.WidgetData.y = labelTextField.Location.Y-this.AutoScrollPosition.Y;
                label.WidgetData.width = label.Width;
                label.WidgetData.height = label.Height;
                label.WidgetText = text;


            }
            else
            {

                this.Controls.Remove(labelTextField);
                this.Controls.Remove(label);
            }

            owner.ResetButtons();

            label.Invalidate();
            labelTextField.Invalidate();
            //Invalidate();
        }

        private void CheckResize(object sender, MouseEventArgs e)
        {
            int resizeTolerance = RESIZE_TOLERANCE;
            int resizeCorrection = 0; //in case of FixedSingle or Fixed3D 
            this.resizeRect = new Rectangle(0, 0, 0, 0);
            if (sender is BaseWidget)
            {
                BaseWidget widget = (BaseWidget)sender;
                if (widget.Selected == false) return;
            }

            if (this.selection.Count == 1)
            {
                BaseWidget widget = (BaseWidget)selection[0];

                if (widget != null)
                {
                    if (widget.BorderStyle == BorderStyle.FixedSingle)
                    {
                        resizeCorrection = 3;
                    }
                    else if (widget.BorderStyle == BorderStyle.Fixed3D)
                    {
                        resizeCorrection = 4;
                    }
                }

                if (widget is LineWidget)
                {
                    int ex = e.X;
                    int ey = e.Y;

                    this.resizeActive = false;

                    resizeTolerance = RESIZE_TOLERANCE + 3;

                    LineWidget line = (LineWidget)widget;

                    Point p1 = line.GetPoint(LineWidget.POINT_1);
                    Point p2 = line.GetPoint(LineWidget.POINT_2);
                    Rectangle rect = line.Bounds;
                    ex += rect.X;
                    ey += rect.Y;
                    int innerWidth, innerHeight;
                    if (rect.Width > rect.Height)
                    {
                        innerWidth = RESIZE_TOLERANCE - resizeCorrection;
                        innerHeight = rect.Height;
                        this.Cursor = Cursors.SizeWE;
                    }
                    else
                    {
                        innerWidth = rect.Height;
                        innerHeight = RESIZE_TOLERANCE - resizeCorrection;
                        this.Cursor = Cursors.SizeNS;
                    }

                    if (rect.Contains(new Point(ex, ey)))
                    
                    {
                        Console.WriteLine(ex + ", " + ey);
                        Rectangle innerRect;
                        innerRect = new Rectangle(rect.X, rect.Y
                            , innerWidth, innerHeight);

                        if (innerRect.Contains(new Point(ex, ey)))
                        {
                            this.resizeActive = true;
                        }

                        innerRect = new Rectangle(rect.X + rect.Width - innerWidth, rect.Y + rect.Height - innerHeight
                            , innerWidth, innerHeight);

                        if (innerRect.Contains(new Point(ex, ey)))
                        {
                            this.resizeActive = true;
                        }
                    }
                    if (!resizeActive)
                    {
                        this.Cursor = Cursors.Arrow;
                    }
                }
                else if (sender is BaseWidget)
                {
                    Rectangle bounds = widget.GetConfigBounds();

                    Point ep = e.Location;

                    ep.X = bounds.X + ep.X;
                    ep.Y = bounds.Y + ep.Y;

                    // Calculate the distances to the sides
                    int dx = Math.Abs(ep.X - bounds.X);
                    int dy = Math.Abs(ep.Y - bounds.Y);
                    int x2 = bounds.X + bounds.Width;//  + resizeCorrection;
                    int y2 = bounds.Y + bounds.Height;// + resizeCorrection;
                    int dx2 = Math.Abs(ep.X - x2);
                    int dy2 = Math.Abs(ep.Y - y2);

                    if (dx < resizeTolerance)
                    {
                        if (dy < resizeTolerance)
                        {
                            resizeActive = true;
                            resizeRect.X = resizeRect.Y = 1;
                            resizeRect.Height = resizeRect.Width = -1;

                            this.Cursor = Cursors.SizeNWSE;
                        }
                        else if (dy2 < resizeTolerance + resizeCorrection)
                        {
                            resizeActive = true;
                            resizeRect.X = resizeRect.Height = 1;
                            resizeRect.Width = -1;
                            this.Cursor = Cursors.SizeNESW;
                        }
                        else if ((ep.Y > bounds.Y) && (ep.Y < y2))
                        {
                            resizeActive = true;
                            resizeRect.X = 1;
                            resizeRect.Width = -1;
                            this.Cursor = Cursors.SizeWE;
                        }
                    }

                    // Check if it's by the right side

                    else if (dx2 < resizeTolerance + resizeCorrection)
                    {
                        if (dy < resizeTolerance + resizeCorrection)
                        {
                            resizeActive = true;
                            resizeRect.Y = resizeRect.Width = 1;
                            resizeRect.Height = -1;

                            this.Cursor = Cursors.SizeNESW;
                        }
                        else if (dy2 < resizeTolerance + resizeCorrection)
                        {
                            resizeActive = true;
                            resizeRect.Width = resizeRect.Height = 1;

                            this.Cursor = Cursors.SizeNWSE;
                        }
                        else if ((ep.Y > bounds.Y) && (ep.Y < y2))
                        {
                            resizeActive = true;
                            resizeRect.Width = 1;
                            this.Cursor = Cursors.SizeWE;
                        }
                    }

                    // Check if it's by the top

                    else if ((dy < resizeTolerance) &&
                    (ep.X > bounds.X) && (ep.X < x2))
                    {
                        resizeActive = true;
                        resizeRect.Y = 1;
                        resizeRect.Height = -1;

                        this.Cursor = Cursors.SizeNS;
                    }

                    // Check if it's by the bottom

                    else if ((dy2 < resizeTolerance + resizeCorrection) &&
                             (ep.X > bounds.X) && (ep.X < x2))
                    {
                        resizeActive = true;
                        resizeRect.Height = 1;

                        this.Cursor = Cursors.SizeNS;
                    }

                    // Show the arrow...

                    else
                    {
                        Console.WriteLine("arrow arrow arrow arrow arrow arrow ");
                        resizeActive = false;
                        this.Cursor = Cursors.Arrow;
                    }
                }
                else
                {
                    resizeActive = false;
                }
            }
            else
            {
                resizeActive = false;
            }
        }

        private int Snap(int value)
        {
            // Snap the int to the grid
            if (snappingEnabled && !snappingToLine)
            {
                int gridSize = this.GridSize;
                // Should not offset more than gridsize, just the difference.
                
                int moded = value % gridSize;

                if (moded < (gridSize / 2))
                {
                    return (value - moded);
                }
                else
                {
                    return (value + gridSize - moded);
                }
            }
        
            return value;
        }            

        protected void ShowContextMenu(object sender, MouseEventArgs e)
        {
            Object obj = sender;
            if (obj is BaseWidget)
            {
                BaseWidget widget = (BaseWidget)obj;
                if (!this.selection.Contains(widget))
                {
                    this.ClearSelection();
                    this.SelectWidget(widget, true);
                }
                this.ContextMenuStrip = this.selectMenu;
            }
            else
            {
                this.ContextMenuStrip = this.globalMenu;
            }
        }

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.globalMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.gloCutItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gloCopyItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gloCopyPropertyItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gloPasteItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gloPasteUnconfigured = new System.Windows.Forms.ToolStripMenuItem();
            this.gloPastePropertyItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gloDeleteItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gloSelectAll = new System.Windows.Forms.ToolStripMenuItem();
            this.globSearch = new System.Windows.Forms.ToolStripMenuItem();
            this.gloSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.gloShowGridItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gloSnappingItem = new System.Windows.Forms.ToolStripMenuItem();
            this.gloSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.gloGridSize = new System.Windows.Forms.ToolStripMenuItem();
            this.selectMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.selEditItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selPropertyItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selUDPropertyItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selCutItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selCopyItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selCopyPropertyItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selPasteItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selPasteUnconfiguredItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selPastePropertyItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selDeleteItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selSelectAllItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.selAlignLeftItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selAlignRightItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selAlignBottomItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selAlignTopItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selCenterVItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selCenterHItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selSnapToGridItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selSameSizeItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selSameDistanceItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selEditSizeItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.selMoveFrontItem = new System.Windows.Forms.ToolStripMenuItem();
            this.selMoveBackItem = new System.Windows.Forms.ToolStripMenuItem();
            this.imageChooser = new System.Windows.Forms.OpenFileDialog();
            this.gloUndo = new System.Windows.Forms.ToolStripMenuItem();
            this.gloRedo = new System.Windows.Forms.ToolStripMenuItem();
            this.globalMenu.SuspendLayout();
            this.selectMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // globalMenu
            // 
            this.globalMenu.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.globalMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.gloCutItem,
            this.gloCopyItem,
            this.gloCopyPropertyItem,
            this.gloPasteItem,
            this.gloPasteUnconfigured,
            this.gloPastePropertyItem,
            this.gloDeleteItem,
            this.gloSelectAll,
            this.globSearch,
            this.gloSeparator1,
            this.gloShowGridItem,
            this.gloSnappingItem,
            this.gloSeparator2,
            this.gloGridSize,
            this.gloUndo,
            this.gloRedo});
            this.globalMenu.Name = "globalMenu";
            this.globalMenu.Size = new System.Drawing.Size(228, 324);
            // 
            // gloCutItem
            // 
            this.gloCutItem.Name = "gloCutItem";
            this.gloCutItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.gloCutItem.Size = new System.Drawing.Size(227, 22);
            this.gloCutItem.Text = "Cut Selection";
            this.gloCutItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloCopyItem
            // 
            this.gloCopyItem.Name = "gloCopyItem";
            this.gloCopyItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.gloCopyItem.Size = new System.Drawing.Size(227, 22);
            this.gloCopyItem.Text = "Copy Selection";
            this.gloCopyItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloCopyPropertyItem
            // 
            this.gloCopyPropertyItem.Name = "gloCopyPropertyItem";
            this.gloCopyPropertyItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.C)));
            this.gloCopyPropertyItem.Size = new System.Drawing.Size(227, 22);
            this.gloCopyPropertyItem.Text = "Copy Property";
            this.gloCopyPropertyItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloPasteItem
            // 
            this.gloPasteItem.Name = "gloPasteItem";
            this.gloPasteItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.gloPasteItem.Size = new System.Drawing.Size(227, 22);
            this.gloPasteItem.Text = "Paste";
            this.gloPasteItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloPasteUnconfigured
            // 
            this.gloPasteUnconfigured.Name = "gloPasteUnconfigured";
            this.gloPasteUnconfigured.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.gloPasteUnconfigured.Size = new System.Drawing.Size(227, 22);
            this.gloPasteUnconfigured.Text = "Paste Unconfigured";
            this.gloPasteUnconfigured.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloPastePropertyItem
            // 
            this.gloPastePropertyItem.Name = "gloPastePropertyItem";
            this.gloPastePropertyItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.V)));
            this.gloPastePropertyItem.Size = new System.Drawing.Size(227, 22);
            this.gloPastePropertyItem.Text = "Paste Property";
            this.gloPastePropertyItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloDeleteItem
            // 
            this.gloDeleteItem.Name = "gloDeleteItem";
            this.gloDeleteItem.ShortcutKeys = System.Windows.Forms.Keys.Delete;
            this.gloDeleteItem.Size = new System.Drawing.Size(227, 22);
            this.gloDeleteItem.Text = "Delete";
            this.gloDeleteItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloSelectAll
            // 
            this.gloSelectAll.Name = "gloSelectAll";
            this.gloSelectAll.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.gloSelectAll.Size = new System.Drawing.Size(227, 22);
            this.gloSelectAll.Text = "Select All";
            this.gloSelectAll.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // globSearch
            // 
            this.globSearch.Name = "globSearch";
            this.globSearch.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.F)));
            this.globSearch.Size = new System.Drawing.Size(227, 22);
            this.globSearch.Text = "Search";
            this.globSearch.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloSeparator1
            // 
            this.gloSeparator1.Name = "gloSeparator1";
            this.gloSeparator1.Size = new System.Drawing.Size(224, 6);
            // 
            // gloShowGridItem
            // 
            this.gloShowGridItem.Name = "gloShowGridItem";
            this.gloShowGridItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.G)));
            this.gloShowGridItem.Size = new System.Drawing.Size(227, 22);
            this.gloShowGridItem.Text = "Hide Grid";
            this.gloShowGridItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloSnappingItem
            // 
            this.gloSnappingItem.Name = "gloSnappingItem";
            this.gloSnappingItem.Size = new System.Drawing.Size(227, 22);
            this.gloSnappingItem.Text = "Enable Snapping";
            this.gloSnappingItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // gloSeparator2
            // 
            this.gloSeparator2.Name = "gloSeparator2";
            this.gloSeparator2.Size = new System.Drawing.Size(224, 6);
            // 
            // gloGridSize
            // 
            this.gloGridSize.Name = "gloGridSize";
            this.gloGridSize.Size = new System.Drawing.Size(227, 22);
            this.gloGridSize.Text = "Grid Size";
            this.gloGridSize.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selectMenu
            // 
            this.selectMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.selEditItem,
            this.selPropertyItem,
            this.selUDPropertyItem,
            this.selCutItem,
            this.selCopyItem,
            this.selCopyPropertyItem,
            this.selPasteItem,
            this.selPasteUnconfiguredItem,
            this.selPastePropertyItem,
            this.selDeleteItem,
            this.selSelectAllItem,
            this.toolStripSeparator1,
            this.selAlignLeftItem,
            this.selAlignRightItem,
            this.selAlignBottomItem,
            this.selAlignTopItem,
            this.selCenterVItem,
            this.selCenterHItem,
            this.selSnapToGridItem,
            this.selSameSizeItem,
            this.selSameDistanceItem,
            this.selEditSizeItem,
            this.selSeparator,
            this.selMoveFrontItem,
            this.selMoveBackItem});
            this.selectMenu.Name = "selectMenu";
            this.selectMenu.Size = new System.Drawing.Size(228, 522);
            // 
            // selEditItem
            // 
            this.selEditItem.Name = "selEditItem";
            this.selEditItem.Size = new System.Drawing.Size(227, 22);
            this.selEditItem.Text = "Edit";
            this.selEditItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selPropertyItem
            // 
            this.selPropertyItem.Name = "selPropertyItem";
            this.selPropertyItem.Size = new System.Drawing.Size(227, 22);
            this.selPropertyItem.Text = "Properties";
            this.selPropertyItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selUDPropertyItem
            // 
            this.selUDPropertyItem.Name = "selUDPropertyItem";
            this.selUDPropertyItem.Size = new System.Drawing.Size(227, 22);
            this.selUDPropertyItem.Text = "UDProperties";
            this.selUDPropertyItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selCutItem
            // 
            this.selCutItem.Name = "selCutItem";
            this.selCutItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.selCutItem.Size = new System.Drawing.Size(227, 22);
            this.selCutItem.Text = "Cut Selection";
            this.selCutItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selCopyItem
            // 
            this.selCopyItem.Name = "selCopyItem";
            this.selCopyItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.selCopyItem.Size = new System.Drawing.Size(227, 22);
            this.selCopyItem.Text = "Copy Selection";
            this.selCopyItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selCopyPropertyItem
            // 
            this.selCopyPropertyItem.Name = "selCopyPropertyItem";
            this.selCopyPropertyItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.C)));
            this.selCopyPropertyItem.Size = new System.Drawing.Size(227, 22);
            this.selCopyPropertyItem.Text = "Copy Property";
            this.selCopyPropertyItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selPasteItem
            // 
            this.selPasteItem.Name = "selPasteItem";
            this.selPasteItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.selPasteItem.Size = new System.Drawing.Size(227, 22);
            this.selPasteItem.Text = "Paste";
            this.selPasteItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selPasteUnconfiguredItem
            // 
            this.selPasteUnconfiguredItem.Name = "selPasteUnconfiguredItem";
            this.selPasteUnconfiguredItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.selPasteUnconfiguredItem.Size = new System.Drawing.Size(227, 22);
            this.selPasteUnconfiguredItem.Text = "Paste Unconfigured";
            this.selPasteUnconfiguredItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selPastePropertyItem
            // 
            this.selPastePropertyItem.Name = "selPastePropertyItem";
            this.selPastePropertyItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift)
                        | System.Windows.Forms.Keys.V)));
            this.selPastePropertyItem.Size = new System.Drawing.Size(227, 22);
            this.selPastePropertyItem.Text = "Paste Property";
            this.selPastePropertyItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selDeleteItem
            // 
            this.selDeleteItem.Name = "selDeleteItem";
            this.selDeleteItem.ShortcutKeys = System.Windows.Forms.Keys.Delete;
            this.selDeleteItem.Size = new System.Drawing.Size(227, 22);
            this.selDeleteItem.Text = "Delete";
            this.selDeleteItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selSelectAllItem
            // 
            this.selSelectAllItem.Name = "selSelectAllItem";
            this.selSelectAllItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.A)));
            this.selSelectAllItem.Size = new System.Drawing.Size(227, 22);
            this.selSelectAllItem.Text = "Select All";
            this.selSelectAllItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(224, 6);
            // 
            // selAlignLeftItem
            // 
            this.selAlignLeftItem.Name = "selAlignLeftItem";
            this.selAlignLeftItem.Size = new System.Drawing.Size(227, 22);
            this.selAlignLeftItem.Text = "Align Left";
            this.selAlignLeftItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selAlignRightItem
            // 
            this.selAlignRightItem.Name = "selAlignRightItem";
            this.selAlignRightItem.Size = new System.Drawing.Size(227, 22);
            this.selAlignRightItem.Text = "Align Right";
            this.selAlignRightItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selAlignBottomItem
            // 
            this.selAlignBottomItem.Name = "selAlignBottomItem";
            this.selAlignBottomItem.Size = new System.Drawing.Size(227, 22);
            this.selAlignBottomItem.Text = "Align Bottom";
            this.selAlignBottomItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selAlignTopItem
            // 
            this.selAlignTopItem.Name = "selAlignTopItem";
            this.selAlignTopItem.Size = new System.Drawing.Size(227, 22);
            this.selAlignTopItem.Text = "Align Top";
            this.selAlignTopItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selCenterVItem
            // 
            this.selCenterVItem.Name = "selCenterVItem";
            this.selCenterVItem.Size = new System.Drawing.Size(227, 22);
            this.selCenterVItem.Text = "Center Vertical";
            this.selCenterVItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selCenterHItem
            // 
            this.selCenterHItem.Name = "selCenterHItem";
            this.selCenterHItem.Size = new System.Drawing.Size(227, 22);
            this.selCenterHItem.Text = "Center Horizontal";
            this.selCenterHItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selSnapToGridItem
            // 
            this.selSnapToGridItem.Name = "selSnapToGridItem";
            this.selSnapToGridItem.Size = new System.Drawing.Size(227, 22);
            this.selSnapToGridItem.Text = "Snap To Grid";
            this.selSnapToGridItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selSameSizeItem
            // 
            this.selSameSizeItem.Name = "selSameSizeItem";
            this.selSameSizeItem.Size = new System.Drawing.Size(227, 22);
            this.selSameSizeItem.Text = "Same Size";
            this.selSameSizeItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selSameDistanceItem
            // 
            this.selSameDistanceItem.Name = "selSameDistanceItem";
            this.selSameDistanceItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.D1)));
            this.selSameDistanceItem.Size = new System.Drawing.Size(227, 22);
            this.selSameDistanceItem.Text = "Same Distance";
            this.selSameDistanceItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selEditSizeItem
            // 
            this.selEditSizeItem.Name = "selEditSizeItem";
            this.selEditSizeItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.selEditSizeItem.Size = new System.Drawing.Size(227, 22);
            this.selEditSizeItem.Text = "Edit Size";
            this.selEditSizeItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selSeparator
            // 
            this.selSeparator.Name = "selSeparator";
            this.selSeparator.Size = new System.Drawing.Size(224, 6);
            // 
            // selMoveFrontItem
            // 
            this.selMoveFrontItem.Name = "selMoveFrontItem";
            this.selMoveFrontItem.Size = new System.Drawing.Size(227, 22);
            this.selMoveFrontItem.Text = "Move To Front";
            this.selMoveFrontItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // selMoveBackItem
            // 
            this.selMoveBackItem.Name = "selMoveBackItem";
            this.selMoveBackItem.Size = new System.Drawing.Size(227, 22);
            this.selMoveBackItem.Text = "Move To Back";
            this.selMoveBackItem.Click += new System.EventHandler(this.PopUpActionHandler);
            // 
            // imageChooser
            // 
            this.imageChooser.DefaultExt = "bmp";
            this.imageChooser.Filter = "Bitmap 파일 (*.bmp)|*.bmp|Jpeg 파일 (Jpeg.*)|*.jpg|모든 파일(*.*)|*.*";
            // 
            // gloUndo
            // 
            this.gloUndo.Name = "gloUndo";
            this.gloUndo.Size = new System.Drawing.Size(227, 22);
            this.gloUndo.Text = "Undo";
            // 
            // gloRedo
            // 
            this.gloRedo.Name = "gloRedo";
            this.gloRedo.Size = new System.Drawing.Size(227, 22);
            this.gloRedo.Text = "Redo";
            // 
            // ECPConfigPanel
            // 
            this.AutoScroll = true;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.ContextMenuStrip = this.globalMenu;
            this.MouseLeave += new System.EventHandler(this.MouseExitHandler);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.MouseMoveHandler);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.MousePressedHandler);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.MouseReleasedHandler);
            this.globalMenu.ResumeLayout(false);
            this.selectMenu.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        public void ToolButtonActionHander()
        {
            if (owner.SelectedButtionId == ECPConfigForm.SAMEDISTANCE)
            {
                SelSameDistanceItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.ALIGNTOP)
            {
                SelAlignTopItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.ALIGNBOTTOM)
            {
                SelAlignBottomItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.ALIGNLEFT)
            {
                SelAlignLeftItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.ALIGNRIGHT)
            {
                SelAlignRightItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.CENTERHORIZONTAL)
            {
                SelCenterHorizontalItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.CENTERVERTICAL)
            {
                SelCenterVerticalItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.BRINGTOFRONT)
            {
                SelMoveFrontItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.SENTTOBACK)
            {
                SelMoveBackItem_Action();
            }
            else if (owner.SelectedButtionId == ECPConfigForm.WIDGETLISTVIEWER)
            {
                if (widgetListForm == null)
                {
                    widgetListForm = new WidgetListForm(this);
                    widgetListForm.Show();
                    widgetListForm.FormClosed += new FormClosedEventHandler(widgetListForm_FormClosed);
                }
                else
                {
                    widgetListForm.Close();
                    widgetListForm = null;
                }
            }

            owner.ResetButtons();
        }

        private void widgetListForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.widgetListForm = null;
        }

        private void PopUpActionHandler(object sender, EventArgs e)
        {
            ToolStripMenuItem menu = sender as ToolStripMenuItem;
            if (menu == null) return;

            string command = menu.Text;

            
            if (command == DELETE)
            {
                this.SelDeleteItem_Action();
            }
            else if (command == CUT_SELECTION)
            {
                this.SelCutItem_Action();
            }
            else if (command == COPY_SELECTION)
            {
                this.SelCopyItem_Action();
            }
            else if (command == COPY_PROPERTY)
            {
                this.SelCopyPropertyItem_Action();
            }
            else if (command == PASTE)
            {
                this.SelPasteItem_Action(false);
            }
            else if (command == PASTE_UNCONFIGURED)
            {
                this.SelPasteItem_Action(true);
            }
            else if (command == PASTE_PROPERTY)
            {
                this.SelPastePropertyItem_Action();
            }
            else if (command == SELECT_ALL)
            {
                this.PerformSelectAllAction();
            }
            else if (command == SHOW_GRID)
            {
                this.GloShowGrid_Action();
            }
            else if (command == HIDE_GRID)
            {
                this.GloHideGrid_Action();
            }
            else if (command == ENABLE_SNAPPING)
            {
                this.GloSnapOn_Action();
            }
            else if (command == ALIGN_LEFT)
            {
                SelAlignLeftItem_Action();
            }
            else if (command == ALIGN_RIGHT)
            {
                SelAlignRightItem_Action();
            }
            else if (command == ALIGN_TOP)
            {
                this.SelAlignTopItem_Action();
            }
            else if (command == ALIGN_BOTTOM)
            {
                SelAlignBottomItem_Action();
            }
            else if (command == CENTER_VERTICAL)
            {
                this.SelCenterVerticalItem_Action();
            }
            else if (command == CENTER_HORIZONTAL)
            {
                this.SelCenterHorizontalItem_Action();
            }
            else if (command == SNAP_TO_GRID)
            {
                MessageBox.Show(SNAP_TO_GRID);
            }
            else if (command == SAME_SIZE)
            {
                SelSameSizeItem_Action();
            }
            else if (command == SAME_DISTANCE)
            {
                SelSameDistanceItem_Action();
            }
            else if (command == EDIT_SIZE)
            {
                // TODO
                SelEditSizeItem_Action();
            }
            else if (command == MOVE_TO_FRONT)
            {
                SelMoveFrontItem_Action();
            }
            else if (command == MOVE_TO_BACK)
            {
                SelMoveBackItem_Action();
            }
            else if (command == GROUP)
            {
                MessageBox.Show(GROUP);
            }
            else if (command == UNGROUP)
            {
                MessageBox.Show(UNGROUP);
            }
            else if (command == EDIT)
            {
                SelEditItem_Action();
            }
            else if (command == WIDGET_PROPERTIES)
            {
                SelPropertyItem_Action();
            }
            else if (command == WIDGET_UDPROPERTIES)
            {
                SelUDPropertyItem_Action();
            }
            else if (command == DISABLE_SNAPPING)
            {
                this.GloSnapOff_Action();
            }
            else if (command == SEARCH)
            {
                //MessageBox.Show(SEARCH);
                this.GloSearch_Action();
            }
            else if (command == SWITCH_DIRECTION)
            {
                MessageBox.Show(SWITCH_DIRECTION);
            }
            else if (command == TRACK_CHECK)
            {
                MessageBox.Show(TRACK_CHECK);
            }
            else if (command == ADJUST_DISTANCES)
            {
                MessageBox.Show(ADJUST_DISTANCES);
            }
            else if (command == ADJUST_SPEEDS)
            {
                MessageBox.Show(ADJUST_SPEEDS);
            }
            else if (command == GRID_SIZE)
            {
                new Dialogs.GridSizeDialog(this).Show();
            }  
        }        

        protected override void OnPaint(PaintEventArgs e)
        {

            if (this.showGrid)
            {
                Graphics g = e.Graphics;
                int w = this.Width;
                int h = this.Height;
                int gridSize = this.GridSize;

                int j = 1;

                for (int i = gridSize; i < w; i += gridSize)
                {
                    int moded = j % 10;

                    if (moded == 0)
                    {
                        g.DrawLine(new Pen(Color.FromArgb(210, 210, 220),2), i, 0, i, h);
                    }
                    else
                    {
                        g.DrawLine(new Pen(Color.FromArgb(210, 210, 220), 1), i, 0, i, h);
                    }
                    j++;
                }

                j = 1;

                for (int i = gridSize; i < h; i += gridSize)
                {
                    int moded = j % 10;

                    if (moded == 0)
                    {
                        g.DrawLine(new Pen(Color.FromArgb(210, 210, 220), 2), 0, i, w, i);
                    }
                    else
                    {
                        g.DrawLine(new Pen(Color.FromArgb(210, 210, 220), 1), 0, i, w, i);
                    }
                    j++;
                }
            }

            
            
        }

        void SelAlignTopItem_Action()
        {
            if (selection.Count < 1) return;            

            int y = Int32.MaxValue;

            if (rubberSelected)
            {
                for (int i = 0; i < selection.Count; i++)
                {
                    BaseWidget temp = (BaseWidget)selection[i];

                    if (temp.Location.Y < y)
                    {
                        y = temp.Location.Y;
                    }
                }
            }
            else
            {
                y = ((BaseWidget)(selection[0])).Location.Y;
            }
            // Align them all to y

            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget temp = (BaseWidget)selection[i];

                Rectangle b = temp.GetConfigBounds();
                b.Y = y;
                temp.SetBounds(b.X,b.Y,b.Width,b.Height);

                temp.WidgetData.x = b.X - this.AutoScrollPosition.X;
                temp.WidgetData.y = b.Y - this.AutoScrollPosition.Y;

                temp.Invalidate();
            }
            //Invalidate();
        }

        void SelAlignLeftItem_Action()
        {
            if (selection.Count < 1) return;        

            int x = Int32.MaxValue;
   
            if (rubberSelected)
            {
                for (int i = 0; i < selection.Count; i++)
                {
                    BaseWidget temp = (BaseWidget)selection[i];
                    Rectangle b = temp.GetConfigBounds();
                    if (b.X < x)
                    {
                        x = b.X;
                    }
                }
            }
            else
            {
                x = ((BaseWidget)(selection[0])).GetConfigBounds().X;
            }

            // Align them all to x

            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget temp = (BaseWidget)selection[i];

                Rectangle b = temp.GetConfigBounds();
                b.X = x;
                temp.SetBounds(b.X, b.Y, b.Width, b.Height);

                temp.WidgetData.x = b.X - this.AutoScrollPosition.X;
                temp.WidgetData.y = b.Y - this.AutoScrollPosition.Y;

                temp.Invalidate();
            }
            //Invalidate();
        }

        void SelAlignRightItem_Action()
        {
            if (selection.Count < 1) return;        

            int x = 0;

            if (rubberSelected)
            {
                for (int i = 0; i < selection.Count; i++)
                {
                    BaseWidget temp = (BaseWidget)selection[i];

                    if ((temp.Location.X+ temp.Width) > x)
                    {
                        x = temp.Location.X + temp.Width;
                    }
                }
            }
            else
            {
                BaseWidget temp = (BaseWidget)selection[0];

                x = temp.Location.X + temp.Width;
            }

            // Align them all to x

            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget temp = (BaseWidget)selection[i];

                Rectangle b = temp.GetConfigBounds();
                b.X = x - b.Width;
                temp.SetBounds(b.X, b.Y, b.Width, b.Height);

                temp.WidgetData.x = b.X - this.AutoScrollPosition.X;
                temp.WidgetData.y = b.Y - this.AutoScrollPosition.Y;

                temp.Invalidate();
            }

            //Invalidate();
        }

        void SelAlignBottomItem_Action()
        {
            if (selection.Count < 1) return;        
       
            int y = 0;

            if (rubberSelected)
            {
                for (int i = 0; i < selection.Count; i++)
                {
                    BaseWidget temp = (BaseWidget)selection[i];

                    if ((temp.Location.Y + temp.Height) > y)
                    {
                        y = temp.Location.Y + temp.Height;
                    }
                }
            }
            else
            {
                BaseWidget temp = (BaseWidget)selection[0];

                y = temp.Location.Y + temp.Height;
            }

            // Align them all to y

            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget temp = (BaseWidget)selection[i];

                Rectangle b = temp.GetConfigBounds();
                b.Y = y - b.Height;
                temp.SetBounds(b.X, b.Y, b.Width, b.Height);

                temp.WidgetData.x = b.X - this.AutoScrollPosition.X;
                temp.WidgetData.y = b.Y - this.AutoScrollPosition.Y;

                temp.Invalidate();
            }
           // Invalidate();
        }

        void SelSameSizeItem_Action()
        {
            if (selection.Count < 1) return;        

            int h = 0;
            int w = 0;
       
            if (rubberSelected)
            {
                for (int i = 0; i < selection.Count; i++)
                {
                    Rectangle temp = ((BaseWidget)selection[i]).
                        GetConfigBounds();

                    if ((temp.Width > w) || (temp.Height > h))
                    {
                        w = temp.Width;
                        h = temp.Height;
                    }
                }
            }
            else
            {
                Rectangle temp = ((BaseWidget)selection[0]).
                        GetConfigBounds();
                w = temp.Width;
                h = temp.Height;
            }

            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget temp = (BaseWidget)selection[i];
                Rectangle rb = temp.GetConfigBounds();

                rb.Width = w;
                rb.Height = h;

                temp.WidgetData.x = rb.X - this.AutoScrollPosition.X;
                temp.WidgetData.y = rb.Y - this.AutoScrollPosition.Y;
                temp.WidgetData.width = rb.Width;
                temp.WidgetData.height = rb.Height;

                temp.SetBounds(rb.X, rb.Y, rb.Width, rb.Height);
                temp.Invalidate();
            }
            //Invalidate();
        }

        void SelSameDistanceItem_Action()
        {
            if (selection.Count < 1) return;        

            int x = 0;
            int totalXDistance = 0;
            int evenDistance = 0;

            //if (rubberSelected)
            //{
                if (selection.Count > 1)
                {
                    WidgetComparerByLocationX comparer = new WidgetComparerByLocationX();

            
                    selection.Sort(comparer);
                  

                    x = selection[0].Location.X;
              
                    totalXDistance = selection[selection.Count - 1].WidgetData.x - selection[0].WidgetData.x;
                    
                    evenDistance = totalXDistance / (selection.Count - 1);

                    foreach (BaseWidget widget in this.selection)
                    {                    

                        widget.SetBounds(x, widget.Location.Y,
                           widget.Width, widget.Height);

                        widget.WidgetData.x = x - this.AutoScrollPosition.X;

                        x = x + evenDistance;

                        widget.Invalidate();
                    }
                }
            //}
            //else
            //{
            //    //do nothing
            //}
        }

        void SelCenterVerticalItem_Action()
        {
            if (selection.Count < 1) return;   
       
            // Find the Middle of the first widget

            Rectangle b = ((BaseWidget)selection[0]).GetConfigBounds();

            int midx = b.X+ b.Width / 2;

            // Align them all to midx

            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget temp = (BaseWidget)selection[i];

                b = temp.GetConfigBounds();

                int rmid = b.X + b.Width / 2;

                b.X += midx - rmid;

                temp.SetBounds(b.X, b.Y, b.Width, b.Height);

                temp.WidgetData.x = b.X - this.AutoScrollPosition.X;
                temp.WidgetData.y = b.Y - this.AutoScrollPosition.Y;

                temp.Invalidate();
            }
            //Invalidate();
        }

        void SelCenterHorizontalItem_Action()
        {
            if (selection.Count < 1) return;   
         
            // Find the Middle of the first widget

            Rectangle b = ((BaseWidget)selection[0]).GetConfigBounds();

            int midy = b.Y + b.Height / 2;

            // Align them all to midy

            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget temp = (BaseWidget)selection[i];

                b = temp.GetConfigBounds();

                int rmid = b.Y + b.Height / 2;

                b.Y += midy - rmid;

                temp.SetBounds(b.X, b.Y, b.Width, b.Height);

                temp.WidgetData.x = b.X - this.AutoScrollPosition.X;
                temp.WidgetData.y = b.Y - this.AutoScrollPosition.Y;

                temp.Invalidate();
            }
            //Invalidate();
        }

        void SelMoveFrontItem_Action()
        {
            if (selection.Count < 1) return;   
        
             //yank the selected items off the panel
             //and add them back on the first of the component list.
             //That will put them last in draw order, and in save order.

            for (int i = selection.Count - 1; i >= 0; i--)
            {
                BaseWidget temp = (BaseWidget)selection[i];

                temp.BringToFront();
                temp.Invalidate();
            }
            //Invalidate();
        }

        void SelMoveBackItem_Action()
        {
            if (selection.Count < 1) return;   
        
            // yank the selected items off the panel
            // and add them back.  That will put them first in draw order,
            // and in save order.

            for (int i = 0; i < selection.Count; i++)
            {
                BaseWidget temp = (BaseWidget)selection[i];
                temp.SendToBack();
                temp.Invalidate();
            }
           // Invalidate(); 
        }   

        void GloHideGrid_Action()
        {
            showGrid = false;
            gloShowGridItem.Text =SHOW_GRID;
            Invalidate();
        }

        void GloShowGrid_Action()
        {
            showGrid = true;
            gloShowGridItem.Text= HIDE_GRID;
            Invalidate();
        }

        void GloSnapOn_Action()
        {
            snappingEnabled = true;
            gloSnappingItem.Text = DISABLE_SNAPPING; 
        }

        void GloSnapOff_Action()
        {
            snappingEnabled = false;
            gloSnappingItem.Text =ENABLE_SNAPPING;
        }

        void GloSearch_Action()
        {
            new ECP.Gui.Config.Dialogs.SearchWidgetForm(this).Show();
        }

        void GloUndo_Action()
        {

        }

        void GloRedo_Action()
        {

        }

        protected void PerformSelectAllAction()
        {
            this.ClearSelection();
            BaseWidget widget;

            for (int i = 0; i < this.Controls.Count; i++)
            {
                Control c = Controls[i];
                if (c is BaseWidget)
                {
                    widget = (BaseWidget) c;
                    selection.Add(widget);
                    ((BaseWidget)widget).Selected = true;
                }
            }
            Invalidate();
        }

        void SelCopyItem_Action()
        {
            if (EmptyClipboard())
            {
                // Now add the selection to the clipboard
                // Make a deep copy so it won't change when it's parent changes
                // Instead of using the selection vector here, we will search
                // through the panel for the actual widgets with the selected flag
                // This will let us preserve ordering

      
                for (int i = this.Controls.Count - 1; i >= 0; i--)
                {
                    if (!(this.Controls[i] is BaseWidget))
                    {
                        continue;
                    }

                    BaseWidget widget = (BaseWidget)(this.Controls[i]);

                    if (!widget.Selected)
                    {
                        continue;
                    }

                    WidgetData data = BaseWidget.CloneWidgetData(widget.WidgetData);

                    BaseWidget widget2 = CreateWidget(data, false, false);

                    clipboard.Add(widget2);
                }

            }
        }

        void SelCopyPropertyItem_Action()
        {
            if (EmptyClipboard())
            {
                for (int i = this.Controls.Count - 1; i >= 0; i--)
                {
                    if (!(this.Controls[i] is BaseWidget))
                    {
                        continue;
                    }

                    BaseWidget widget = (BaseWidget)(this.Controls[i]);

                    if (!widget.Selected)
                    {
                        continue;
                    }
                    // We just need to have property Only, but now we just copy whole data anyway.
                    WidgetData data = BaseWidget.CloneWidgetData(widget.WidgetData);

                    BaseWidget widget2 = CreateWidget(data, false, false);

                    clipboard.Add(widget2);
                    // we need only one widget
                    break;
                }
            }
        }

        void SelEditSizeItem_Action()
        {
            new ECP.Gui.Config.Dialogs.WidgetSizeForm(this.selection).Show();            
        }

        private bool EmptyClipboard()
        {
            this.clipboard.Clear();
            return true;
        }

        Rectangle WidgetListBounds(List<BaseWidget> widgets)
        {
            if (widgets.Count <= 0)
            {
                return new Rectangle(0, 0, 0, 0);
            }

            BaseWidget widget = (BaseWidget)(widgets[0]);
            Rectangle cb = widget.GetConfigBounds();
            Point p1 = new Point(cb.X, cb.Y);
            Point p2 = new Point(cb.X + cb.Width, cb.Y + cb.Height);

            for (int i = 1; i < widgets.Count; i++)
            {
                widget = (BaseWidget)(widgets[i]);
                cb = widget.GetConfigBounds();
                if (cb.X < p1.X)
                {
                    p1.X = cb.X;
                }
                if (cb.Y < p1.Y)
                {
                    p1.Y = cb.Y;
                }
                if (cb.Width + cb.X > p2.X)
                {
                    p2.X = cb.X + cb.Width;
                }
                if (cb.Height + cb.Y > p2.Y)
                {
                    p2.Y = cb.Y + cb.Height;
                }
            }
            return new Rectangle(p1.X, p1.Y, p2.X - p1.X, p2.Y - p1.Y);
        }

        void SelPasteItem_Action(bool unconfigure)
        {
            if (clipboard.Count > 0)
            {
                this.ClearSelection();
                
                // Find the bounds of the widgets so we can offset them to appear
                // under the cursor

                Rectangle b = this.WidgetListBounds(clipboard);
                
                int dx = mousePosition.X - b.X;
                int dy = mousePosition.Y - b.Y;
        
                // Make a copy of the clipboards contents.  We don't want to add
                // them multiple times if you paste more than once...

                for (int i = clipboard.Count -1 ; i >= 0 ; i--)
                {
                    BaseWidget widget = (BaseWidget)(clipboard[i]);
                    WidgetData data = BaseWidget.CloneWidgetData(widget.WidgetData);


                    Guid newId = Guid.NewGuid(); ;
                    data.guId = newId;

                    if (unconfigure)
                    {
                        data.elementId = "";
                    }

                    widget = CreateWidget(data, false, true);

                    // Put the new widgets under the cursor
                    Rectangle configBounds = widget.GetConfigBounds();


                    widget.WidgetData.x = configBounds.X + dx - this.AutoScrollPosition.X;
                    widget.WidgetData.y = configBounds.Y + dy - this.AutoScrollPosition.Y;

                    widget.WidgetData = widget.WidgetData; // set Bound.

                    widget.SetBounds(configBounds.X + dx, configBounds.Y + dy, widget.Width, widget.Height);

                    widget.Selected = true;
                    selection.Add(widget);

                    widget.Invalidate();
                  
                }        
        
               // Invalidate();
            }
        }

        void SelPastePropertyItem_Action()
        {
            if (clipboard.Count > 0)
            {

                BaseWidget widget = (BaseWidget)(clipboard[0]);
                WidgetData data = BaseWidget.CloneWidgetData(widget.WidgetData);

                foreach (BaseWidget w in this.selection)
                {  
                    if (widget.GetType() == w.GetType())
                    {
                        if (widget is TextWidget)
                        {
                            foreach (UserDefinedPropertyInfo wUd in w.WidgetData.properties)
                            {                                
                                foreach (UserDefinedPropertyInfo dataUd in data.properties)
                                {
                                    if (wUd.name.Equals(dataUd.name))
                                    {
                                        if (dataUd.name.Equals("Text"))
                                        {
                                            //do not copy text
                                        }
                                        else
                                        {
                                            wUd.value = dataUd.value;
                                        }
                                        break;
                                    }
                                }
                            }

                        }
                        else
                        {
                            w.WidgetData.properties = data.properties;
                        }
                        w.InitializeWidgetProperties();
                        w.Invalidate();
                    }
                }
            }
        }

        void SelCutItem_Action()
        {
            if (EmptyClipboard())
            {
                // Now add the selection to the clipboard
                // Instead of using the selection vector here, we will search
                // through the panel for the actual widgets with the selected flag
                // This will let us preserve ordering

                for (int i = this.Controls.Count - 1; i >= 0; i--)
                {
                    Control c = this.Controls[i];
                    if (c is BaseWidget)
                    {
                        BaseWidget widget = (BaseWidget)c;

                        if (widget.Selected)
                        {
                            clipboard.Add(widget);
                        }
                    }
                }

       
                for (int i = 0; i < selection.Count; i++)
                {
                    BaseWidget widget = (BaseWidget)(selection[i]);

                    this.Controls.Remove(widget);


                    widget.MouseUp -= new MouseEventHandler(this.MouseReleasedHandler);
                    widget.MouseDown -= new MouseEventHandler(this.MousePressedHandler);
                    widget.MouseLeave -= new EventHandler(this.MouseExitHandler);
                    if (UserPermission.GetInstance().HasPermission(PERMISSION_WIDGET_DROW))
                    {
                        widget.MouseMove -= new MouseEventHandler(this.MouseMoveHandler);
                    }
                    widget.KeyUp -= new KeyEventHandler(this.KeyUpHandler);
                    widget.KeyDown -= new KeyEventHandler(KeyDownHandler);
                    widget.Invalidate();

                }
                //Invalidate();
                ClearSelection();
            }
        }

        void SelDeleteItem_Action()
        {
            if (!this.textEditMode)
            {
                DeleteSelection();
            }
        }

        private void DeleteSelection()
        {
            if (ContainsConfigured(selection))
            {
                string message = "This view contains configured subsystem with only "+
                    "on associated view Do you want to delete them from the system as well ! ";
                string caption = "Widget Delete";
                MessageBoxButtons buttons = MessageBoxButtons.YesNoCancel;
                DialogResult result;

                // Displays the MessageBox.

                result = MessageBox.Show(this, message, caption, buttons);

                if (result == DialogResult.Yes)
                {
                    // We delete them from the system.
                    for (int i = selection.Count - 1; i >= 0; i--)
                    {
                        BaseWidget widget = (BaseWidget)(selection[i]);

                        if (!DeleteFromSystem(widget))
                        {
                            // Don't do anything.
                            //return;
                        }
                        selection.RemoveAt(i);
                        this.Controls.Remove(widget);
                        widget.Invalidate();
                    }

                }
                else if (result == DialogResult.No) /// we delete widget but not from system
                {
                    for (int i = selection.Count - 1; i >= 0; i--)
                    {
                        BaseWidget widget = (BaseWidget)(selection[i]);

                        selection.RemoveAt(i);
                        this.Controls.Remove(widget);
                        widget.Invalidate();
                    }
                }
                else if (result == DialogResult.Cancel)
                {
                    return;
                }
            }
            else
            {
                for (int i = selection.Count - 1; i >= 0; i--)
                {
                    BaseWidget widget = (BaseWidget)(selection[i]);

                    //if (!DeleteFromSystem(widget))
                    //{
                    //    return;
                    //}
                    selection.Remove(widget);
                    this.Controls.Remove(widget);
                    widget.Invalidate();
                }
            }
            //Invalidate();
        }
      
        protected bool ContainsConfigured(List<BaseWidget> deathRow)
        {
            // Are any configured and the last view?

            bool configuredDevices = false;

            for (int i = 0; i < deathRow.Count; i++)
            {
                BaseWidget widget = (BaseWidget)(deathRow[i]);

                if (!widget.ElementId.Equals("") &&
                    WidgetViewCount(widget.ElementId) == 1)
                {
                    configuredDevices = true;
                    break;
                }
            }
            return configuredDevices;
        }

        protected int WidgetViewCount(String name)
        {
            int count = 0;

            for (int i = 0; i < this.Controls.Count; i++)
            {
                Control c = this.Controls[i];
                if (c is BaseWidget)
                {
                    BaseWidget widget = (BaseWidget)c;
                    if (!widget.ElementId.Equals("") && widget.ElementId.Equals(name))
                    {
                        count++;
                    }
                }
            }

            return count;
        }

        protected virtual bool DeleteFromSystem(BaseWidget widget)
        {  
            return true;
        }

        void SelPropertyItem_Action()
        {
            BaseWidget widget = (BaseWidget)(selection[0]);

            if (widget is BaseWidget)
            {
                ((BaseWidget)widget).ShowProperty();
            }
            else
            {
                // Don't do anything.  We don't know how
                return;
            }

        }

        void SelUDPropertyItem_Action()
        {
            BaseWidget widget = (BaseWidget)(selection[0]);

            if (widget is BaseWidget)
            {
                ((BaseWidget)widget).ShowUDProperty();
            }
            else
            {
                // Don't do anything.  We don't know how
                return;
            }

        }

        void SelEditItem_Action()
        {
            //Create & show the appropriate config edit dialog.

            //Widget 하나만 선택되었을 경우
            if (selection.Count == 1)
            {
                BaseWidget widget = (BaseWidget)(selection[0]);

                if (widget is BaseWidget)
                {
                    if (widget is TextWidget)
                    {
                        label = (TextWidget)widget;

                        labelTextField = new TextBox();

                        labelTextField.SetBounds(label.Location.X, label.Location.Y, label.Width, label.Height);


                        labelTextField.Font = label.GetFont();

                        labelTextField.Text = label.WidgetText;

                        label.Visible = false;


                        this.Controls.Add(labelTextField);


                        labelTextField.KeyUp += new KeyEventHandler(labelTextField_KeyUp);
                        labelTextField.LostFocus += new EventHandler(labelTextField_LostFocus);


                        labelTextField.Visible = true;
                        labelTextField.BringToFront();
                        labelTextField.Focus();
                        
                        textEditMode = true;

                    }
                    else
                    {
                        ((BaseWidget)widget).ShowWizard(this.owner);   
                    }
                }
                else
                {
                    // Don't do anything.  We don't know how
                    return;
                }
            }
            //Widget 여러 개가 선택되었을 경우
            else
            {
                BaseWidget firstWidget = (BaseWidget)(selection[0]);
             
                int arraySize =0;
                int inx = 0;

                for (int i = 0; i < selection.Count; i++)
                {
                    BaseWidget widget = (BaseWidget)selection[i];
                    
                    if(firstWidget.GetType() == widget.GetType())
                        arraySize ++;
                }

                string[] elementsIds = new string[arraySize];

                for (int i = 0; i < selection.Count; i++)
                {
                    BaseWidget widget = (BaseWidget)selection[i];

                    if (firstWidget.GetType() == widget.GetType())
                    {
                        elementsIds[inx] = widget.ElementId;
                        inx++;
                    }
                }
         
                if (firstWidget is BaseWidget)
                {
                    ((BaseWidget)firstWidget).ShowWizard(this.owner, elementsIds);
                }
            }
        }


    }

    public class WidgetComparerByLocationX : IComparer<BaseWidget>
    {
        public int Compare(BaseWidget x, BaseWidget y)
        {
            if (x == null)
            {
                if (y == null)
                {
                    // If x is null and y is null, they're equal. 
                    return 0;
                }
                else
                {
                    // If x is null and y is not null, y is greater.
                    return 1;
                }
            }
            else
            {
                // If x is not null...
                if (y == null)
                // ...and y is null, x is greater.
                {
                    return -1;
                }
                else
                {
                    if (x.WidgetData.x < y.WidgetData.x)
                        return -1;
                    else if (x.WidgetData.x > y.WidgetData.x)
                        return 1;
                    else
                    {
                        return 0;
                    }
                }
            }
        }
    }

    public class RedoCommand
    {    
        BaseWidget widgetRef;

        public RedoCommand(BaseWidget widget,WidgetData data)
        {
            this.widgetRef = widget;
        }
    }
}
