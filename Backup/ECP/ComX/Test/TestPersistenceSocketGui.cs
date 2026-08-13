using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;


namespace ECP.ComX.Test
{
    public partial class TestPersistentSocketGui : Form, Logger
    {


        TestPersistentSocket socket = null;

        int logLineCount = 0;


        public TestPersistentSocketGui()
        {
            InitializeComponent();
        }
       

        private void InitializeConnection()
        {
            if (socket == null)
            {
                socket = new TestPersistentSocket(
                textBoxHostIP.Text.Trim(),
                Convert.ToInt32(textBoxPort.Text.Trim()),
                checkBoxISActive.Checked,
                Convert.ToInt32(textBoxInterval.Text.Trim()),
                this);
            }
        }           

        private void checkBoxConnection_CheckedChanged(object sender, EventArgs e)
        {
            InitializeConnection();

            if (checkBoxConnection.Checked)
            {
                if (socket != null)
                    socket.ChangeConnectState(true);
            }
            else
            {
                if (socket != null)
                    socket.ChangeConnectState(false);
            }           
        }

        private void checkBoxContinousMessage_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxContinousMessage.Checked)
            {
                if(socket != null)
                socket.StartSendContinousMessage();
            }
            else
            {
                if (socket != null)
                socket.StopSendContinousMessage();

            }
        }

        private void buttonSendOneShot_Click(object sender, EventArgs e)
        {
            if (socket != null)
            {
                if (textBoxMessage.Text.Equals(string.Empty))
                {
                    socket.SendOnsShotTestMessage();
                }
                else
                {
                    socket.SendMessage(this.textBoxMessage.Text);
                }


            }
        }


        private delegate void DelegateDebug(int machineId, string id, string message);  


        public void Debug(int machineId, string id, string message)
        {
            try
            {
                this.BeginInvoke(new DelegateDebug(DebugImpl), machineId, id, message);
            }
            catch (InvalidOperationException)
            {
            }
        }

        public void DebugImpl(int machineId, string id, string message)
        {
            logLineCount++;

            if (logLineCount > 30)
            {
                textBoxLog.Text = textBoxLog.Text.Substring
                    (textBoxLog.Lines[0].Length + Environment.NewLine.Length);

            }


            this.textBoxLog.Text += message;
            this.textBoxLog.Text += Environment.NewLine;
            this.textBoxLog.SelectionStart = textBoxLog.Text.Length;
            this.textBoxLog.ScrollToCaret();
        }



        protected override bool ProcessDialogKey(Keys keyData)
        {
            bool bRet = false;
            switch (keyData)
            {
                case Keys.Up:
                case Keys.Down:
                case Keys.Left:
                case Keys.Right:
                    bRet = true;
                    break;
                default:
                    bRet = base.ProcessDialogKey(keyData);
                    break;
            }
            return bRet;
        }

        private void textBoxSleep_TextChanged(object sender, EventArgs e)
        {
            if (socket != null)
                socket.ContinousMessageInterval = Convert.ToInt32(textBoxInterval.Text);
        }

        private void groupBoxSendMessage_Enter(object sender, EventArgs e)
        {

        }

        
       
     
    }
}