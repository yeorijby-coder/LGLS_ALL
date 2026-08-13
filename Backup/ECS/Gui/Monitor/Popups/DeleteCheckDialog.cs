using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Event;



using ECP.Service.Statistics;
using HECS.Util;


namespace HECS.Gui.Monitor.Popups
{


    public partial class DeleteCheckDialog : DialogBaseForm
    {
        DialogResult ret = DialogResult.No;

        public delegate void DelegateButtonSelected(DialogResult ret, string formClass);
        public event DelegateButtonSelected OnButtonSelected;

        private delegate void delegateUpdateWidget();

        private string messageData = "";
        public string MessageData
        {
            get { return messageData; }
            set 
            { 
                messageData = value;
                UpdateDisplayText();
            }
        }

        private string formName = "";

        public string FormName
        {
            get { return formName; }
            set { formName = value; }
        }


        public void UpdateDisplayText()
        {
            delegateUpdateWidget update = new delegateUpdateWidget(UpdateDialog);
            update.Invoke();
        }

        private void UpdateDialog()
        {
            this.txtDesciption.Text = messageData;
            this.txtDesciption.Visible = true;

            buttonYes.Visible = true;
            buttonNo.Visible = true;
            Invalidate();
        }

        public DeleteCheckDialog()
        {
            InitializeComponent();
        }

        private void buttonYes_Click(object sender, EventArgs e)
        {
            ret = DialogResult.Yes;

            if (OnButtonSelected != null)
            {
                OnButtonSelected(ret, this.FormName);
            }
            this.Hide();
        }

        private void buttonNo_Click(object sender, EventArgs e)
        {
            ret = DialogResult.No;

            if (OnButtonSelected != null)
            {
                OnButtonSelected(ret, this.FormName);
            }
            this.Hide();
        }         
   
    }


}

