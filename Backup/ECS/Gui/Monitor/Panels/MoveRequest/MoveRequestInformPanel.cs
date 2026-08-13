using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Db.Entity;

namespace HECS.Gui.Monitor.Panels.MoveRequest
{
    public partial class MoveRequestInformPanel : UserControl
    {
        public MoveRequestInformPanel()
        {
            InitializeComponent();
        }

        public void setComponentValues(TransferData data)
        {
            this.labelCommandid.Text = data.Commandid;
            this.labelCommandtype.Text = data.Commandtype;
            this.labelMovementtype.Text = data.Movementtype;
            this.labelPriority.Text = data.Priority;
            this.labelPalletid.Text = data.Palletid;
            this.labelMaterialno.Text = data.Materialno;
            this.labelQty.Text = data.Qty;
            this.labelUnit.Text = data.Unit;
            this.labelFromdeviceid.Text = data.Fromdeviceid;
            this.labelFromlocationid.Text = data.Fromlocationid;
            this.labelFromsublocationid.Text = data.Fromsublocationid;
            this.labelTodeviceid.Text = data.Todeviceid;
            this.labelTolocationid.Text = data.Tolocationid;
            this.labelTosublocationid.Text = data.Tosublocationid;
            //this.labelCurrentcommandseq.Text = data.Executecount;
            this.labelTransferstatus.Text = data.Transferstatus;
            this.labelCreatetime.Text = data.Createtime;
            this.labelCreateman.Text = data.Createman;
            this.labelExecutetime.Text = data.Executetime;
            this.labelCompletetime.Text = data.Completetime;
        }

        public TransferData getComponentValues()
        {
            TransferData data = new TransferData();

            data.Commandid = this.labelCommandid.Text;
            data.Commandtype = this.labelCommandtype.Text;
            data.Movementtype = this.labelMovementtype.Text;
            data.Priority = this.labelPriority.Text;
            data.Palletid = this.labelPalletid.Text;
            data.Materialno = this.labelMaterialno.Text;
            data.Qty = this.labelQty.Text;
            data.Unit = this.labelUnit.Text;
            data.Fromdeviceid = this.labelFromdeviceid.Text;
            data.Fromlocationid = this.labelFromlocationid.Text;
            data.Fromsublocationid = this.labelFromsublocationid.Text;
            data.Todeviceid = this.labelTodeviceid.Text;
            data.Tolocationid = this.labelTolocationid.Text;
            data.Tosublocationid = this.labelTosublocationid.Text;
            //data.Executecount = this.labelCurrentcommandseq.Text;
            data.Transferstatus = this.labelTransferstatus.Text;
            data.Createtime = this.labelCreatetime.Text;
            data.Createman = this.labelCreateman.Text;
            data.Executetime = this.labelExecutetime.Text;
            data.Completetime = this.labelCompletetime.Text;
            
            return data;
        }
    }
}
