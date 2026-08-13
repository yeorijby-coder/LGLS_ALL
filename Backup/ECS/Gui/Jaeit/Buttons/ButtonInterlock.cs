using System;
using System.ComponentModel;
using System.Collections.Generic;
using System.Diagnostics;
using System.Text;
using System.Drawing;
using System.Drawing.Drawing2D;
using ECP.Gui.Widget;
using System.Windows.Forms;

namespace HECS.Gui.Jaeit.Buttons
{
    public partial class ButtonInterlock : BaseWidget
    {

        private ButtonXP button = null;
        public enum INTERLOCK_LAMP_POSITION
        {
            TOP = 1,
            LEFT = 2,
            RIGHT = 3,
            BOTTOM = 4
        }
        private Font captionFont = new Font("Arial", 9, FontStyle.Bold);

        public Font CaptionFont
        {
            get
            {
                return captionFont;
            }
            set
            {
                captionFont = value;
                Invalidate();
            }
        }

        private string caption = "Button";

        public string Caption
        {
            get
            {
                return caption;
            }
            set
            {
                caption = value;
                Invalidate();
            }
        }

        private bool isInterlockLampVisible = false;

        public bool IsInterlockLampVisible
        {
            get
            {
                return isInterlockLampVisible;
            }
            set
            {
                isInterlockLampVisible = value;
                Invalidate();
            }
        }
        private Color interlockLampColor = Color.Red;

        public Color InterlockLampColor
        {
            get
            {
                return interlockLampColor;
            }
            set
            {
                interlockLampColor = value;
                Invalidate();
            }
        }

        private INTERLOCK_LAMP_POSITION interlockLampPosition = INTERLOCK_LAMP_POSITION.LEFT;

        public INTERLOCK_LAMP_POSITION InterlockLampPosition
        {
            get
            {
                return interlockLampPosition;
            }
            set
            {
                interlockLampPosition = value;
                Invalidate();
            }
        }
        public ButtonInterlock()
        {
            InitializeComponent();
            button = new ButtonXP();
            button.BackColor = Color.Transparent;
            this.Controls.Add(button);
            button.Click += new EventHandler(ButtonClick);
        }

        void ButtonClick(object sender, EventArgs e)
        {
            this.OnClick(e);
        }

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;


            Rectangle lampRect;

            Point buttonLocation = new Point(0, 0);
            Size buttonSize = new Size(0, 0);
            Point lampLocation = new Point(0, 0);
            Size lampSize = new Size(0, 0);


            SolidBrush lampBrush = new SolidBrush(interlockLampColor);
            if (InterlockLampPosition == INTERLOCK_LAMP_POSITION.RIGHT || interlockLampPosition == INTERLOCK_LAMP_POSITION.BOTTOM)
            {
                buttonLocation.X = 0;
                buttonLocation.Y = 0;
            }
            else if (InterlockLampPosition == INTERLOCK_LAMP_POSITION.LEFT)
            {
                buttonLocation.X = 8;
                buttonLocation.Y = 0;
            }
            else if (InterlockLampPosition == INTERLOCK_LAMP_POSITION.TOP)
            {
                buttonLocation.X = 0;
                buttonLocation.Y = 8;
            }
            button.Location = buttonLocation;

            if (interlockLampPosition == INTERLOCK_LAMP_POSITION.LEFT || interlockLampPosition == INTERLOCK_LAMP_POSITION.RIGHT)
            {
                buttonSize = new Size(this.Width - 8, this.Height);
            }
            else
            {
                buttonSize = new Size(this.Width, this.Height - 8);
            }
            button.Size = buttonSize;
            button.Font = this.captionFont;
            button.Text = this.caption;






            if (interlockLampPosition == INTERLOCK_LAMP_POSITION.LEFT || interlockLampPosition == INTERLOCK_LAMP_POSITION.RIGHT)
            {
                lampSize = new Size(10, this.Height - 16);
            }
            else
            {
                lampSize = new Size(this.Width - 16, 10);
            }

            if (interlockLampPosition == INTERLOCK_LAMP_POSITION.RIGHT)
            {
                lampLocation = new Point(this.Width - 11, 8);
            }
            else if (interlockLampPosition == INTERLOCK_LAMP_POSITION.LEFT)
            {
                lampLocation = new Point(0, 8);
            }
            else if (interlockLampPosition == INTERLOCK_LAMP_POSITION.TOP)
            {
                lampLocation = new Point(8, 0);
            }
            else if (interlockLampPosition == INTERLOCK_LAMP_POSITION.BOTTOM)
            {
                lampLocation = new Point(8, this.Height - 11);
            }
            if (isInterlockLampVisible == true)
            {
                lampRect = new Rectangle(lampLocation, lampSize);
                DrawRoundRect(g, lampRect, 2, lampBrush, Brushes.Black, 1);
            }
            
        }
    }

  

}
