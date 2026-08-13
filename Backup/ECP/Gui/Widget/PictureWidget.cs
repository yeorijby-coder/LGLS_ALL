using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;


using ECP.Gui.Util;
using ECP.Global;

namespace ECP.Gui.Widget
{
    public partial class PictureWidget : BaseWidget
    {

        public const string PROPERTY_NAME_PICTURE_IMAGE = "PictureImage";

        protected static PropertyInfo PROPERTY_PICTURE_IMAGE =
            new PropertyInfo(PROPERTY_NAME_PICTURE_IMAGE,
                 PropertyInfo.PROPERTY_TYPE_STRING, "");


        private static Dictionary<string, PictureWidget> pictureWidgetByName =
           new Dictionary<string, PictureWidget>();

        private string picturePath;

        public string PicturePath
        {
            get
            {
                return picturePath;
            }
            set
            {
                picturePath = value;
                this.SetProperty(PROPERTY_NAME_PICTURE_IMAGE, this.picturePath);
            }
        }

        private Bitmap image, filteredImage = null;

        private FillColorFiter fillColorFilter = new FillColorFiter();

        private byte fillA = 0, fillR = 0, fillG = 0, fillB = 0;


        //¿Ã∫•∆Æ
        //private static PortStateChangeAdapter portStateChangeAdapter = null;


        public PictureWidget()
        {
            InitializeComponent();

            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.BackColor = System.Drawing.Color.Transparent;

            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, false);
        }


        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ExStyle |= 0x00000020; //WS_EX_TRANSPARENT 

                return cp;
            }
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            // Don't paint background           
        }

        public override void Subscribe()
        {
            ////OutPutEnable
            //if (portOutPutEnable == null)
            //{
            //    portOutPutEnable = new PortOutPutEnable();
            //}

            ////PortRemove
            //if (portRemove == null)
            //{
            //    portRemove = new PortRemove();
            //}


            //if (!pictureWidgetByName.ContainsKey(this.ElementId))
            //{
            //    pictureWidgetByName.Add(this.ElementId, this);
            //}
        }

        private delegate void delegateUpdateWidget();

        public void UpdateWidgetDelegate()
        {
            //if(this.InvokeRequired)
            this.BeginInvoke(new delegateUpdateWidget(UpdateWidget));
        }

        public void UpdateWidget()
        {

        }

        //public static void UpdateAGVStateChange(ECPEvent ecpEvent)
        //{
        //    StockerPortRunIdleDownEvent agv = (StockerPortRunIdleDownEvent)ecpEvent;
        //    if (AGVPortWidgetByName.ContainsKey(agv.PortId))
        //    {
        //        AGVPortWidget widget = AGVPortWidgetByName[agv.PortId];
        //        widget.portState = agv.PortState;
        //        widget.UpdateWidgetDelegate();
        //    }
        //}


        #region EventListener

        //private class PortStateChangeAdapter : ECPEventAdapter
        //{
        //    public PortStateChangeAdapter()
        //    {
        //        Subscribe();
        //    }

        //    public override string GetSubscriptionSubject()
        //    {
        //        return StockerPortRunIdleDownEvent.FormSubject("*");
        //    }

        //    public override void ReceiveECPEvent(ECPEvent ecpEvent)
        //    {
        //        UpdateAGVStateChange(ecpEvent);
        //    }
        //}

        #endregion EventListener


        protected override void OnPaint(PaintEventArgs e)
        {

            Graphics g = e.Graphics;
            if (image != null)
            {
                //Green
                //fillA = 255;
                //fillR = 0;
                //fillG = 150;
                //fillB = 0;

                //Blue
                //fillA = 255;
                //fillR = 0;
                //fillG = 0;
                //fillB = 150;

                // Red
                //fillA = 255;
                //fillR = 150;
                //fillG = 0;
                //fillB = 0;
                // Nothing


                fillA = 255;
                fillR = 0;
                fillG = 0;
                fillB = 0;

                fillColorFilter.FillColor = new ARGB(fillA, fillR, fillG, fillB);

                UpdateFilteredColor();

                // Set the best settings possible (quality-wise)
                g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.AntiAlias;
                g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBilinear;
                g.PixelOffsetMode = System.Drawing.Drawing2D.PixelOffsetMode.HighQuality;
                g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

                //  g.DrawImage(image, (int)0, (int)0, (int)this.Width, (int)this.Height);
                g.DrawImage(filteredImage, (int)0, (int)0, this.WidgetData.width, this.WidgetData.height);
            }

            base.OnPaint(e);
        }

        public void UpdateFilteredColor()
        {

            if ((image != null) && (fillColorFilter != null))
            {
                Bitmap tmp = image.Clone(new Rectangle(0, 0
                    , image.Width, image.Height), image.PixelFormat);

                try
                {
                    filteredImage = fillColorFilter.Apply(tmp);
                }
                catch (Exception)
                {
                }

                // release temp image
                tmp.Dispose();
            }

        }

        public override void ShowWizard(Form parentForm)
        {
            base.ShowWizard(parentForm);
        }

        protected void PictureImangeChanged(string imageResourceName)
        {
            this.image = CreateImage(imageResourceName);
            Invalidate();
        }

        public bool IsImageConfigured()
        {
            return image != null;
        }

        public void SetImage(String imageResourceName)
        {
            this.picturePath = @imageResourceName;
            image = CreateImage(imageResourceName);

            //this.Width = image.Width;
            //this.Height = image.Height;
        }

        public override void UserDefinedPropertyModified(string name, string newValue)
        {
            if (name.Equals(PROPERTY_NAME_PICTURE_IMAGE))
            {

                this.picturePath = newValue;
                this.SetImage(this.picturePath);
            }
            else
            {
                base.UserDefinedPropertyModified(name, newValue);
            }

            Invalidate();
        }

        public override PropertyInfo[] GetPropertyInfoList()
        {
            return new PropertyInfo[]
            {
                PROPERTY_PICTURE_IMAGE
            };
        }


    }
}
