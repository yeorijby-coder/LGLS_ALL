using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Gui.Widget;
using ECP.Event;
using System.Drawing.Drawing2D;
using HECS.Device;
 
using ECP.Util.Client;
/// Date:2008.06.16, 
/// Writer:Kim Seong Won
/// Location:신성 Eng (음성 공장)
/// SafetySenor의 상태를 보여줍니다.
/// 
namespace HECS.Gui.Monitor.Controls
{
    /// <summary>
    /// 스토커의 메인트-옵션 화면에 SafetySensor 상태를 보여줍니다.
    /// </summary>
    public partial class StkSafetySensorUsedStateViewer : BaseControl
    {

        private bool isHPSensorOn = false;
        public bool IsHPSensorOn
        {
            get { return isHPSensorOn; }
            set { isHPSensorOn = value; Invalidate(); }
        }

        private bool isMDSensorOn = false;
        public bool IsMDSensorOn
        {
            get { return isMDSensorOn; }
            set { isMDSensorOn = value; Invalidate(); }
        }

        private bool isOPSensorOn = false;
        public bool IsOPSensorOn
        {
            get { return isOPSensorOn; }
            set { isOPSensorOn = value; Invalidate(); }
        }

        private bool isPortSideSensorOn = false;
        public bool IsPortSideSensorOn
        {
            get { return isPortSideSensorOn; }
            set { isPortSideSensorOn = value; Invalidate(); }
        }

        private bool isEqpSideSensorOn = false;
        public bool IsEqpSideSensorOn
        {
            get { return isEqpSideSensorOn; }
            set { isEqpSideSensorOn = value; Invalidate(); }
        }

        private bool isDual = true;
        public bool IsDual
        {
            get { return isDual; }
            set { isDual = value; }
        }


        #region 생성자

        public StkSafetySensorUsedStateViewer()
        {
            InitializeComponent();
            //this.subscribe();
        }

        #endregion 생성자
        protected override void OnParentRightToLeftChanged(EventArgs e)
        {
            base.OnParentRightToLeftChanged(e);
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);

            //dual            
            if (IsDual == true)
            {
                Graphics g = panelSensorMonitor.CreateGraphics();
                //Graphics g = e.Graphics;
                //조건에 따라서 brush의 color를 변경 해주는 부분
                Brush fillBrush = Brushes.Lime;
                Brush borderBrush = Brushes.Black;
                Brush textBrush = Brushes.Black;

                //Block 크기 조정            
                int boxW, boxH;
                int monWidth, monHeight;
                int temp, temp2;

                monHeight = panelSensorMonitor.Height;
                monWidth = panelSensorMonitor.Width;

                temp = monHeight / 10; //10 등분
                temp2 = monWidth / 8;  // 5 등분

                boxW = monWidth / 20;
                boxH = monHeight / 10;
                if (boxW < 5) boxW = 5;
                if (boxH < 5) boxH = 5;

                //Line 크기 조정
                int lineW, lineH;
                lineW = monWidth - (boxW * 2);
                lineH = monHeight / 20;
                if (lineW < 2) lineW = 2;
                if (lineH < 2) lineH = 2;

                int rackmasterW, rackmasterH;
                rackmasterW = monWidth / 6;
                rackmasterH = monHeight / 3;

                //Block 표시 부분.
                //HP
                DrawRect(g, new Rectangle(0, (temp * 2) - (boxH / 2), boxW, boxH), Brushes.White, fillBrush);
                DrawRect(g, new Rectangle(0, (monHeight / 2) - (boxH / 2), boxW, boxH), Brushes.White, fillBrush);
                DrawRect(g, new Rectangle(0, (temp * 8) - (boxH / 2), boxW, boxH), Brushes.White, fillBrush);
                //OP
                DrawRect(g, new Rectangle(monWidth - boxW, (temp * 2) - (boxH / 2), boxW, boxH), Brushes.White, fillBrush);
                DrawRect(g, new Rectangle(monWidth - boxW, (monHeight / 2) - (boxH / 2), boxW, boxH), Brushes.White, fillBrush);
                DrawRect(g, new Rectangle(monWidth - boxW, (temp * 8) - (boxH / 2), boxW, boxH), Brushes.White, fillBrush);


                //Line 표시 부분.
                fillBrush = Brushes.Red;
                //Port
                if (this.IsPortSideSensorOn)
                {
                    DrawRect(g, new Rectangle(boxW, (temp * 2) - (lineH / 2), lineW, lineH), Brushes.Lime, Brushes.LimeGreen);
                }
                else
                {
                    DrawRect(g, new Rectangle(boxW, (temp * 2) - (lineH / 2), lineW, lineH), Brushes.Red, Brushes.Pink);
                }

                //ProcessPort
                if (this.isEqpSideSensorOn)
                {
                    DrawRect(g, new Rectangle(boxW, (temp * 8) - (lineH / 2), lineW, lineH), Brushes.Lime, Brushes.LimeGreen);
                }
                else
                {
                    DrawRect(g, new Rectangle(boxW, (temp * 8) - (lineH / 2), lineW, lineH), Brushes.Red, Brushes.Pink);
                }

                //HP Side
                if (this.IsHPSensorOn)
                {
                    DrawRect(g, new Rectangle(boxW, (monHeight / 2) - (lineH / 2), (monHeight / 2) - (lineH / 2) - boxW, lineH), Brushes.Lime, Brushes.LimeGreen);
                }
                else
                {
                    DrawRect(g, new Rectangle(boxW, (monHeight / 2) - (lineH / 2), (monHeight / 2) - (lineH / 2) - boxW, lineH), Brushes.Red, Brushes.Pink);
                }


                //Mid Side
                if (this.isMDSensorOn)
                {
                    DrawRect(g, new Rectangle(temp2 * 2 + rackmasterH + boxW + 18,
                        (monHeight / 2) - (lineH / 2), temp2 * 5 - boxW - 2 - (temp2 * 2 + rackmasterH + boxW + 18), lineH), Brushes.Lime, Brushes.LimeGreen);
                }
                else
                {
                    DrawRect(g, new Rectangle(temp2 * 2 + rackmasterH + boxW + 18,
                        (monHeight / 2) - (lineH / 2), temp2 * 5 - boxW - 2 - (temp2 * 2 + rackmasterH + boxW + 18), lineH), Brushes.Red, Brushes.Pink);
                }

                //OP Side           
                if (this.IsOPSensorOn)
                {
                    DrawRect(g, new Rectangle(temp2 * 5 + rackmasterH + boxW + 18, (monHeight / 2) - (lineH / 2), (monWidth - boxW) - (temp2 * 5 + rackmasterH + 18 + boxW), lineH), Brushes.Lime, Brushes.LimeGreen);
                }
                else
                {
                    DrawRect(g, new Rectangle(temp2 * 5 + rackmasterH + boxW + 18, (monHeight / 2) - (lineH / 2), (monWidth - boxW) - (temp2 * 5 + rackmasterH + 18 + boxW), lineH), Brushes.Red, Brushes.Pink);
                }



                //RM 표시 부분
                fillBrush = Brushes.Black;
                Rectangle rmHp = new Rectangle(temp2 * 2, (monHeight / 2) - (rackmasterH / 2), rackmasterW, rackmasterH);
                Rectangle sensor = new Rectangle(temp2 * 2 - boxW - 2, (monHeight / 2) - (boxH / 2), boxW, boxH);
                //HOME
                DrawRoundRect(g, rmHp, 3, Brushes.Lime, Brushes.Black, 4);
                //Block
                DrawRect(g, sensor, Brushes.White, fillBrush);

                sensor = new Rectangle(temp2 * 2 + rackmasterH + 18, (monHeight / 2) - (boxH / 2), boxW, boxH);
                DrawRect(g, sensor, Brushes.White, fillBrush);
                //REAR
                DrawRoundRect(g, new Rectangle(temp2 * 5, (monHeight / 2) - (rackmasterH / 2), rackmasterW, rackmasterH), 2, Brushes.Lime, Brushes.Black, 4);
                //Block
                DrawRect(g, new Rectangle(temp2 * 5 - boxW - 2, (monHeight / 2) - (boxH / 2), boxW, boxH), Brushes.White, fillBrush);
                DrawRect(g, new Rectangle(temp2 * 5 + rackmasterH + 18, (monHeight / 2) - (boxH / 2), boxW, boxH), Brushes.White, fillBrush);
                g.Dispose();
            }
            //오버랩 관련해서 넣어줘야 할듯,..오버랩구분이 없어서 주석
            //else if (true)
            //{
            //}
            //else if (true)
            //{
            //}
            //single
            else
            {
                Graphics g = panelSensorMonitor.CreateGraphics();
                int heightCenter = panelSensorMonitor.Height/2;
                int widthCenter = panelSensorMonitor.Width/2;

                Rectangle rackmaster = new Rectangle(0, 0, 0, 0);
                DrawRoundRect(g, rackmaster,3, Brushes.Lime, Brushes.Black, 4);
                g.Dispose();
            }
        }
        //public void subscribe()
        //{
        //    if (stkSafetySensorUsedEventAdapter == null)
        //    {
        //        stkSafetySensorUsedEventAdapter = new StkSafetySensorUsedEventAdapter();
        //    }

        //    if (!stkSafetySensorStateViewerbyName.ContainsKey(this.ElementId))
        //    {
        //        stkSafetySensorStateViewerbyName.Add(this.ElementId, this);
        //    }
        //}

        //public void UpdateWidget()
        //{
            //switch (this.stockerSensorEventType)
            //{
            //    case MasterStockerSensorEvent.StockerEventType.HPSIDE_SAFETYSENSOR_USE:
            //        break;
            //    case MasterStockerSensorEvent.StockerEventType.MDSIDE_SAFETYSENSOR_USE:
            //        break;
            //    case MasterStockerSensorEvent.StockerEventType.OPSIDE_SAFETYSENSOR_USE:
            //        break;
            //    case MasterStockerSensorEvent.StockerEventType.PORTSIDE_SAFETYSENSOR_USE:
            //        break;
            //    case MasterStockerSensorEvent.StockerEventType.EQPSIDE_SAFETYSENSOR_USE:
            //        break;

            //}
        //}

        //internal void UpdateWidgetDelegate()
        //{
        //    //if(this.InvokeRequired)
        //    this.BeginInvoke(new delegateUpdateWidget(UpdateWidget));
        //}

        //public static void UpDateStkSafetySensorUsedEvent(ECPEvent ecpEvent)
        //{
        //    MasterStockerSensorEvent mse = (MasterStockerSensorEvent)ecpEvent;
        //    if (stkSafetySensorStateViewerbyName.ContainsKey(mse.DeviceID))
        //    {
        //        StkSafetySensorUsedStateViewer widget = stkSafetySensorStateViewerbyName[mse.DeviceID];
        //        widget.stockerSensorEventType = mse.StockerSensorEvent;
        //        //widget.UpdateWidgetDelegate();
        //    }
        //}

        //private class StkSafetySensorUsedEventAdapter : ECPEventAdapter
        //{
        //    public StkSafetySensorUsedEventAdapter()
        //    {
        //        Subscribe();
        //    }
        //    public override string GetSubscriptionSubject()
        //    {
        //        return MasterStockerSensorEvent.FormSubject("*");
        //    }
        //    public override void ReceiveECPEvent(ECPEvent ecpEvent)
        //    {
        //        UpDateStkSafetySensorUsedEvent(ecpEvent);
        //    }
        //}

    }


}
