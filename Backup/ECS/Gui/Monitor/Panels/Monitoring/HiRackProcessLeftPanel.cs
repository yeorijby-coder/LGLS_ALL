using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using HECS.Device;
using HECS.Gui.Widget;

namespace HECS.Gui.Monitor.Panels.Monitoring
{
    public partial class HiRackProcessLeftPanel : UserControl
    {


        private int maxRow = 13;
        private int maxColumn = 40;
        private int offsetLeft = 20;
        private int offsetTop = 20;



        private StockerSEM stocker = null;

        public StockerSEM Stocker
        {
            get { return stocker; }
            set { stocker = value; }
        }



        public HiRackProcessLeftPanel()
        {
            InitializeComponent();

            //나중에는 제거되어야 하고, 
            //StockerSEM이 다시 설정될 때,
            //Delegated 된 이벤트를 모두 해제하고 
            //다시 이벤트를 등록해 주어야 한다. 
            InitializeHirack();
        }

        private void InitializeHirack()
        {
            HiRackCellWidget widget = null;
            for (int i = 0; i < maxRow; i++)
            {
                for (int j = 0; j < maxColumn; j++)
                {

                    if (j == 0)
                    {
                        Label label = new Label();
                        label.AutoSize = false;
                        label.Text = (maxRow - i).ToString();
                        label.TextAlign = ContentAlignment.MiddleCenter;
                        label.ForeColor = Color.Blue;
                        label.Font = new Font("Arial", 6.5F, FontStyle.Regular);
                        this.Controls.Add(label);
                        label.SetBounds(0, i * 14 + offsetTop, 16, 16);
                    }
                    if (i == 0)
                    {
                        Label label = new Label();
                        label.AutoSize = false;
                        label.Text = (j + 1).ToString();
                        label.Font = new Font("Arial", 6.5F, FontStyle.Regular);
                        label.TextAlign = ContentAlignment.MiddleCenter;
                        label.ForeColor = Color.Blue;
                        this.Controls.Add(label);
                        label.SetBounds(j * 14 + offsetLeft, 0, 16, 16);
                    }
                    widget = new HiRackCellWidget();
                    widget.Row = i;
                    widget.Column = j;
                    if (stocker != null)
                    {
                        widget.StockerID = stocker.HostID;
                    }
                    else
                    {
                        widget.StockerID = "[Undefined]";
                    }
                    this.Controls.Add(widget);
                    widget.SetBounds(j * 14 + offsetLeft, i * 14 + offsetTop , 16, 16);
                    widget.BorderWidth = 0;
                    widget.WidgetBackColor = Color.Black;
                    widget.WidgetForeColor = Color.White;
                    // widget.MaterialImage = imageListProduct.Images[(i + j) % 7];
                }


            }
        }




    }
}
