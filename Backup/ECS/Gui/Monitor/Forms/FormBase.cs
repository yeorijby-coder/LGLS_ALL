using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.Remoting;

namespace HECS.Gui.Monitor.Forms
{
    public partial class FormBase : Form
    {
        protected string defaultPanelName = "";
        protected string viewPanelName = "";
        public FormBase()
        {
            InitializeComponent();
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);

            //this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            //this.SetStyle(ControlStyles.CacheText, true);
            //this.SetStyle(ControlStyles.Opaque, true);
            //this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            //this.SetStyle(ControlStyles.ResizeRedraw, false);
            //this.SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            //this.SetStyle(ControlStyles.UserPaint, true);
            ViewPanel(defaultPanelName);
        }


        protected Control IsLoadedPanel(string panelName)
        {
           
            foreach (Control control in this.panelContainer.Controls)
            {
                if ( control.GetType().FullName == panelName)
                {
                    return control;
                }
            }
            return null;
        }
        protected void HidePanel(string panelName)
        {

            foreach (Control control in this.panelContainer.Controls)
            {
                if (control.GetType().FullName != panelName)
                {
                    if (control.Visible == true)
                    {
                        control.Visible = false;
                    }
                }
            }
        }
        protected void ViewPanel(string panelName)
        {
            if (string.IsNullOrEmpty (panelName))
            {
                return;
            }
            Control control = IsLoadedPanel(panelName);
            if (control != null)
            {
                viewPanelName = panelName;
                //control.BringToFront();
                HidePanel(panelName);
                control.Visible = true;
                return ;
            }
            try
            {
                ObjectHandle objHandle = Activator.CreateInstance(this.GetType().Assembly.FullName, panelName);
                Control createPanel = (Control)objHandle.Unwrap();
                //createPanel.SuspendLayout();
                this.panelContainer.Controls.Add(createPanel);
                createPanel.BackColor = Color.LemonChiffon;
                createPanel.Dock = DockStyle.Fill;
                //createPanel.BringToFront();
                //createPanel.ResumeLayout();
                HidePanel(panelName);
                createPanel.Visible = true;
                viewPanelName = panelName;
            }
            catch (Exception ex )
            {
                System.Console.WriteLine("FormBase:" + ex.StackTrace);
            }
        }
    }
}