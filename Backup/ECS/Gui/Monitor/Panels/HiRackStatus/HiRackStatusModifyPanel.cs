using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Db.Entity;
using ECP.Global;

namespace HECS.Gui.Monitor.Panels.HiRackStatus
{
    public partial class HiRackStatusModifyPanel : UserControl
    {
        List<CodeMasterData> hirackTransferStatusList = null;
        List<CodeMasterData> hirackStatusList = null;
        List<CodeMasterData> fromPositionList = null;
        List<CodeMasterData> companyTypeList = null;
        List<CodeMasterData> unitList = null;

        public HiRackStatusModifyPanel()
        {
            InitializeComponent();
            InitComponent();
        }

        private void InitComponent()
        {
            setConditionData();
        }

        private void setConditionData()
        {
            //Hirack 상태 정보 조회
            CodeMasterData codeMaster = new CodeMasterData();
            hirackTransferStatusList = codeMaster.getHirackTransferStatusValues(GlobalConstant.ALL_CHARACTER);
            hirackStatusList = codeMaster.getHirackStatusValues(GlobalConstant.ALL_CHARACTER);
            fromPositionList = codeMaster.getPositionValues(GlobalConstant.FROM_TEXT);
            companyTypeList = codeMaster.getCompanyTypeValues();
            unitList = codeMaster.getUnitValues(GlobalConstant.ALL_CHARACTER);

            comboBoxUnit.Items.Clear();
            foreach (CodeMasterData dataTemp in unitList)
            {
                this.comboBoxUnit.Items.Add(dataTemp.Value);
            }

            comboBoxHiracktransferstatus.Items.Clear();
            foreach (CodeMasterData dataTemp in hirackTransferStatusList)
            {
                this.comboBoxHiracktransferstatus.Items.Add(dataTemp.Value);
            }

            comboBoxCellstatus.Items.Clear();
            foreach (CodeMasterData dataTemp in hirackStatusList)
            {
                this.comboBoxCellstatus.Items.Add(dataTemp.Value);
            }

            comboBoxFromposition.Items.Clear();
            foreach (CodeMasterData dataTemp in fromPositionList)
            {
                this.comboBoxFromposition.Items.Add(dataTemp.Value);
            }

            comboBoxFromplant.Items.Clear();
            foreach (CodeMasterData dataTemp in companyTypeList)
            {
                this.comboBoxFromplant.Items.Add(dataTemp.Value);
            }
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
                setConditionData();

                this.labelPlantcode.Text = data.Plantcode;
                this.labelCellid.Text = data.Cellid;
                this.textBoxPalletid.Text = data.Palletid;

                //          this.comboBoxHiracktransferstatus.Text = "";
                this.comboBoxHiracktransferstatus.Items.Clear();
                this.comboBoxHiracktransferstatus.Text = "";
                foreach (CodeMasterData dataTemp in hirackTransferStatusList)
                {
                    this.comboBoxHiracktransferstatus.Items.Add(dataTemp.Value);
                }

                if (this.comboBoxHiracktransferstatus.Items.Count > 0)
                {
                    int Count = 0;
                    foreach (CodeMasterData dataTemp in hirackTransferStatusList)
                    {
                        if (data.Hiracktransferstatus == dataTemp.Code)
                        {
                            this.comboBoxHiracktransferstatus.SelectedIndex = Count;
                            break;
                        }
                        Count++;
                    }
                }


                //            this.comboBoxCellstatus.Text = data.Cellstatus;
                this.comboBoxCellstatus.Items.Clear();
                this.comboBoxCellstatus.Text = "";
                foreach (CodeMasterData dataTemp in hirackStatusList)
                {
                    this.comboBoxCellstatus.Items.Add(dataTemp.Value);
                }

                if (this.comboBoxCellstatus.Items.Count > 0)
                {
                    int Count = 0;
                    foreach (CodeMasterData dataTemp in hirackStatusList)
                    {
                        if (data.Cellstatus == dataTemp.Code)
                        {
                            this.comboBoxCellstatus.SelectedIndex = Count;
                            break;
                        }
                        Count++;
                    }
                }

                this.textBoxBatchno.Text = data.Batchno;
                this.textBoxMaterialno.Text = data.Materialno;
                this.textBoxQty.Text = data.Qty;

                //            this.comboBoxUnit.Text = data.Unit;
                this.comboBoxUnit.Items.Clear();
                this.comboBoxUnit.Text = "";
                foreach (CodeMasterData dataTemp in unitList)
                {
                    this.comboBoxUnit.Items.Add(dataTemp.Value);
                }

                if (this.comboBoxUnit.Items.Count > 0)
                {
                    int Count = 0;
                    foreach (CodeMasterData dataTemp in unitList)
                    {
                        if (data.Unit == dataTemp.Code)
                        {
                            this.comboBoxUnit.SelectedIndex = Count;
                            break;
                        }
                        Count++;
                    }
                }

                //            this.comboBoxFromplant.Text = data.Fromplant;
                this.comboBoxFromplant.Items.Clear();
                this.comboBoxFromplant.Text = "";
                foreach (CodeMasterData dataTemp in companyTypeList)
                {
                    this.comboBoxFromplant.Items.Add(dataTemp.Value);
                }

                if (this.comboBoxFromplant.Items.Count > 0)
                {
                    int Count = 0;
                    foreach (CodeMasterData dataTemp in companyTypeList)
                    {
                        if (data.Fromplant == dataTemp.Code)
                        {
                            this.comboBoxFromplant.SelectedIndex = Count;
                            break;
                        }
                        Count++;
                    }
                }

                //            this.comboBoxFromposition.Text = data.Fromposition;
                this.comboBoxFromposition.Items.Clear();
                this.comboBoxFromposition.Text = "";
                foreach (CodeMasterData dataTemp in fromPositionList)
                {
                    this.comboBoxFromposition.Items.Add(dataTemp.Value);
                }

                if (this.comboBoxFromposition.Items.Count > 0)
                {
                    int Count = 0;
                    foreach (CodeMasterData dataTemp in fromPositionList)
                    {
                        if (data.Fromposition == dataTemp.Code)
                        {
                            this.comboBoxFromposition.SelectedIndex = Count;
                            break;
                        }
                        Count++;
                    }
                }

                this.textBoxValidexpireddate.Text = data.Validexpireddate;
                this.labelSappalletid.Text = data.Sappalletid;
                this.labelSapbatchno.Text = data.Sapbatchno;
                this.labelSapitemno.Text = data.Sapitemno;
                this.labelSapqty.Text = data.Sapqty;
                this.labelSapunit.Text = data.Sapunit;
                this.textBoxCreatetime.Text = data.Createtime;
                this.textBoxCreateman.Text = data.Createman;
                this.labelUpdatetime.Text = data.Updatetime;
                this.labelUpdateman.Text = data.Updateman;
            }
        }

        public HirackData getComponentValues()
        {
            HirackData data = new HirackData();

            data.Plantcode = this.labelPlantcode.Text;
            data.Cellid = this.labelCellid.Text;
            data.Palletid = this.textBoxPalletid.Text;

            //data.Hiracktransferstatus = this.comboBoxHiracktransferstatus.Text;
            foreach (CodeMasterData dataTemp in hirackTransferStatusList)
            {
                if (this.comboBoxHiracktransferstatus.Text == dataTemp.Value)
                {
                    data.Hiracktransferstatus = dataTemp.Code;
                }
            }

            //data.Cellstatus = this.comboBoxCellstatus.Text;
            foreach (CodeMasterData dataTemp in hirackStatusList)
            {
                if (this.comboBoxCellstatus.Text == dataTemp.Value)
                {
                    data.Cellstatus = dataTemp.Code;
                }
            }

            data.Batchno = this.textBoxBatchno.Text;
            data.Materialno = this.textBoxMaterialno.Text;
            data.Qty = this.textBoxQty.Text;

            //data.Unit = this.comboBoxUnit.Text;
            foreach (CodeMasterData dataTemp in unitList)
            {
                if (this.comboBoxUnit.Text == dataTemp.Value)
                {
                    data.Unit = dataTemp.Code;
                }
            }


            //data.Fromplant = this.comboBoxFromplant.Text;
            foreach (CodeMasterData dataTemp in companyTypeList)
            {
                if (this.comboBoxFromplant.Text == dataTemp.Value)
                {
                    data.Fromplant = dataTemp.Code;
                }
            }

            //data.Fromposition = this.comboBoxFromposition.Text;
            foreach (CodeMasterData dataTemp in fromPositionList)
            {
                if (this.comboBoxFromposition.Text == dataTemp.Value)
                {
                    data.Fromposition = dataTemp.Code;
                }
            }

            data.Validexpireddate = this.textBoxValidexpireddate.Text;
            data.Sappalletid = this.labelSappalletid.Text;
            data.Sapbatchno = this.labelSapbatchno.Text;
            data.Sapitemno = this.labelSapitemno.Text;
            data.Sapqty = this.labelSapqty.Text;
            data.Sapunit = this.labelSapunit.Text;
            data.Createtime = this.textBoxCreatetime.Text;
            data.Createman = this.textBoxCreateman.Text;
            data.Updatetime = DateTime.Now.ToString("yyyyMMddHHmmssfff");
            data.Updateman = "SYSTEM"; 

            return data;
        }
    }
}
