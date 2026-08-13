using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Gui.Widget;
using ECP.Global.Gui;

namespace ECP.Gui.Config.Dialogs.Property
{
    public partial class TextPropertyDialog : Form
    {
        TextWidget textWidget = null;

        bool wereChanged = false;

        Font theRequestedFont = null;
        Color theRequestedFontColor = Color.Black;
        Color theRequestedBGColor = SystemColors.Control;
        string theRequestedText = null;
        int theRequestedCornerMargin = 0;

        public TextPropertyDialog()
        {
            InitializeComponent();
        }

        public TextPropertyDialog(TextWidget textWidget )
        {
            InitializeComponent();
            
            this.textWidget = textWidget;

            //text
            this.textContentTextBox.Text = textWidget.WidgetText;
            this.theRequestedText = textWidget.WidgetText;

            //font family, size ,style
            Font cfont = textWidget.WidgetTextFont;

            this.theRequestedFont = cfont;
            this.textFontTextBox.Text = cfont.FontFamily.Name.TrimEnd() + ","+
                                         cfont.Size.ToString() + "," +
                                          cfont.Style.ToString();
            // back color
            this.bgColorTextBox.BackColor=  textWidget.WidgetBackColor;
            this.theRequestedBGColor = textWidget.WidgetBackColor;
            
            // text color
            this.fontColorTextBox.BackColor = textWidget.WidgetTextColor;
            this.theRequestedFontColor = textWidget.WidgetTextColor;
            
            // margin
            this.cornerMarginTextBox.Text = Convert.ToString(textWidget.WidgetTextCornerMargin);
            this.theRequestedCornerMargin = textWidget.WidgetTextCornerMargin;

        }

        private void fontChooser_Click(object sender, EventArgs e)
        {
            fontDialog.Font = this.theRequestedFont;

            this.textFontTextBox.Text = theRequestedFont.FontFamily.Name.TrimEnd() + "," +
                                             theRequestedFont.Size.ToString() + "," +
                                              theRequestedFont.Style.ToString();
       
            if (fontDialog.ShowDialog() != DialogResult.Cancel)
            {
                //font family, size ,style
                Font cfont = fontDialog.Font;
                this.textFontTextBox.Text = cfont.FontFamily.Name.TrimEnd() + "," +
                                             cfont.Size.ToString() + "," +
                                              cfont.Style.ToString();
                theRequestedFont = cfont;

            }
        }

        private void fontColorChooser_Click(object sender, EventArgs e)
        {
            fontColorDialog.Color = this.theRequestedFontColor;
            this.fontColorTextBox.BackColor = theRequestedFontColor;

            if (fontColorDialog.ShowDialog() != DialogResult.Cancel)
            {
                this.fontColorTextBox.BackColor = fontColorDialog.Color;
                this.theRequestedFontColor = fontColorDialog.Color;   
            }
        }

        private void BGColorChooser_Click(object sender, EventArgs e)
        {
            bgColorDialog.Color = this.theRequestedBGColor;
            this.bgColorTextBox.BackColor = this.theRequestedBGColor;

            if (bgColorDialog.ShowDialog() != DialogResult.Cancel)
            {
                this.bgColorTextBox.BackColor =  bgColorDialog.Color;
                this.theRequestedBGColor = bgColorDialog.Color;
            }
        }

        private void cancelButton_Click(object sender, EventArgs e)
        {
            // do nothing
            this.Dispose();
            this.Close();
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            textWidget.WidgetText = this.textContentTextBox.Text;
            textWidget.WidgetBackColor = theRequestedBGColor;
            textWidget.WidgetTextFont = theRequestedFont;
            textWidget.WidgetTextColor = theRequestedFontColor;
            textWidget.WidgetTextCornerMargin = Convert.ToInt32( cornerMarginTextBox.Text);

            textWidget.Invalidate();

            this.Dispose();
            this.Close();            
        }
    }
}