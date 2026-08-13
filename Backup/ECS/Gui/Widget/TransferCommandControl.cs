using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Db.Entity;
using ECP.Service.Movement;
using ECP.Db;
using System.Data.SqlClient;

namespace HECS.Gui.Widget
{
    public partial class TransferCommandControl : UserControl
    {
        private delegate void delegatInitialize();
        private string commandId = null;

        public string CommandId
        {
            get
            {
                return commandId;
            }
            set
            {
                commandId = value;
                InitDelegate();
            }
        }

   
        public TransferCommandControl()
        {
            InitializeComponent();
        }
        public void InitDelegate()
        {
            try
            {
                delegatInitialize view = new delegatInitialize(ViewTransferData);
                view.Invoke();
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("InitDelegate:" + ex.StackTrace);
            }
        }
        private void ViewTransferData()
        {
            if (string.IsNullOrEmpty (commandId ) )
            {
                return;
            }
            TransferData transfer =  MovementManager.getTransferData(commandId);
            if (commandId == null)
            {
                return;
            }
            this.textBoxBatch.Text = transfer.Batchno;
            this.textBoxCommandId.Text = transfer.Commandid;
            if (transfer.Commandtype == EntityEnumData.COMMANDTYPE.MOVE.ToString())
            {
                this.textBoxCommandType.Text = "이동";
            }
            else if (transfer.Commandtype == EntityEnumData.COMMANDTYPE.INGO.ToString())
            {
                this.textBoxCommandType.Text = "입고";
            }
            else if (transfer.Commandtype  == EntityEnumData.COMMANDTYPE.OUTGO.ToString())
            {
                this.textBoxCommandType.Text = "출고";
            }
            else
            {
                this.textBoxCommandType.Text = transfer.Commandtype;
            }
            this.textBoxMaterialNo.Text = transfer.Materialno.TrimStart('0');
            this.textBoxPalletId.Text = transfer.Palletid;
            this.textBoxPriority.Text = transfer.Priority;
            this.textBoxSource.Text = transfer.Fromposition ;
            this.textBoxDestination.Text = transfer.Toposition;
        }

        private void buttonChangePriority_Click(object sender, EventArgs e)
        {
            TransferData transferData = MovementManager.getTransferData(this.textBoxCommandId.Text);
            if (transferData == null)
            {
                return; 
            }
            if(string.IsNullOrEmpty(this.textBoxNewPriority.Text ))
            {
                return ;
            }
            int newPriority = 0; 
            if(int.TryParse(this.textBoxNewPriority.Text , out newPriority ) == false)
            {
                return ;
            }

            DBConnection theDB = null;
            try
            {
                theDB = DBControl.GetConnection();
                transferData.Priority = newPriority.ToString();
                this.textBoxPriority.Text = newPriority.ToString();
                transferData.updateTransferData(theDB, true);
            }
            catch (SqlException se)
            {
                //Error Log
            }
            finally
            {
                DBControl.RemoveDBConection(theDB);
            }


        }
    }
}
