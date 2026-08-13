using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace HECS.Gui.Monitor
{
    public partial class ConfigurationForm : Form
    {
        public ConfigurationForm()
        {
            InitializeComponent();
        }

        private void buttonSave_Click(object sender, EventArgs e)
        {
            
            for (int i = 0; i < this.panel1.Controls.Count; i++)
            {
                string str = "";
                str = str + panel1.Controls[i].GetType().Name;
                str = str + "\t" + ((Control)panel1.Controls[i]).Text;
                str = str + "\t" + ((Control)panel1.Controls[i]).ForeColor.ToArgb().ToString();
                str = str + "\t" + ((Control)panel1.Controls[i]).BackColor.ToArgb().ToString();
                str = str + "\t" + ((Control)panel1.Controls[i]).Location.X ;
                str = str + "\t" + ((Control)panel1.Controls[i]).Location.Y;
                str = str + "\t" + ((Control)panel1.Controls[i]).Width;
                str = str + "\t" + ((Control)panel1.Controls[i]).Height;
                ////System.Console.WriteLine( str);
            }
        }


    }
}