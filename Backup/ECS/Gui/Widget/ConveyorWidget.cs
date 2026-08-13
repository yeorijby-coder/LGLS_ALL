using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using ECP.Gui.Widget;
using ECP.Event;
using HECS.Event.ECS;
using HECS.Device.Unit;
using ECP.Util.Client;
using HECS.Device;
using ConveyorEvent = HECS.Event.ECS.MasterConveyorEvent;
using ECP.Service.Material;
using ECP.Global.Device;
using System.Drawing.Imaging;

namespace HECS.Gui.Widget
{


    public partial class ConveyorWidget : HECSBaseWidget
    {


        public enum PORT_TYPE
        {
            MOVING = 1,
            UNLOAD = 2,
            LOAD = 3,
            LOAD_UNLOAD = 4,
        }


        private PORT_TYPE portType = PORT_TYPE.MOVING;

        public PORT_TYPE PortType
        {
            get
            {
                return portType;
            }
            set
            {
                portType = value;
            }
        }
        private Conveyor conveyorObject = null;

        public Conveyor ConveyorObject
        {
            get
            {
                return conveyorObject;
            }
            set
            {
                conveyorObject = value;
                if (conveyorObject != null)
                {
                    if (ECP.Global.GlobalConstant.START_MODE == "ECS")
                    {
                        conveyorObject.PropertyChanged += new PropertyChangedEventHandler(Object_PropertyChanged);
                        Object_PropertyChanged(conveyorObject, new PropertyChangedEventArgs("DefaultValue"));
                    }

                }

            }
        }
        private Port portObject = null;

        public Port PortObject
        {
            get
            {
                return portObject;
            }
            set
            {
                portObject = value;
                if (portObject != null)
                {
                    if (ECP.Global.GlobalConstant.START_MODE == "ECS")
                    {
                        portObject.PropertyChanged += new PropertyChangedEventHandler(Object_PropertyChanged);
                        Object_PropertyChanged(portObject, new PropertyChangedEventArgs("DefaultValue"));
                    }
                }
            }
        }

        void Object_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            Invalidate();
        }


        private delegate void WidgetRefresh();
        /// <summary>
        /// Initializes a new instance of the <see cref="ConveyorWidget"/> class.
        /// </summary>
        public ConveyorWidget()
            : base()
        {
            InitializeComponent();
            tooltip.ToolTipIcon = ToolTipIcon.Info;
        }

        public ConveyorWidget(string elementId)
            : base()
        {
            InitializeComponent();
            this.ElementId = elementId;
            tooltip.ToolTipIcon = ToolTipIcon.Info;
        }

        public override void SetElementIDAtDesignMode(string elementId) //BaseWidget.cs
        {
            this.ElementId = elementId;
            this.conveyorObject.IsAvailible = true;
        }

        private void SetDefaultConveyorInfo()
        {
            if (conveyorObject != null)
            {
                //    if (this.plcId != conveyorObject.PlcID)
                //    {
                //        this.plcId = conveyorObject.PlcID;
                //    }
                //    if (this.isShared != conveyorObject.Shared)
                //    {
                //        this.isShared = conveyorObject.Shared;
                //    }
                //    if (this.isReserved != conveyorObject.IsReserved)
                //    {
                //        this.isReserved = conveyorObject.IsReserved;
                //    }
                //    if (this.isAvailible != conveyorObject.IsAvailible)
                //    {
                //        this.isAvailible = conveyorObject.IsAvailible;
                //    }
                //    if (this.imHere != conveyorObject.ImHere)
                //    {
                //        this.imHere = conveyorObject.ImHere;
                //    }
            }
        }

        protected override void OnMouseHover(EventArgs e)
        {
            base.OnMouseHover(e);
            if (this.conveyorObject != null)
            {
                //string tooltipText = "";
                //string masterRackmaster = ((this.conveyorObject.MasterRackmaster != null) ? this.conveyorObject.MasterRackmaster.Trim() : "");
                //string slaveRackmaster = ((this.conveyorObject.SlaveRackmaster != null) ? this.conveyorObject.SlaveRackmaster.Trim() : "");

                //tooltip.ToolTipTitle = "[" + this.conveyorObject.ElementId + "]";
                //tooltipText = tooltipText + "─────────────────" + "\r\n";
                //tooltipText = tooltipText + " PLC ID   \t: " + this.conveyorObject.PlcID + "\r\n";                
                //tooltipText = tooltipText + " Zone     \t: " + this.conveyorObject.HostID + "\r\n";
                //tooltipText = tooltipText + " Zone Part\t: " + this.conveyorObject.ZonePart + "\r\n";
                //tooltipText = tooltipText + " Zone Type\t: " + this.conveyorObject.ZoneType + "\r\n";
                //tooltipText = tooltipText + " XPosition\t: " + this.conveyorObject.Position + "\r\n";
                //tooltipText = tooltipText + "─────────────────" + "\r\n";
                //tooltipText = tooltipText + " CarrierID\t: " + (this.conveyorObject.GetCarrier() != null ? this.conveyorObject.GetCarrier() : "") + "\r\n";
                //tooltipText = tooltipText + " 입고금지 \t: " + this.conveyorObject.IsProhibited.ToString() + "\r\n";
                //tooltipText = tooltipText + "─────────────────" + "\r\n";
                //tooltipText = tooltipText + " Availible\t: " + this.conveyorObject.IsAvailible.ToString() + "\r\n";
                //tooltipText = tooltipText + " 반송예약 \t: " + this.conveyorObject.IsReserved.ToString() + "\r\n";
                //tooltipText = tooltipText + " 공유여부 \t: " + this.conveyorObject.Shared.ToString() + "\r\n";
                //tooltipText = tooltipText + "─────────────────" + "\r\n";
                //tooltipText = tooltipText + " Master   \t: " + masterRackmaster + "\r\n";
                //tooltipText = tooltipText + " Slave    \t: " + slaveRackmaster;
                //tooltip.Show (tooltipText, this,this.Width,this.Height,15000);
            }
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            e.Graphics.SmoothingMode = SmoothingMode.HighQuality;
            e.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;
            int carrierGap = 4;
            Rectangle carrierRect = new Rectangle(carrierGap, carrierGap, this.Width - (carrierGap * 2), this.Height - (carrierGap * 3));
            DrawCarrier(e.Graphics, carrierRect);

            int prohibitGap = 3;
            Rectangle prohibitRect = new Rectangle(prohibitGap, prohibitGap, this.Width - (prohibitGap * 2) - 1, this.Height - (prohibitGap * 2) - 1);
            //DrawProhibit(e.Graphics, prohibitRect);
            //DrawPlcId(e.Graphics);

            if (this.selected == true)
            {
                Pen p = new Pen(Color.Red, 2);
                Rectangle selectedRect = new Rectangle(1, 1, this.Width - 3, this.Height - 3);
                e.Graphics.DrawRectangle(p, selectedRect);
            }

            DrawText(e.Graphics);

        }

        private void DrawText(Graphics graphics)
        {
            if (string.IsNullOrEmpty(this.WidgetText))
            {
                return;
            }

            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Center;
            stringFormat.LineAlignment = StringAlignment.Center;
            graphics.DrawString(this.WidgetText, WidgetTextFont, new SolidBrush(this.WidgetTextColor), new RectangleF(0, this.Height / 2, this.Width, this.Height / 2), stringFormat);
            if (portObject != null)
            {
                Conveyor ownerConveyor = ECSDeviceManager.GetConveyor(portObject.OwnerId);
                if (ownerConveyor != null)
                {
                    string palletId = ownerConveyor.GetPallet(this.portObject);
                    if (string.IsNullOrEmpty(palletId) == false)
                    {
                        graphics.DrawString(palletId, WidgetTextFont, new SolidBrush(Color.Yellow), new RectangleF(0, 0, this.Width, this.Height / 2), stringFormat);
                    }
                }
            }
        }

        //private void DrawPlcId(Graphics graphics)
        //{

        //    //if (this.plcId != null && !this.plcId.Equals(""))
        //    //{
        //    //    StringFormat stringFormat = new StringFormat();
        //    //    stringFormat.Alignment = StringAlignment.Center;
        //    //    stringFormat.LineAlignment = StringAlignment.Far;

        //    //    Font font = new Font("Arial", 6, FontStyle.Regular);
        //    //    graphics.DrawString(plcId, font, Brushes.Black, new RectangleF(0, this.Height - 15, this.Width, 15), stringFormat);
        //    //}
        //}

        //private void DrawProhibit(Graphics graphics, Rectangle phbRect)
        //{
        //    if (conveyorObject != null && conveyorObject.IsProhibited == true)
        //    {
        //        Color backColor = Color.MistyRose;
        //        Color foreColor = Color.Red;
        //        DrawRoundFillRectangle(graphics, phbRect, 1, backColor, foreColor, 2);
        //        Pen pen = new Pen(Color.Red, 2);
        //        graphics.DrawLine(pen, phbRect.X, phbRect.Y, phbRect.Width, phbRect.Height);
        //        graphics.DrawLine(pen, phbRect.Width, phbRect.Y, phbRect.X, phbRect.Height);
        //        pen.Dispose();
        //    }
        //}

        private void DrawCarrier(Graphics graphic, Rectangle drawRect)
        {
            if (portObject != null)
            {
                Conveyor ownerConveyor = ECSDeviceManager.GetConveyor(portObject.OwnerId);
                if (ownerConveyor == null)
                {
                    return;
                }
                string palletId = ownerConveyor.GetPallet(this.portObject);

                if (portObject.IsPalletExist && string.IsNullOrEmpty(palletId) == false)
                {
                    DrawRoundRect(graphic, new Rectangle(3, 3, this.Width - 7, this.Height - 7), 1, Brushes.RoyalBlue, Brushes.RoyalBlue, 1);
                }
                else if (portObject.IsPalletExist && string.IsNullOrEmpty(palletId))
                {
                    DrawRoundRect(graphic, new Rectangle(3, 3, this.Width - 7, this.Height - 7), 1, Brushes.DarkGreen, Brushes.RoyalBlue, 1);
                }
                else if (portObject.IsPalletExist == false && string.IsNullOrEmpty(palletId) == false)
                {
                    DrawRoundRect(graphic, new Rectangle(3, 3, this.Width - 7, this.Height - 7), 1, Brushes.Crimson, Brushes.RoyalBlue, 1);
                }
                return;
            }


            //if (conveyorObject != null)
            //{
            //    return;
            //}
            //if (conveyorObject == null)
            //{
            //    return;
            //}

            //string carrierId = conveyorObject.GetCarrier();

            //if (carrierId == null || carrierId.Trim().Equals(""))
            //{
            //    return;
            //}

            //Carrier carrier = Naming.GetCarrierManager().GetCarrier(carrierId);

            //if (carrier != null)
            //{
            //    //    Bitmap carrierBitmap = CarrierWidget.GetCarrier(carrier.CarrierType);
            //    //    if (this.ImHere == true && this.tick == 1)
            //    //    {
            //    //        graphic.DrawImage(carrierBitmap, new Rectangle(drawRect.X-3,drawRect.Y-3,drawRect.Width+6, drawRect.Height +6 ));

            //    //        if (carrier.CarrierHold != null && carrier.CarrierHold == CarrierHold.SET)
            //    //        {
            //    //            Pen p = new Pen(Color.Red);
            //    //            p.Width = 3;
            //    //            graphic.DrawRectangle(p, new Rectangle(drawRect.X - 3, drawRect.Y - 3, drawRect.Width + 6, drawRect.Height + 6));
            //    //        }
            //    //    }
            //    //    else
            //    //    {
            //    //        graphic.DrawImage(carrierBitmap, drawRect);

            //    //        if (carrier.CarrierHold != null && carrier.CarrierHold == CarrierHold.SET)
            //    //        {
            //    //            Pen p = new Pen(Color.Red);
            //    //            p.Width = 3;
            //    //            graphic.DrawRectangle(p, drawRect);
            //    //        }
            //    //    }
            //}
        }
    }



}
