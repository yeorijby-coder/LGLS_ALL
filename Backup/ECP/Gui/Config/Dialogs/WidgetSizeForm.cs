using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using ECP.Gui.Widget;

namespace ECP.Gui.Config.Dialogs
{
    public partial class WidgetSizeForm : Form
    {
        List<BaseWidget> selections = null;

        public WidgetSizeForm()
        {
            InitializeComponent();
            this.CenterToScreen();
        }

        public WidgetSizeForm(List<BaseWidget> selections )
        {
            InitializeComponent();
            this.CenterToScreen();

            this.selections = selections;
            foreach (BaseWidget w in this.selections)
            {                
                this.textBoxWidth.Text = w.WidgetData.width.ToString();
                this.textBoxHeight.Text = w.WidgetData.height.ToString();
                break;
            }
            
        }

        private void buttonOk_Click(object sender, EventArgs e)
        {
            AdjustWidgetsSize();
            this.Close();
        }

        private void AdjustWidgetsSize()
        {
            foreach (BaseWidget w in this.selections)
            {
                w.Width = Convert.ToInt32(this.textBoxWidth.Text);
                w.WidgetData.width = Convert.ToInt32(this.textBoxWidth.Text);

                w.Height = Convert.ToInt32(this.textBoxHeight.Text);
                w.WidgetData.height = Convert.ToInt32(this.textBoxHeight.Text);

                w.Invalidate();
            }
        }

        private void buttonApply_Click(object sender, EventArgs e)
        {
            AdjustWidgetsSize();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}