using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Db.Entity;
using ECP.Global;
using ECP.BizRule;
using System.Data.SqlClient;

namespace HECS.Gui.Monitor.Panels.MoveRequest
{
    public partial class MoveRequestAddPanel : UserControl
    {
        List<CodeMasterData> fromPositionList = null;
        List<CodeMasterData> toPositionList = null;

        public MoveRequestAddPanel()
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
            fromPositionList = null;
            toPositionList = null;

            CodeMasterData data = new CodeMasterData();
            fromPositionList = data.getPositionValues(GlobalConstant.FROM_TEXT);
            toPositionList = data.getPositionValues(GlobalConstant.TO_TEXT);

            //출발 위치 setting
            comboBoxFromposition.Items.Clear();
            foreach (CodeMasterData dataTemp in fromPositionList)
            {
                comboBoxFromposition.Items.Add(dataTemp.Value);
            }

            if (comboBoxFromposition.Items.Count > 0)
            {
                comboBoxFromposition.SelectedIndex = 0;
            }

            //도착 위치 setting
            comboBoxToposition.Items.Clear();
            foreach (CodeMasterData dataTemp in toPositionList)
            {
                comboBoxToposition.Items.Add(dataTemp.Value);
                
            }

            if(comboBoxToposition.Items.Count >0) 
            {
                comboBoxToposition.SelectedIndex = 0;
            }
        }

        private void buttonMakeNo_Click(object sender, EventArgs e)
        {
            TransferData transferData = new TransferData();
            string strCommandid = "";

            try
            {
                strCommandid = transferData.getMaxCommandId(EntityEnumData.MOVEMENTTYPE.MOVE.ToString());
            }
            catch (Exception ex)
            {
                Util.Util.ShowMesagePopup(ex.Message);
                return;
            }

            labelCommandid.Text = strCommandid;
        }

        public TransferData getComponentValues()
        {
            TransferData transferData = new TransferData();

            string fromposition = "";
            string toposition = "";
            string now = DateTime.Now.ToString("yyyyMMddHHmmssfff");

            foreach (CodeMasterData codeData in fromPositionList)
            {
                if (codeData.Value == (comboBoxFromposition.Text))
                {
                    fromposition = codeData.Code;
                }
            }

            foreach (CodeMasterData codeData in toPositionList)
            {
                if (codeData.Value == (comboBoxToposition.Text))
                {
                    toposition = codeData.Code;
                }
            }

            //transferData.Plantcode = GlobalConstant.PLANT_CODE;
            transferData.Commandid = labelCommandid.Text;

            transferData.Fromposition = fromposition;
            transferData.Toposition = toposition;

            if (fromposition == GlobalConstant.STORAGE_LOCATION_CODE)
            {
                if (toposition != GlobalConstant.STORAGE_LOCATION_CODE)
                {
                    transferData.Commandtype = EntityEnumData.COMMANDTYPE.OUTGO.ToString();
                }
                else
                {
                    transferData.Commandtype = EntityEnumData.COMMANDTYPE.MOVE.ToString();
                }
            }
            else
            {
                if (toposition == GlobalConstant.STORAGE_LOCATION_CODE)
                {
                    transferData.Commandtype = EntityEnumData.COMMANDTYPE.INGO.ToString();
                }
                else
                {
                    transferData.Commandtype = EntityEnumData.COMMANDTYPE.MOVE.ToString();
                }
            }

            transferData.Movementtype = EntityEnumData.MOVEMENTTYPE.MOVE.ToString();  //이동;
            transferData.Priority = GlobalConstant.TRANSFER_PRIORITY_DEFAULT;

            transferData.Palletid = textBoxPalletId.Text;

            transferData.Fromdeviceid = comboBoxFromdevice.Text;
            transferData.Fromlocationid = comboBoxFromlocation.Text;
            transferData.Fromsublocationid = textBoxFromsublocation.Text;

            transferData.Todeviceid = comboBoxTodevice.Text;
            transferData.Tolocationid = comboBoxTolocation.Text;
            transferData.Tosublocationid = textBoxTosublocation.Text;

            transferData.Materialno = textBoxMaterialno.Text;
            transferData.Qty = textBoxQty.Text;
            transferData.Unit = textBoxUnit.Text;

            transferData.Transferstatus = EntityEnumData.TRANSFERSTATUS.Wait.ToString();
            transferData.Createtime = now;
            transferData.Createman = "SYSTEM";


            //transferData.Resultcode = "";
            transferData.Executetime = "";
            transferData.Completetime = "";
            //transferData.Executecount = "";

            transferData.Transferdetailflag = "N";

//            if ((fromposition == GlobalConstant.STORAGE_LOCATION_CODE) &&
//               (toposition == GlobalConstant.STORAGE_LOCATION_CODE)
//              )
//            {
//                HirackData hData = new HirackData();

//                try
//                {
//                    hData.getHirackData(transferData.Fromsublocationid);
//                }
//                catch (Exception se)
//                {
//                    throw se;
//                }

////                transferData.Batchno = hData.Batchno;
//                transferData.Materialno = hData.Materialno;
//                transferData.Qty = hData.Qty;
//                transferData.Unit = hData.Unit;
//            }

            return transferData;
        }

        public void setComponentValues(TransferData data)
        {
            setConditionData();

            //출발 위치 setting
            comboBoxFromposition.Items.Clear();
            comboBoxFromposition.Text = "";
            foreach (CodeMasterData dataTemp in fromPositionList)
            {
                comboBoxFromposition.Items.Add(dataTemp.Value);
            }
            if (comboBoxFromposition.Items.Count > 0)
            {
                comboBoxFromposition.SelectedIndex = 0;
            }

            //도착 위치 setting
            comboBoxToposition.Items.Clear();
            comboBoxToposition.Text = "";
            foreach (CodeMasterData dataTemp in toPositionList)
            {
                comboBoxToposition.Items.Add(dataTemp.Value);
            }
            if (comboBoxToposition.Items.Count > 0)
            {
                comboBoxToposition.SelectedIndex = 0;
            }

            labelCommandid.Text = data.Commandid;
            textBoxPalletId.Text = data.Palletid;

        }

        private void comboBoxFromposition_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<PositionData> fromList = new List<PositionData>();
            PositionData data = new PositionData();

            string fromposition = "";
            string sampleflag = "";

            foreach (CodeMasterData codeData in fromPositionList)
            {
                if (codeData.Value == (comboBoxFromposition.Text))
                {
                    fromposition = codeData.Code;
                }
            }

            //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
            fromList = data.getPositionDataList(GlobalConstant.FROM_TEXT, fromposition, sampleflag);

            comboBoxFromdevice.Items.Clear();
            comboBoxFromdevice.Text = "";

            comboBoxFromlocation.Items.Clear();
            comboBoxFromlocation.Text = "";

            textBoxFromsublocation.Text = "";

            string strTemp = "";
            foreach (PositionData listTemp in fromList)
            {
                if (strTemp != listTemp.Deviceid)
                {
                    comboBoxFromdevice.Items.Add(listTemp.Deviceid);
                    strTemp = listTemp.Deviceid;
                }
            }

            if (comboBoxFromdevice.Items.Count == 1)
            {
                comboBoxFromdevice.Text = comboBoxFromdevice.Items[0].ToString();
                comboBoxFromdevice_SelectedIndexChanged(null, null);
            }
        }

        private void comboBoxToposition_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<PositionData> toList = new List<PositionData>();
            PositionData data = new PositionData();

            string toposition = "";

            foreach (CodeMasterData codeData in toPositionList)
            {
                if (codeData.Value == (comboBoxToposition.Text))
                {
                    toposition = codeData.Code;
                }
            }


            //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
            toList = data.getPositionDataList(GlobalConstant.TO_TEXT, toposition, "");

            comboBoxTodevice.Items.Clear();
            comboBoxTodevice.Text = "";

            comboBoxTolocation.Items.Clear();
            comboBoxTolocation.Text = "";

            textBoxTosublocation.Text = "";

            string strTemp = "";
            foreach (PositionData listTemp in toList)
            {
                if (strTemp != listTemp.Deviceid)
                {
                    comboBoxTodevice.Items.Add(listTemp.Deviceid);
                    strTemp = listTemp.Deviceid;
                }
            }

            if (comboBoxTodevice.Items.Count == 1)
            {
                comboBoxTodevice.Text = comboBoxTodevice.Items[0].ToString();
                comboBoxTodevice_SelectedIndexChanged(null, null);
            }
        }

        private void comboBoxFromdevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<PositionData> fromList = new List<PositionData>();
            PositionData data = new PositionData();

            string strFromposition = comboBoxFromposition.Text;
            string sampleflag = "";

            foreach (CodeMasterData codeData in fromPositionList)
            {
                if (codeData.Value == (comboBoxFromposition.Text))
                {
                    strFromposition = codeData.Code;
                }
            }


            string strFromDevice = comboBoxFromdevice.Text;

            //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
            fromList = data.getPositionDataList(GlobalConstant.FROM_TEXT, strFromposition, sampleflag);

            comboBoxFromlocation.Items.Clear();
            comboBoxFromlocation.Text = "";

            textBoxFromsublocation.Text = "";

            string strTemp = "";
            foreach (PositionData listTemp in fromList)
            {
                if (strFromDevice == listTemp.Deviceid)
                {
                    if (strTemp != listTemp.Locationid)
                    {
                        comboBoxFromlocation.Items.Add(listTemp.Locationid);
                        strTemp = listTemp.Locationid;
                    }
                }
            }

            if (comboBoxFromlocation.Items.Count == 1)
            {
                comboBoxFromlocation.Text = comboBoxFromlocation.Items[0].ToString(); ;
                comboBoxFromlocation_SelectedIndexChanged(null, null);
            }

        }

        private void comboBoxFromlocation_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<PositionData> fromList = new List<PositionData>();
            PositionData data = new PositionData();

            string strFromposition = comboBoxFromposition.Text;
            string sampleflag = "";

            foreach (CodeMasterData codeData in fromPositionList)
            {
                if (codeData.Value == (comboBoxFromposition.Text))
                {
                    strFromposition = codeData.Code;
                }
            }

           
            string strFromDevice = comboBoxFromdevice.Text;
            string strFromLocation = comboBoxFromlocation.Text;

            //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
            fromList = data.getPositionDataList(GlobalConstant.FROM_TEXT, strFromposition, sampleflag);

            textBoxFromsublocation.Text = "";

            string strTemp = "";
            foreach (PositionData listTemp in fromList)
            {
                if (strFromDevice == listTemp.Deviceid)
                {
                    if (strFromLocation == listTemp.Locationid)
                    {
                        if (strTemp != listTemp.Sublocationid)
                        {
                            textBoxFromsublocation.Text = listTemp.Sublocationid;
                            strTemp = listTemp.Sublocationid;
                        }
                    }
                }
            }
        }

        private void comboBoxTodevice_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<PositionData> toList = new List<PositionData>();
            PositionData data = new PositionData();

            string strToposition = comboBoxToposition.Text;

            foreach (CodeMasterData codeData in toPositionList)
            {
                if (codeData.Value == (comboBoxToposition.Text))
                {
                    strToposition = codeData.Code;
                }
            }

            string strToDevice = comboBoxTodevice.Text;

            //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
            toList = data.getPositionDataList(GlobalConstant.TO_TEXT, strToposition, "");

            comboBoxTolocation.Items.Clear();
            comboBoxTolocation.Text = "";

            textBoxTosublocation.Text = "";

            string strTemp = "";
            foreach (PositionData listTemp in toList)
            {
                if (strToDevice == listTemp.Deviceid)
                {
                    if (strTemp != listTemp.Locationid)
                    {
                        comboBoxTolocation.Items.Add(listTemp.Locationid);
                        strTemp = listTemp.Locationid;
                    }
                }
            }

            if (comboBoxTolocation.Items.Count == 1)
            {
                comboBoxTolocation.Text = comboBoxTolocation.Items[0].ToString(); ;
                comboBoxTolocation_SelectedIndexChanged(null, null);
            }

        }

        private void comboBoxTolocation_SelectedIndexChanged(object sender, EventArgs e)
        {
            List<PositionData> toList = new List<PositionData>();
            PositionData data = new PositionData();

            string strToposition = comboBoxToposition.Text;

            foreach (CodeMasterData codeData in toPositionList)
            {
                if (codeData.Value == (comboBoxToposition.Text))
                {
                    strToposition = codeData.Code;
                }
            }

            string strToDevice = comboBoxTodevice.Text;
            string strToLocation = comboBoxTolocation.Text;

            //FROM/To Location은 List 가져옴(Sample Flag 무조건 "N")
            toList = data.getPositionDataList(GlobalConstant.TO_TEXT, strToposition, "");

            textBoxTosublocation.Text = "";

            string strTemp = "";
            foreach (PositionData listTemp in toList)
            {
                if (strToDevice == listTemp.Deviceid)
                {
                    if (strToLocation == listTemp.Locationid)
                    {
                        if (strTemp != listTemp.Sublocationid)
                        {
                            textBoxTosublocation.Text = listTemp.Sublocationid;
                            strTemp = listTemp.Sublocationid;
                        }
                    }
                }
            }
        }

        private void MoveRequestAddPanel_Load(object sender, EventArgs e)
        {

        }
    }
}
