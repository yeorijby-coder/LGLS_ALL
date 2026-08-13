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
    public partial class SearchWidgetForm : Form
    {
        ECPConfigPanel parent = null;

        public SearchWidgetForm()
        {
            InitializeComponent();
            this.CenterToScreen();
        }

        public SearchWidgetForm(ECPConfigPanel parent)
        {
            InitializeComponent();
            this.CenterToScreen();

            this.parent = parent;
        }

        private void SerchElement(object sender, EventArgs e)
        {
            string elementID = this.textBoxSearchWidget.Text;

            //ECPConfigPanel의 ClearSelection()메소드가 private이라서 직접 코딩 
            for (int i = 0; i < parent.selection.Count; i++)
            {
                BaseWidget w = (BaseWidget)parent.selection[i];
                w.Selected = false;
                Invalidate(w.Bounds);
            }

            parent.selection.Clear();

            for (int i = 0; i < parent.Controls.Count; i++)
            {
                Control control = parent.Controls[i];

                if (control is BaseWidget)
                {
                    BaseWidget widget = (BaseWidget)control;

                    if (widget.ElementId.Equals(elementID))
                    {
                        parent.selection.Add(widget);
                        widget.Selected = true;
                        return;
                    }
                }
                else
                {
                    //do nothing
                }
            }

            MessageBox.Show( this.textBoxSearchWidget.Text +"doesn't exist");
            this.textBoxSearchWidget.Text = "";
        }

        private void textBoxSearchWidget_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                this.SerchElement(sender, e);
            }
        }

        private void buttonApply_Click(object sender, EventArgs e)
        {
            this.SerchElement(sender, e);
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.SerchElement(sender, e);
            this.Dispose();
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.Dispose();
            this.Close();
        }
    }
}