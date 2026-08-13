using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using ECP.Global.Gui;
using System.Drawing.Drawing2D;

using ECP.Global;

namespace ECP.Gui.Widget
{
    public partial class BaseWidget : UserControl
    {

        #region Custom Property 관련

        public const string PROPERTY_NAME_TEXT = "Text";

        public const string PROPERTY_NAME_TEXT_FONT = "FontFamily";

        public const string PROPERTY_NAME_TEXT_COLOR = "TextColor";

        public const string PROPERTY_NAME_TEXT_ALIGN = "TextAlign";

        public const string PROPERTY_NAME_TEXT_MARGIN_SIZE = "Margin";

        public const string PROPERTY_NAME_FORE_COLOR = "ForeGroundColor";
    
        public const string PROPERTY_NAME_BACK_COLOR = "BackGroundColor";

        public const string PROPERTY_NAME_BORDER_STYLE = "BorderStyle";

        public const string PROPERTY_NAME_BACKGROUND_IMAGE = "BackgroundImage";

        public const string PROPERTY_NAME_BACKGROUND_IMAGE_LAYOUT = "BackgroundImageLayout"; 

        



        protected static PropertyInfo PROPERTY_TEXT =
            new PropertyInfo(PROPERTY_NAME_TEXT, PropertyInfo.PROPERTY_TYPE_STRING, "Blank");

        public static PropertyInfo PROPERTY_TEXT_FONT =
            new PropertyInfo(PROPERTY_NAME_TEXT_FONT, PropertyInfo.PROPERTY_TYPE_STRING, "Helvetica");

        protected static PropertyInfo PROPERTY_TEXT_COLOR =
            new PropertyInfo(PROPERTY_NAME_TEXT_COLOR,
                             PropertyInfo.PROPERTY_TYPE_COLOR,
                             Convert.ToString(Color.Black.ToArgb()));

        protected static PropertyInfo PROPERTY_TEXT_ALIGN =
            new PropertyInfo(PROPERTY_NAME_TEXT_ALIGN,
                            PropertyInfo.PROPERTY_TYPE_STRING,
                            Convert.ToString(ContentAlignment.TopLeft.ToString()));


        public static PropertyInfo PROPERTY_TEXT_MARGIN_SIZE =
            new PropertyInfo(PROPERTY_NAME_TEXT_MARGIN_SIZE, PropertyInfo.PROPERTY_TYPE_NUMBER, "O");


        public static PropertyInfo PROPERTY_FORE_COLOR =
            new PropertyInfo(PROPERTY_NAME_FORE_COLOR, 
                PropertyInfo.PROPERTY_TYPE_COLOR,
                Convert.ToString(Color.Transparent.ToArgb()));


        protected static PropertyInfo PROPERTY_BACK_COLOR =
            new PropertyInfo(PROPERTY_NAME_BACK_COLOR,
                            PropertyInfo.PROPERTY_TYPE_COLOR,
                            Convert.ToString(Color.Transparent.ToArgb()));

  
        protected static PropertyInfo PROPERTY_BORDER_STYLE =
            new PropertyInfo(PROPERTY_NAME_BORDER_STYLE,
                            PropertyInfo.PROPERTY_TYPE_STRING,
                            Convert.ToString(BorderStyle.None.ToString()));


        protected static PropertyInfo PROPERTY_BACKGROUND_IMAGE =
            new PropertyInfo(PROPERTY_NAME_BACKGROUND_IMAGE, PropertyInfo.PROPERTY_TYPE_STRING, "");

        protected static PropertyInfo PROPERTY_BACKGROUND_IMAGE_LAYOUT =
           new PropertyInfo(PROPERTY_NAME_BACKGROUND_IMAGE_LAYOUT,
                             PropertyInfo.PROPERTY_TYPE_STRING,
                             Convert.ToString(ImageLayout.None.ToString()));

        private string widgetText = "";

        public string WidgetText
        {
            get
            {
                return widgetText;
            }
            set
            {
                widgetText = value;
                this.SetProperty(PROPERTY_NAME_TEXT, this.widgetText);
            }
        }

        public override Font Font
        {
            get
            {
                return this.widgetTextFont;
            }
            set
            {
                this.WidgetTextFont = value;
                base.Font = value;
            }
        }
        private Font widgetTextFont = new Font("Helvetica", 9, FontStyle.Regular);

        public Font WidgetTextFont
        {
            get
            {
                return widgetTextFont;
            }
            set
            {
                widgetTextFont = value;
      

                FontConverter fontConverter = new FontConverter();
                string fontString =  fontConverter.ConvertToString(this.widgetTextFont);
                this.SetProperty(PROPERTY_NAME_TEXT_FONT, fontString);
            }
        }

        private Color widgetTextColor = Color.Black;

        public Color WidgetTextColor
        {
            get
            {
                return widgetTextColor;
            }
            set
            {
                widgetTextColor = value;
                this.SetProperty(PROPERTY_NAME_TEXT_COLOR, Convert.ToString(this.WidgetTextColor.ToArgb().ToString()));
            }
        }


        private int widgetTextCornerMargin = 0;

        public int WidgetTextCornerMargin
        {
            get
            {
                return widgetTextCornerMargin;
            }
            set
            {
                widgetTextCornerMargin = value;
        
                this.SetProperty(PROPERTY_NAME_TEXT_MARGIN_SIZE, Convert.ToString(this.widgetTextCornerMargin));
            }
        }


        private ContentAlignment widgetTextAlignment = ContentAlignment.TopLeft;

        public ContentAlignment WidgetTextAlignment
        {
            get
            {
                return widgetTextAlignment;
            }
            set
            {
                widgetTextAlignment = value;
                this.SetProperty(PROPERTY_NAME_TEXT_ALIGN, widgetTextAlignment.ToString());
            }
        }


        private Color widgetForeColor = Color.Transparent;

        public Color WidgetForeColor
        {
            get { return widgetForeColor; }
            set
            {
                widgetForeColor = value;
                //this.ForeColor = widgetForeColor;
                this.SetProperty(PROPERTY_NAME_FORE_COLOR,
                    Convert.ToString(widgetForeColor.ToArgb()).ToString());

            }
        }


        private Color widgetBackColor = Color.Transparent;
        public Color WidgetBackColor
        {
            get
            {
                return widgetBackColor;
            }
            set
            {
                widgetBackColor = value;
                //this.BackColor = widgetBackColor;
                this.SetProperty(PROPERTY_NAME_BACK_COLOR, Convert.ToString(this.widgetBackColor.ToArgb().ToString()));
            }
        }

        private BorderStyle widgetBoarderStyle = BorderStyle.None;

        public BorderStyle WidgetBorderStyle
        {
            get
            {
                return widgetBoarderStyle;
            }
            set
            {
                widgetBoarderStyle = value;
                this.SetProperty(PROPERTY_NAME_BORDER_STYLE, widgetBoarderStyle.ToString());
            }
        }




        private string widgetBackGroundImage = "";

        public string WidgetBackGroundImage
        {
            get { return widgetBackGroundImage; }
            set 
            {
                widgetBackGroundImage = value;
                this.SetProperty(PROPERTY_NAME_BACKGROUND_IMAGE, widgetBackGroundImage);
            }
        }


        private ImageLayout widgetBackGroundImageLayout = ImageLayout.Stretch;

        public ImageLayout WidgetBackGroundImageLayout
        {
            get { return widgetBackGroundImageLayout; }
            set 
            { 
                widgetBackGroundImageLayout = value;
                this.SetProperty(PROPERTY_NAME_BACKGROUND_IMAGE_LAYOUT, widgetBackGroundImageLayout.ToString());
            }
        }

        public Font GetFont()
        {
           //// if (this.autoSizing)
           // {
           //     return base.Font;
           // }
           // else
            {
                return this.widgetTextFont;
            }
        }


        #endregion


        bool autoSizing = false;

        public bool AutoSizing
        {
            get { return autoSizing; }
            set { autoSizing = value; }
        }


        protected bool selected = false;

        public bool Selected
        {
            get
            {
                return selected;
            }
            set
            {
                selected = value;
                Invalidate();
            }
        }

        protected bool errorState = false;

        public bool ErrorState
        {
            get
            {
                return errorState;
            }
            set
            {
                errorState = value;
                if (this.Visible == true)
                    Invalidate();
            }
        }

        protected WidgetData widgetData = new WidgetData();

        virtual public WidgetData WidgetData
        {
            get
            {
                return widgetData;
            }
            set
            {
                widgetData = value;
                this.ElementId = widgetData.elementId;
                //this.SetBounds(widgetData.x, widgetData.y, widgetData.width, widgetData.height);            
                InitializeWidgetProperties();

            }
        }

        public void InitializeWidgetProperties()
        {
            if (widgetData.properties == null)
            {
                widgetData.properties = new UserDefinedPropertyInfo[0];
            }

            PropertyInfo[] supportedProperties = GetPropertyInfoList();

            // do initialize property as defalt value
            for (int i = 0; i < supportedProperties.Length; i++)
            {
                PropertyInfo p = supportedProperties[i];

                int found = -1;

                for (int j = 0; j < widgetData.properties.Length; j++)
                {
                    if (p.Name.Equals(widgetData.properties[j].name))
                    {
                        found = j;
                    }
                }

                if (found > -1)
                {
                    SetProperty(widgetData.properties[found].name, (widgetData.properties[found].value));
                }
                else
                {
                    SetProperty(p.Name, p.DefaultValue);
                }
            }

            for (int i = 0; i < widgetData.properties.Length; i++)
            {
                UserDefinedPropertyInfo info = widgetData.properties[i];
                UserDefinedPropertyModified(info.name, info.value);
            }
        }

        public string ElementId
        {
            get
            {
                return widgetData.elementId;
            }
            set
            {
                widgetData.elementId = value;
            }
        }

        public BaseWidget()
        {
            InitializeComponent();
        }


        public virtual void Subscribe()
        {
        }

        public virtual void UnSubscribe()
        {
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            base.OnPaint(e);
            this.HighlightForSelection(e);
        }

        public void HighlightForSelection(PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            // Draw a box to indicate selection
            if (selected)
            {

                Rectangle cb = this.GetConfigBounds();
                Rectangle b = Bounds;
                int x = cb.X - b.X;
                int y = cb.Y - b.Y;
                int w = cb.Width - 1;
                int h = cb.Height - 1; //BorderStyle.None
                if (this.BorderStyle == BorderStyle.FixedSingle)
                {
                    w = cb.Width - 3; 
                    h = cb.Height - 3;
                }
                else if (this.BorderStyle == BorderStyle.Fixed3D)
                {
                    w = cb.Width - 5;
                    h = cb.Height - 5;
                }

                g.DrawRectangle(new Pen(Color.Red), x, y, w, h);
                g.DrawRectangle(new Pen(Color.Red), x + 1, y + 1, w - 2, h - 2);
            }
        }

        public Rectangle GetConfigBounds()
        {
            return this.Bounds;
        }

        public static WidgetData CloneWidgetData(WidgetData data)
        {
            WidgetData d2 = (WidgetData)data.Clone();

            d2.properties = new UserDefinedPropertyInfo[data.properties.Length];

            for (int i = 0; i < data.properties.Length; i++)
            {
                d2.properties[i] = (UserDefinedPropertyInfo)data.properties[i].Clone();
            }

            return d2;
        }

        public virtual void ShowWizard(Form parentForm)
        {
            new ECP.Gui.Config.Dialogs.ElementWizardForm(this, this.ElementId).Show();
        }

        public virtual void ShowWizard(Form parentForm, string[] elementIds)
        {
            //new ECP.Gui.Config.Dialogs.ElementWizardForm(this, this.ElementId).Show();
        }

        public virtual void ShowProperty()
        {
            new ECP.Gui.Config.Dialogs.Property.BaseWidgetPropertyDialog(this).Show();
        }


        public virtual void ShowUDProperty()
        {
            //
        }

        public virtual void SetElementIDAtDesignMode(string elementId)
        {
            //080930 김성원 추가.. 하위단에서 구현함.
        }

        public static Bitmap CreateImage(string resource)
        {
            Bitmap image = null;
            try
            {
                image = new Bitmap(@resource);
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("CreateImage:" + ex.StackTrace);
                return null;
            }
            return image;
        }


        public virtual void UserDefinedPropertyModified(string name, string newValue)
        {
            if (name.Equals(PROPERTY_NAME_TEXT))
            {
                this.widgetText = newValue;
                AdjustTextBounds();
            }
            else if (name.Equals(PROPERTY_NAME_TEXT_FONT))
            {
                FontConverter fontConverter = new FontConverter();

                try
                {
                    this.widgetTextFont = (Font)fontConverter.ConvertFromString(newValue);
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("UserDefinedPropertyModified:" + ex.StackTrace);
                        this.widgetTextFont = new Font("Helvetica", 9, FontStyle.Regular);
                }
                AdjustTextBounds();

            }
            else if (name.Equals(PROPERTY_NAME_TEXT_COLOR))
            {
                int colorInt = 0;
                try
                {
                    if (int.TryParse(newValue, out colorInt))
                    {
                        colorInt = Convert.ToInt32(newValue);
                    }
                    else
                    {
                        ColorConverter colorConverter = new ColorConverter();
                        colorInt = ((Color)colorConverter.ConvertFromString(newValue)).ToArgb();
                    }
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("UserDefinedPropertyModified:" + ex.StackTrace);
                    //ColorConverter colorConverter = new ColorConverter();
                    //try
                    //{
                    //    colorInt = ((Color)colorConverter.ConvertFromString(newValue)).ToArgb();
                    //}
                    //catch (Exception)
                    //{
                        colorInt = Color.Black.ToArgb();
                    //}
                }
                this.widgetTextColor = Color.FromArgb(colorInt);

            }
            else if (name.Equals(PROPERTY_NAME_TEXT_MARGIN_SIZE))
            {
                int size = 0;
                try
                {
                    size = Convert.ToInt32(newValue);
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("UserDefinedPropertyModified:" + ex.StackTrace);
                    // This is ok, it just means to turn it off
                }

                if (size > 0)
                {
                    this.widgetTextCornerMargin = size;
                    AdjustTextBounds();
                }
                else
                {
                    // this.autoSizing = true;
                }
            }
            else if (name.Equals(PROPERTY_NAME_BACK_COLOR))
            {
                int colorInt;
                try
                {
                    if (int.TryParse(newValue, out colorInt))
                    {
                        widgetBackColor = Color.FromArgb(Convert.ToInt32(newValue));
                        //this.BackColor = widgetBackColor;
                    }
                    else
                    {
                        ColorConverter colorConverter = new ColorConverter();
                        widgetBackColor = (Color)colorConverter.ConvertFromString(newValue);
                        //this.BackColor = widgetBackColor;
                    }

                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("UserDefinedPropertyModified:" + ex.StackTrace);
                    //ColorConverter colorConverter = new ColorConverter();
                    //try
                    //{
                    //    widgetBackColor = (Color)colorConverter.ConvertFromString(newValue);
                    //    this.BackColor = widgetBackColor;
                    //}
                    //catch (Exception)
                    //{
                        widgetBackColor = Color.Transparent;
                        //this.BackColor = widgetBackColor;
                    //}
                }
            }
            else if (name.Equals(PROPERTY_NAME_FORE_COLOR))
            {
                int colorInt;
                
                ColorConverter colorConverter = new ColorConverter();

                try
                {
                    if (int.TryParse(newValue, out colorInt))
                    {
                        this.widgetForeColor = Color.FromArgb(Convert.ToInt32(newValue));
                        //this.ForeColor = (Color)colorConverter.ConvertFromString(newValue);
                    }
                    else
                    {
                        widgetForeColor = (Color)colorConverter.ConvertFromString(newValue);
                        //this.ForeColor = widgetForeColor;
                    }
                }
                catch (Exception ex)
                {
                    System.Console.WriteLine("UserDefinedPropertyModified:" + ex.StackTrace);
                    //try
                    //{
                    //    widgetForeColor = (Color)colorConverter.ConvertFromString(newValue);
                    //    this.ForeColor = widgetForeColor;
                    //}
                    //catch (Exception)
                    //{
                        widgetForeColor = Color.Transparent;
                        //this.ForeColor = widgetForeColor;
                    //}
                }
            }
            else if (name.Equals(PROPERTY_NAME_BORDER_STYLE))
            {
                EnumConverter enumConverter = new EnumConverter(typeof(BorderStyle));
                try
                {
                    this.widgetBoarderStyle = (BorderStyle)enumConverter.ConvertFromString(newValue);
                }
                catch (Exception)
                {
                    this.widgetBoarderStyle = BorderStyle.None;
                }
            }
            else if (name.Equals(PROPERTY_NAME_TEXT_ALIGN))
            {
                EnumConverter enumConverter = new EnumConverter(typeof(ContentAlignment));
                try
                {
                    this.widgetTextAlignment = (ContentAlignment)enumConverter.ConvertFromString(newValue);
                }
                catch (Exception)
                {
                    this.widgetTextAlignment = ContentAlignment.TopLeft;
                }
            }
            else if (name.Equals(PROPERTY_NAME_BACKGROUND_IMAGE_LAYOUT))
            {
                EnumConverter enumConverter = new EnumConverter(typeof(ImageLayout));
                try
                {
                    this.widgetBackGroundImageLayout = (ImageLayout)enumConverter.ConvertFromString(newValue);
                    this.BackgroundImageLayout = (ImageLayout)enumConverter.ConvertFromString(newValue);
                }
                catch (Exception)
                {
                    this.BackgroundImageLayout = ImageLayout.None;
                }
            }
            else if (name.Equals(PROPERTY_NAME_BACKGROUND_IMAGE))
            {
                try
                {
                    this.widgetBackGroundImage = newValue;
                    if (newValue == null || newValue.Equals(""))
                    {
                        this.BackgroundImage = null;
                    }
                    else
                    {
                        this.BackgroundImage = (Image)SetImage(newValue);
                    }
                }
                catch (Exception)
                {
                    this.BackgroundImage = null;
                }
            }
            else
            {
                // UserDefinedPropertyModified (name, newValue);
            }

            Invalidate();
        }
        public Bitmap SetImage(String imageResourceName)
        {
            Bitmap image = null;
            try
            {
                image = CreateImage(imageResourceName);
            }

            catch (Exception ex)
            {
                System.Console.WriteLine("SetImage:" + ex.StackTrace);
                image = null;
            }
            return image;
        }
        public void AdjustTextBounds()
        {
            if (this is TextWidget)
            {
                if (autoSizing)
                {
                    Size stringSize = TextRenderer.MeasureText(this.widgetText, this.widgetTextFont);

                    SetBounds(this.Location.X, this.Location.Y,
                        stringSize.Width + widgetTextCornerMargin * 2,
                        stringSize.Height + widgetTextCornerMargin * 2);

                    this.widgetData.x = this.Location.X;
                    this.widgetData.y = this.Location.Y;
                    this.widgetData.width = stringSize.Width + widgetTextCornerMargin * 2;
                    this.widgetData.height = stringSize.Height + widgetTextCornerMargin * 2;
                    this.autoSizing = false;
                }
            }
        }



        public virtual PropertyInfo[] GetPropertyInfoList()
        {
            return new PropertyInfo[]
            {
                PROPERTY_TEXT,//
                PROPERTY_TEXT_COLOR,//
                PROPERTY_TEXT_FONT,
                PROPERTY_BACK_COLOR,//
                PROPERTY_TEXT_MARGIN_SIZE,
                PROPERTY_FORE_COLOR,
                PROPERTY_BORDER_STYLE,
                PROPERTY_TEXT_ALIGN,
                PROPERTY_BACKGROUND_IMAGE,
                PROPERTY_BACKGROUND_IMAGE_LAYOUT
            };
        }
        public void SetProperty(string name, string value)
        {
            if (SetPropertyDontNotify(name, value))
            {
                UserDefinedPropertyModified(name, value);
            }
        }


        public void RemoveProperty(string name)
        {
            int index = -1;
            UserDefinedPropertyInfo[] oldProps = this.widgetData.properties;

            for (int i = 0; i < oldProps.Length; i++)
            {
                if (oldProps[i].name.Equals(name))
                {
                    index = i;
                    break;
                }
            }
            if (index != -1)
            {
                UserDefinedPropertyInfo[] newProps =
                    new UserDefinedPropertyInfo[oldProps.Length - 1];

                // Collapse on the proper index
                if (index > 0)
                {
                    System.Array.Copy(oldProps, 0, newProps, 0, index);
                }
                if (index < oldProps.Length - 1)
                {

                    System.Array.Copy(oldProps, index + 1, newProps, index,
                        oldProps.Length - 1 - index);
                }

                this.widgetData.properties = newProps;
            }
        }

        public string GetProperty(string name)
        {

            // Make sure the list has been initialized

            if (this.widgetData.properties == null)
            {
                this.widgetData.properties = new UserDefinedPropertyInfo[0];
            }

            // Search through to find it

            for (int i = 0; i < this.widgetData.properties.Length; i++)
            {
                if (this.widgetData.properties[i].name.Equals(name))
                {
                    return this.widgetData.properties[i].value;
                }
            }

            // Didn't find it, so it is blank
            return "";
        }


        protected bool SetPropertyDontNotify(string name, string value)
        {
            if (this.widgetData.properties == null)
            {
                this.widgetData.properties = new UserDefinedPropertyInfo[0];

            }

            int blankProperty = -1;

            for (int i = 0; i < this.widgetData.properties.Length; i++)
            {
                if (this.widgetData.properties[i].name.Equals(name))
                {
                    bool changed =
                        !this.widgetData.properties[i].value.Equals(value);

                    this.widgetData.properties[i].value = value;

                    return changed;
                }
                else if (this.widgetData.properties[i].value.Length == 0)
                {
                    // Remember this guy so he can be reused if needed
                    blankProperty = i;
                }
            }

            // Didn't find it, try to reuse a property's spot that has been cleared
            if (blankProperty >= 0)
            {
                this.widgetData.properties[blankProperty] =
                    new UserDefinedPropertyInfo(name, value);
            }
            else
            {
                // tack one on onto the end
                UserDefinedPropertyInfo[] oldProps = this.widgetData.properties;
                UserDefinedPropertyInfo[] newProps =
                    new UserDefinedPropertyInfo[oldProps.Length + 1];

                for (int i = 0; i < oldProps.Length; i++)
                {
                    newProps[i] = oldProps[i];
                }

                newProps[newProps.Length - 1] = new UserDefinedPropertyInfo(name, value);

                this.widgetData.properties = newProps;
            }

            // If we didn't have the property it is always a change
            return true;
        }




        //***************************************************//
        // GDI Utility Should be Removed .. Soon             //
        //***************************************************//

        protected void DrawRect(Graphics g, Rectangle rect, Brush drawColor, Brush fillColor)
        {
            int penWidth = 1;

            int X = rect.X;
            int Y = rect.Y;
            int width = rect.Width;
            int height = rect.Height;

            g.FillRectangle(drawColor, new Rectangle(X + (penWidth / 2), Y + (penWidth / 2), width - penWidth, height - penWidth));
            Pen p = new Pen(fillColor, penWidth);
            g.DrawRectangle(p, new Rectangle(X + (penWidth / 2), Y + (penWidth / 2), width - penWidth, height - penWidth));

        }
        protected void DrawRoundRect(Graphics g, Rectangle rect, int radius, Brush drawColor, Brush fillColor, int penWidth)
        {
            //int penWidth = 2;

            int X = rect.X;
            int Y = rect.Y;
            int width = rect.Width;
            int height = rect.Height;

            Pen p = new Pen(fillColor, penWidth);   //랙마
            g.FillRectangle(drawColor, new Rectangle(X + (penWidth / 2), Y + (penWidth / 2), width - penWidth, height - penWidth));

            GraphicsPath gp = new GraphicsPath();
            gp.AddLine(X + radius, Y, X + width - (radius * 2), Y);
            gp.AddArc(X + width - (radius * 2), Y, radius * 2, radius * 2, 270, 90);
            gp.AddLine(X + width, Y + radius, X + width, Y + height - (radius * 2));
            gp.AddArc(X + width - (radius * 2), Y + height - (radius * 2), radius * 2, radius * 2, 0, 90);
            gp.AddLine(X + width - (radius * 2), Y + height, X + radius, Y + height);
            gp.AddArc(X, Y + height - (radius * 2), radius * 2, radius * 2, 90, 90);
            gp.AddLine(X, Y + height - (radius * 2), X, Y + radius);
            gp.AddArc(X, Y, radius * 2, radius * 2, 180, 90);
            gp.CloseFigure();
            g.DrawPath(p, gp);
            gp.Dispose();
        }

        protected static GraphicsPath CreateRoundRectangle(Rectangle rectangle, int radius)
        {
            GraphicsPath path = new GraphicsPath();
            int l = rectangle.Left;
            int t = rectangle.Top;
            int w = rectangle.Width;
            int h = rectangle.Height;
            int d = radius << 1;
            path.AddArc(l, t, d, d, 180, 90); // topleft
            path.AddLine(l + radius, t, l + w - radius, t); // top
            path.AddArc(l + w - d, t, d, d, 270, 90); // topright
            path.AddLine(l + w, t + radius, l + w, t + h - radius); // right
            path.AddArc(l + w - d, t + h - d, d, d, 0, 90); // bottomright
            path.AddLine(l + w - radius, t + h, l + radius, t + h); // bottom
            path.AddArc(l, t + h - d, d, d, 90, 90); // bottomleft
            path.AddLine(l, t + h - radius, l, t + radius); // left
            path.CloseFigure();
            return path;
        }

        protected static GraphicsPath CreateTopRoundRectangle(Rectangle rectangle, int radius)
        {
            GraphicsPath path = new GraphicsPath();
            int l = rectangle.Left;
            int t = rectangle.Top;
            int w = rectangle.Width;
            int h = rectangle.Height;
            int d = radius << 1;
            path.AddArc(l, t, d, d, 180, 90); // topleft
            path.AddLine(l + radius, t, l + w - radius, t); // top
            path.AddArc(l + w - d, t, d, d, 270, 90); // topright
            path.AddLine(l + w, t + radius, l + w, t + h); // right
            path.AddLine(l + w, t + h, l, t + h); // bottom
            path.AddLine(l, t + h, l, t + radius); // left
            path.CloseFigure();
            return path;
        }

        protected static GraphicsPath CreateBottomRadialPath(Rectangle rectangle)
        {
            GraphicsPath path = new GraphicsPath();
            RectangleF rect = rectangle;
            rect.X -= rect.Width * .35f;
            rect.Y -= rect.Height * .15f;
            rect.Width *= 1.7f;
            rect.Height *= 2.3f;
            path.AddEllipse(rect);
            path.CloseFigure();
            return path;
        }
    }
}
