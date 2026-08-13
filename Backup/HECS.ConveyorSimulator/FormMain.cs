using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.ConveyorSimulator
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            FormControlMain form = new FormControlMain();
            form.Text = "LG생명과학 물류창고 시뮬레이터";

            form.ShowIcon = false;
            form.MinimizeBox = false;
            form.MaximizeBox = false;
            form.ControlBox = false;
            //form.TopLevel = true;
            form.FormBorderStyle = FormBorderStyle.None;

            this.AddOwnedForm(form);
            form.MdiParent = this;

            form.Show();
            form.WindowState = FormWindowState.Maximized;
          //  this.LayoutMdi(MdiLayout.TileVertical );
            this.Resize += new EventHandler(FormMain_Resize);
        }

        void FormMain_Resize(object sender, EventArgs e)
        {
            if (this.MdiChildren.Length > 0)
            {
                this.MdiChildren[0].WindowState = FormWindowState.Maximized;
            }
        }

    }
}