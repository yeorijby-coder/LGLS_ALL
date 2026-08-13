using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Gui.Widget;

namespace ECP.Gui.Config.Dialogs.Property
{
    public partial class BaseWidgetPropertyDialog : Form
    {
        BaseWidget baseWidget = null;

        bool wereChanged = false;

        string theRequestedText = null;
        Font theRequestedTextFont = null;
        Color theRequestedTextColor = Color.Black;

        ContentAlignment theReqestedTextAlignment = ContentAlignment.MiddleCenter;        
        int theRequestedTextCornerMargin = 0;


        Color theRequestedFGColor = Color.Transparent;
        Color theRequestedBGColor = Color.Transparent;


        BorderStyle theRequestedBorderStyle = BorderStyle.None;

        string theRequestedImagePath = "";

        ImageLayout theRequestedBackGroundImageLayout = ImageLayout.None;


        public BaseWidgetPropertyDialog()
        {
            InitializeComponent();            
        }

        
        public BaseWidgetPropertyDialog(BaseWidget baseWidget)
        {
            InitializeComponent();


            this.baseWidget = baseWidget;
            

            // text
            this.textBoxTextContent.Text = baseWidget.WidgetText;
            this.theRequestedText = baseWidget.WidgetText;

       
            ////font family, size ,style  

            this.theRequestedTextFont = baseWidget.WidgetTextFont;
            this.textBoxFont.Text = theRequestedTextFont.FontFamily.Name.TrimEnd() + "," +
                                    theRequestedTextFont.Size.ToString() + "," +
                                    theRequestedTextFont.Style.ToString();

            // text color

            this.textBoxTextColor.BackColor = baseWidget.WidgetTextColor;
            this.theRequestedTextColor = baseWidget.WidgetTextColor;

            //baseWidget.WidgetTextAlignment


            foreach (object item in this.comboBoxTextAlign.Items)
            {
                string textAlign = (string)item;
                if (textAlign.Equals(baseWidget.WidgetTextAlignment.ToString()))
                {
                    this.comboBoxTextAlign.SelectedItem = item;
                    this.theReqestedTextAlignment = baseWidget.WidgetTextAlignment;
                    break;
                }
            }

            // auto size
            //if (baseWidget.AutoSizing)
            //{
            //    this.comboBoxAutoSize.SelectedIndex = 0;
            //}
            //else
            //{
            //    this.comboBoxAutoSize.SelectedIndex = 1;
            //}

            foreach (object item in this.comboBoxAutoSize.Items)
            {

                if (item.ToString().Equals(baseWidget.AutoSize.ToString()))
                {
                    this.comboBoxAutoSize.SelectedItem = item;
                    break;
                }
            }
            // margin

            this.numericUpDownTextCornerMargin.Value = baseWidget.WidgetTextCornerMargin;
            this.theRequestedTextCornerMargin = baseWidget.WidgetTextCornerMargin;



            // fore color

            if (baseWidget.WidgetForeColor != Color.FromArgb(0,255,255,255)&&
                baseWidget.WidgetForeColor != Color.Transparent)
            {
                this.textBoxFGColor.BackColor = baseWidget.WidgetForeColor;
                this.theRequestedFGColor = baseWidget.WidgetForeColor;
            }
            

            // back color

            if (baseWidget.WidgetBackColor != Color.FromArgb(0,255,255,255)&&
                baseWidget.WidgetBackColor != Color.Transparent)
            {
                this.textBoxBGColor.BackColor = baseWidget.WidgetBackColor;
                this.theRequestedBGColor = baseWidget.WidgetBackColor;
            }

       

            foreach (object item in this.comboBoxBorderStyle.Items)
            {
                string bs = (string)item;
                if (bs.Equals(baseWidget.WidgetBorderStyle.ToString()))
                {
                    this.comboBoxBorderStyle.SelectedItem = item;
                    this.theRequestedBorderStyle = baseWidget.WidgetBorderStyle;
                    break;
                }
            }

            this.textBoxImage.Text = baseWidget.WidgetBackGroundImage;
            this.theRequestedImagePath = baseWidget.WidgetBackGroundImage;
 
            foreach (object item in this.comboBoxImageLayout.Items)
            {
                string il = (string)item;
                if (il.Equals(baseWidget.WidgetBackGroundImageLayout.ToString()))
                {
                    this.comboBoxImageLayout.SelectedItem = item;
                    this.theRequestedBorderStyle = baseWidget.WidgetBorderStyle;
                    break;
                }
            }          
        }

        private void chooserFont_Click(object sender, EventArgs e)
        {
            fontDialog.Font = this.theRequestedTextFont;

            this.textBoxFont.Text = theRequestedTextFont.FontFamily.Name.TrimEnd() + "," +
                                             theRequestedTextFont.Size.ToString() + "," +
                                              theRequestedTextFont.Style.ToString();

            if (fontDialog.ShowDialog() != DialogResult.Cancel)
            {
                //font family, size ,style
                Font cfont = fontDialog.Font;
                this.textBoxFont.Text = cfont.FontFamily.Name.TrimEnd() + "," +
                                             cfont.Size.ToString() + "," +
                                              cfont.Style.ToString();
                this.theRequestedTextFont = cfont;
            }
        }

        private void ChooserFontColor_Click(object sender, EventArgs e)
        {

            colorDialogTextColor.Color = this.theRequestedTextColor;
            this.textBoxTextColor.BackColor = theRequestedTextColor;

            if (colorDialogTextColor.ShowDialog() != DialogResult.Cancel)
            {
                this.textBoxTextColor.BackColor = colorDialogTextColor.Color;
                this.theRequestedTextColor = colorDialogTextColor.Color;
            }
        }

        private void chooserFGColor_Click(object sender, EventArgs e)
        {
            colorDialogFGColor.Color = this.theRequestedFGColor;

            if(theRequestedFGColor != Color.Transparent)
                this.textBoxFGColor.BackColor = theRequestedFGColor;

            if (colorDialogFGColor.ShowDialog() != DialogResult.Cancel)
            {
                if (colorDialogFGColor.Color != Color.Transparent)
                    this.textBoxFGColor.BackColor = colorDialogFGColor.Color;
                this.theRequestedFGColor = colorDialogFGColor.Color;
            }
        }

        private void chooserBGColor_Click(object sender, EventArgs e)
        {
            colorDialogBGColor.Color = this.theRequestedBGColor;

            if (theRequestedBGColor != Color.Transparent)
                this.textBoxBGColor.BackColor = theRequestedBGColor;

            if (colorDialogBGColor.ShowDialog() != DialogResult.Cancel)
            {
                if (colorDialogBGColor.Color != Color.Transparent)
                    this.textBoxBGColor.BackColor = colorDialogBGColor.Color;
                this.theRequestedBGColor = colorDialogBGColor.Color;
            }
        }

        private void chooserImage_Click(object sender, EventArgs e)
        {

            this.imageChooser.FileName = this.theRequestedImagePath;
            this.textBoxImage.Text = this.theRequestedImagePath;

            if (imageChooser.ShowDialog() != DialogResult.Cancel)
            {
                this.textBoxImage.Text = this.imageChooser.FileName;
                this.theRequestedImagePath = this.imageChooser.FileName;
            }
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            // do nothing
            this.Dispose();
            this.Close();
        }

        protected virtual void buttonOK_Click(object sender, EventArgs e)
        {
            this.buttonApply_Click(sender, e);

            this.Dispose();
            this.Close();
        }       

       

        private void textBoxTextContent_KeyUp(object sender, KeyEventArgs e)
        {
            this.theRequestedText = textBoxTextContent.Text;
        }      

        protected virtual void buttonApply_Click(object sender, EventArgs e)
        {           

            baseWidget.WidgetBackColor = this.theRequestedBGColor;
            baseWidget.WidgetBackGroundImage = this.theRequestedImagePath;


            string item = (string)comboBoxImageLayout.SelectedItem;

            if (item != null)
            {
                if (item.Equals(ImageLayout.Center.ToString()))
                {
                    baseWidget.WidgetBackGroundImageLayout = ImageLayout.Center;
                }
                else if (item.Equals(ImageLayout.None.ToString()))
                {
                    baseWidget.WidgetBackGroundImageLayout = ImageLayout.None;
                }
                else if (item.Equals(ImageLayout.Stretch.ToString()))
                {
                    baseWidget.WidgetBackGroundImageLayout = ImageLayout.Stretch;
                }
                else if (item.Equals(ImageLayout.Tile.ToString()))
                {
                    baseWidget.WidgetBackGroundImageLayout = ImageLayout.Tile;
                }
                else if (item.Equals(ImageLayout.Zoom.ToString()))
                {
                    baseWidget.WidgetBackGroundImageLayout = ImageLayout.Zoom;
                }
            }

            item = (string)comboBoxBorderStyle.SelectedItem;

            if (item != null)
            {
                if (item.Equals(BorderStyle.Fixed3D.ToString()))
                {
                    baseWidget.WidgetBorderStyle = BorderStyle.Fixed3D;
                }
                else if (item.Equals(BorderStyle.FixedSingle.ToString()))
                {
                    baseWidget.WidgetBorderStyle = BorderStyle.FixedSingle;
                }
                else if (item.Equals(BorderStyle.None.ToString()))
                {
                    baseWidget.WidgetBorderStyle = BorderStyle.None;
                }
            }

            
            baseWidget.WidgetForeColor = this.theRequestedFGColor;

            item = (string)comboBoxAutoSize.SelectedItem;
            if (item != null)
            {
                if (item.Equals("True"))
                {
                    baseWidget.AutoSize = true;
                }
                else
                {
                    baseWidget.AutoSize = false;
                }
            }

            baseWidget.WidgetText = this.theRequestedText;

            item = (string)comboBoxTextAlign.SelectedItem;
            if (item != null)
            {
                if (item.Equals(ContentAlignment.BottomCenter.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.BottomCenter;
                }
                else if (item.Equals(ContentAlignment.BottomLeft.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.BottomLeft;
                }
                else if (item.Equals(ContentAlignment.BottomRight.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.BottomRight;
                }
                else if (item.Equals(ContentAlignment.MiddleCenter.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.MiddleCenter;
                }
                else if (item.Equals(ContentAlignment.MiddleLeft.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.MiddleLeft;
                }
                else if (item.Equals(ContentAlignment.MiddleRight.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.MiddleRight;
                }
                else if (item.Equals(ContentAlignment.TopCenter.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.TopCenter;
                }
                else if (item.Equals(ContentAlignment.TopLeft.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.TopLeft;
                }
                else if (item.Equals(ContentAlignment.TopRight.ToString()))
                {
                    baseWidget.WidgetTextAlignment = ContentAlignment.TopRight;
                }

            }


            baseWidget.WidgetTextColor = this.theRequestedTextColor;

            //baseWidget.AutoSizing = true;  
            baseWidget.WidgetTextCornerMargin = this.theRequestedTextCornerMargin;

            //baseWidget.AutoSizing = 
            baseWidget.WidgetTextFont = this.theRequestedTextFont;
            
            baseWidget.Invalidate();
        }

        private void numericUpDownTextCornerMargin_ValueChanged(object sender, EventArgs e)
        {
            this.theRequestedTextCornerMargin = (int)this.numericUpDownTextCornerMargin.Value;
        }        
    }
}