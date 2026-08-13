using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Db.Entity;

namespace HECS.Gui.Monitor.Panels.HiRackStatus
{
    public partial class HiRackStatusInformPanel : UserControl
    {
        public HiRackStatusInformPanel()
        {
            InitializeComponent();
        }

        delegate void setComponentValuesDelegate(HirackData dataTemp);
        public void setComponentValues(HirackData data)
        {
            if (this.InvokeRequired)
            {
                setComponentValuesDelegate tempDelegate = new setComponentValuesDelegate(setComponentValues);
                this.Invoke(tempDelegate, new object[] { data });
            }
            else
            {
                this.labelPlantcode.Text = data.Plantcode;
                this.labelCellid.Text = data.Cellid;
                this.labelPalletid.Text = data.Palletid;
                this.labelHiracktransferstatus.Text = data.Hiracktransferstatus;
                this.labelCellstatus.Text = data.Cellstatus;
                this.labelBatchno.Text = data.Batchno;
                this.labelMaterialno.Text = data.Materialno;
                this.labelQty.Text = data.Qty;
                this.labelUnit.Text = data.Unit;
                this.labelFromplant.Text = data.Fromplant;
                this.labelFromposition.Text = data.Fromposition;
                this.labelValidexpireddate.Text = data.Validexpireddate;
                this.labelSappalletid.Text = data.Sappalletid;
                this.labelSapbatchno.Text = data.Sapbatchno;
                this.labelSapitemno.Text = data.Sapitemno;
                this.labelSapqty.Text = data.Sapqty;
                this.labelSapunit.Text = data.Sapunit;
                this.labelCreatetime.Text = data.Createtime;
                this.labelCreateman.Text = data.Createman;
                this.labelUpdatetime.Text = data.Updatetime;
                this.labelUpdateman.Text = data.Updateman;
            }
        }

        //public HirackData getComponentValues()
        //{
        //    PalletBlockingData data = new PalletBlockingData();

        //    data.Palletid = this.labelPalletid.Text;
        //    data.Batchno = this.labelBatchno.Text;
        //    data.Materialno = this.labelMaterialno.Text;
        //    data.Qty = this.labelQty.Text;
        //    data.Unit = this.labelUnit.Text;
        //    data.Blockingtype = this.labelBlockingtype.Text;
        //    data.Blockingreason = this.labelBlockingreason.Text;
        //    data.Starttime = this.labelStarttime.Text;
        //    data.Startman = this.labelStartman.Text;
        //    data.Endtime = this.labelEndtime.Text;
        //    data.Endman = this.labelEndman.Text;

        //    return data;
        //}
    }
}
