using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HECS.Device.Unit;
using HECS.Device;
using ECP.Db.Entity;
using ECP.Service.Movement;
using ECP.Util.Server;

namespace HECS.Gui.Monitor.Popups
{
    public partial class AGVForm : DialogBaseForm
    {
        private delegate void delegatInitialize();
        Timer timerRefresh = new Timer();
        private Vehicle vehicleObject = null;
        private bool isFirst = false;
        private bool isCached = false;
        private Dictionary<string, Control> controlCache = new Dictionary<string, Control>();
        public Vehicle VehicleObject
        {
            get
            {
                return vehicleObject;
            }
            set
            {
                vehicleObject = value;
                InitDelegate();
                timerRefresh.Start();
            }
        }
        public AGVForm()
        {
            InitializeComponent();

            timerRefresh.Interval = 1000;
            timerRefresh.Tick += new EventHandler(timerRefresh_Tick);
        }



        private void InitializeVehicleInfo()
        {
            if (vehicleObject == null)
            {
                return;
            }
            InitVehicleInformation();
        }

        private void InitVehicleInformation()
        {
            if (vehicleObject == null)
            {
                return;
            }
            switch (vehicleObject.ElementId)
            {
                case "VEHICLE:1":
                    this.labelVehicleName.Text = "AGV1";
                    this.labelVehicleDescription.Text = "(AGV 1호차)";
                    break;
                case "VEHICLE:2":
                    this.labelVehicleName.Text = "AGV2";
                    this.labelVehicleDescription.Text = "(AGV 2호차)";
                    break;
                case "VEHICLE:3":
                    this.labelVehicleName.Text = "AGV3";
                    this.labelVehicleDescription.Text = "(AGV 3호차)";
                    break;
            }
            switch (vehicleObject.AgvVehicleState)
            {
                case Vehicle.AGV_RUN_STATE.CHARGE:
                    this.labelVehicleStatus.Text = "충전중";
                    this.labelVehicleStatus.GradientColorOne = Color.Cyan;
                    this.labelVehicleStatus.GradientColorTwo = Color.Cyan;
                    break;
                case Vehicle.AGV_RUN_STATE.DOWN:
                    this.labelVehicleStatus.Text = "DOWN";
                    this.labelVehicleStatus.GradientColorOne = Color.Red;
                    this.labelVehicleStatus.GradientColorTwo = Color.Red;
                    break;
                case Vehicle.AGV_RUN_STATE.EXIT_MAP:
                    this.labelVehicleStatus.Text = "탈선";
                    this.labelVehicleStatus.GradientColorOne = Color.Gray;
                    this.labelVehicleStatus.GradientColorTwo = Color.Gray;
                    break;
                case Vehicle.AGV_RUN_STATE.IDLE:
                    this.labelVehicleStatus.Text = "대기중";
                    this.labelVehicleStatus.GradientColorOne = Color.Yellow;
                    this.labelVehicleStatus.GradientColorTwo = Color.Yellow;
                    break;
                case Vehicle.AGV_RUN_STATE.MANUAL:
                    this.labelVehicleStatus.Text = "매뉴얼";
                    this.labelVehicleStatus.GradientColorOne = Color.Violet;
                    this.labelVehicleStatus.GradientColorTwo = Color.Violet;
                    break;
                case Vehicle.AGV_RUN_STATE.OFFLINE:
                    this.labelVehicleStatus.Text = "통신장애";
                    this.labelVehicleStatus.GradientColorOne = Color.Crimson;
                    this.labelVehicleStatus.GradientColorTwo = Color.Crimson;
                    break;
                case Vehicle.AGV_RUN_STATE.RUN:
                    this.labelVehicleStatus.Text = "RUN";
                    this.labelVehicleStatus.GradientColorOne = Color.Green;
                    this.labelVehicleStatus.GradientColorTwo = Color.Green;
                    break;
            }


        }

        public void InitDelegate()
        {
            try
            {
                this.BeginInvoke(new delegatInitialize(InitializeVehicleInfo));
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("InitDelegate:" + ex.StackTrace);
            }
        }
        void timerRefresh_Tick(object sender, EventArgs e)
        {
            if (vehicleObject == null)
            {
                return;
            }
            RefreshVehicleState();
            InitVehicleInformation();
            RefreshCommandDetail();
        }

        private void RefreshCommandDetail()
        {
            labelCommandStatus.Text = "";
            labelStartLocation.Text = "";
            labelEndLocation.Text = "";
            labelTransferPallet.Text = "";
            labelMaterialCode.Text = "";
            labelBatchNo.Text = "";
            labelCommandId.Text = "";
            labelCommandSeq.Text = "";
            labelAdvice.Visible = false;
            if (string.IsNullOrEmpty(vehicleObject.Commandid) || string.IsNullOrEmpty(vehicleObject.CommandSeq))
            {
                if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.IDLE)
                {
                    labelCommandStatus.Text = "대기";
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.DOWN)
                {
                    labelCommandStatus.Text = "에러";
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.RUN)
                {
                    labelCommandStatus.Text = "명령수행";
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.CHARGE)
                {
                    labelCommandStatus.Text = "충전중";
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.EXIT_MAP)
                {
                    labelCommandStatus.Text = "탈선";
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.OFFLINE)
                {
                    labelCommandStatus.Text = "통신단절";
                }
                else if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.MANUAL)
                {
                    labelCommandStatus.Text = "매뉴얼";
                }
                else
                {
                    labelCommandStatus.Text = "";
                }
                return;
            }

            TransferData transferData = MovementManager.getTransferData(vehicleObject.Commandid);
            if (transferData == null)
            {
                labelCommandId.Text = vehicleObject.Commandid;
                labelAdvice.Text = "입/출고 요청번호가 DB에 없습니다.  [이상종료]처리하세요!!";
                labelAdvice.Visible = true;
                return;
            }
            TransferDetailData transferDetail = new TransferDetailData();
            transferDetail.getTransferDetailData(vehicleObject.Commandid, vehicleObject.CommandSeq);
            if (transferDetail == null)
            {
                labelCommandSeq.Text = vehicleObject.CommandSeq;
                labelAdvice.Text = "입/출고 상세요청번호가 DB에 없습니다.  [이상종료]처리하세요!!";
                labelAdvice.Visible = true;
                return;
            }

            labelCommandId.Text = vehicleObject.Commandid;
            labelCommandSeq.Text = vehicleObject.CommandSeq;



            ItineraryData itinerary = new ItineraryData();
            itinerary.getItineraryData(transferDetail.Pathid);

            if (itinerary != null)
            {
                if (itinerary.Fromsublocationid.StartsWith("PORT"))
                {
                    labelStartLocation.Text = itinerary.Fromsublocationid.Replace("PORT:", "").PadLeft(2, '0');
                }
                else
                {
                    labelStartLocation.Text = transferData.Fromsublocationid.Substring(0, 2) + "-" + transferData.Fromsublocationid.Substring(2, 2) + "-" + transferData.Fromsublocationid.Substring(4, 2);
                }
                if (itinerary.Tosublocationid.StartsWith("PORT"))
                {
                    labelEndLocation.Text = itinerary.Tosublocationid.Replace("PORT:", "").PadLeft(2, '0');
                }
                else
                {
                    labelEndLocation.Text = transferData.Tosublocationid.Substring(0, 2) + "-" + transferData.Tosublocationid.Substring(2, 2) + "-" + transferData.Tosublocationid.Substring(4, 2);
                }
            }

            labelTransferPallet.Text = transferData.Palletid;
            labelMaterialCode.Text = transferData.Materialno.TrimStart('0');
            labelBatchNo.Text = transferData.Batchno;
            labelCommandId.Text = vehicleObject.Commandid;
            labelCommandSeq.Text = vehicleObject.CommandSeq;


            this.labelLocation01.Text = vehicleObject.AgvLocation;
            this.labelFrom01.Text = labelStartLocation.Text;
            this.labelTo01.Text = labelEndLocation.Text;
            this.labelComplete01.Text = "";
            this.labelPalletId.Text = vehicleObject.AgvPalletId;
        }

        //Vehicle 상태 표시 
        private void RefreshVehicleState()
        {

            if (vehicleObject.IsAlarmResetReport)
            {
                this.buttonAlarmResetReport.BackColor = Color.Lime;
            }
            else
            {
                this.buttonAlarmResetReport.BackColor = Color.DarkGreen;
            }

            if (vehicleObject.IsAlarmResetReportAck)
            {
                this.buttonAlarmResetReportACK.BackColor = Color.Lime;
            }
            else
            {
                this.buttonAlarmResetReportACK.BackColor = Color.DarkGreen;
            }

            if (vehicleObject.IsAlarmSetReport)
            {
                this.buttonAlarmSetReport.BackColor = Color.Lime;
            }
            else
            {
                this.buttonAlarmSetReport.BackColor = Color.DarkGreen;
            }

            if (vehicleObject.IsAlarmSetReportAck)
            {
                this.buttonAlarmSetReportACK.BackColor = Color.Lime;
            }
            else
            {
                this.buttonAlarmSetReportACK.BackColor = Color.DarkGreen;
            }
            if (vehicleObject.IsLoadComplete)
            {
                this.buttonLoadComplete.BackColor = Color.Lime;
            }
            else
            {
                this.buttonLoadComplete.BackColor = Color.DarkGreen;
            }

            if (vehicleObject.IsPalletExist)
            {
                this.buttonPalletExist.BackColor = Color.Lime;
            }
            else
            {
                this.buttonPalletExist.BackColor = Color.DarkGreen;
            }

            if (vehicleObject.IsTransferRequest)
            {
                this.buttonTransferRequest.BackColor = Color.Lime;
            }
            else
            {
                this.buttonTransferRequest.BackColor = Color.DarkGreen;
            }

            if (vehicleObject.IsUnloadComplete)
            {
                this.buttonUnloadComplete.BackColor = Color.Lime;
            }
            else
            {
                this.buttonUnloadComplete.BackColor = Color.DarkGreen;
            }
            this.labelFrom01.Text = vehicleObject.From01;
            this.labelLocation01.Text = vehicleObject.VehicleLocation01;
            this.labelTo01.Text = vehicleObject.To01;
            this.labelComplete01.Text = vehicleObject.TransferCompleteLocation01;
            this.labelAlarmCode.Text = vehicleObject.AlarmSetCode.ToString();
        }


        private void buttonYes_Click(object sender, EventArgs e)
        {
            timerRefresh.Stop();
            this.Hide();
        }

        private void label21_Click(object sender, EventArgs e)
        {

        }

        private void buttonForceComplete_Click(object sender, EventArgs e)
        {
            //this.buttonYes_Click(this, e);
            if (vehicleObject == null)
            {
                return;
            }

            if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.RUN)
            {
                Util.Util.ShowMesagePopup("RUN(작업중) 상태입니다. 작업이 완료될때 까지 기다리십시오.");
                return;
            }

            if (string.IsNullOrEmpty(vehicleObject.CommandSeq))
            {
                Util.Util.ShowMesagePopup("[완료처리]할 명령이 없습니다.");
                return;
            }

            MovementManager manager = Naming.GetMovementManager();
            manager.chageTransferDetailComplete(vehicleObject.Commandid, vehicleObject.CommandSeq);
            vehicleObject.Commandid = "";
            vehicleObject.CommandSeq = "";
            vehicleObject.AgvBinaryCommandId = "";
            vehicleObject.AgvCommandResetCount = 0;

            Util.Util.ShowMesagePopup("[ " + vehicleObject.Commandid + " - " + vehicleObject.CommandSeq + "] 명령을 완료하였습니다.");
        }

        private void buttonAbnormalComplete_Click(object sender, EventArgs e)
        {
            //this.buttonYes_Click(this, e);
            if (vehicleObject == null)
            {
                return;
            }

            if (vehicleObject.AgvVehicleState == Vehicle.AGV_RUN_STATE.RUN)
            {
                Util.Util.ShowMesagePopup("RUN(작업중) 상태입니다. 작업이 완료될때 까지 기다리십시오.");
                return;
            }

            if (string.IsNullOrEmpty(vehicleObject.Commandid))
            {
                Util.Util.ShowMesagePopup("[이상종료]처리할 명령이 없습니다.");
                return;
            }
            if (string.IsNullOrEmpty(vehicleObject.CommandSeq))
            {
                Util.Util.ShowMesagePopup("[이상종료]처리할 명령이 없습니다.");
                return;
            }

            vehicleObject.Commandid = "";
            vehicleObject.CommandSeq = "";
            vehicleObject.AgvBinaryCommandId = "";
            vehicleObject.AgvCommandResetCount = 0;

            Util.Util.ShowMesagePopup("[이상종료]처리를 완료하였습니다.");
        }

        private void buttonRetry_Click(object sender, EventArgs e)
        {
            //this.buttonYes_Click(this, e);
            if (string.IsNullOrEmpty(vehicleObject.Commandid))
            {
                Util.Util.ShowMesagePopup("재 전송 가능한 명령이 없습니다.");
                return;
            }
            if (string.IsNullOrEmpty(vehicleObject.CommandSeq))
            {
                Util.Util.ShowMesagePopup("재 전송 가능한 명령이 없습니다.");
                return;
            }

            //Pallet이 원래 목적지에 있는 경우에만 처리해야 한다.
            TransferData transferData = MovementManager.getTransferData(vehicleObject.Commandid);
            if (transferData == null)
            {
                Util.Util.ShowMesagePopup("재 전송 가능한 명령이 없습니다.");
                return;
            }

            TransferDetailData transferDetail = new TransferDetailData();
            transferDetail.getTransferDetailData(vehicleObject.Commandid, vehicleObject.CommandSeq);
            if (transferDetail == null)
            {
                Util.Util.ShowMesagePopup("재 전송 가능한 명령이 없습니다.");
                return;
            }

            transferDetail.Transferdetailstatus = "Wait";
            transferDetail.Palletidtomove = transferDetail.Palletid;
            transferDetail.Palletid = "";
            if (string.IsNullOrEmpty(transferDetail.Palletidtomove))
            {
                transferDetail.Palletidtomove = transferData.Palletid;
            }
            transferDetail.Completetime = "";
            transferDetail.Executetime = "";
            transferDetail.updateTransferDetailData();

            //HISTORY Backup LGLS를 위해 추가함.
            try
            {
                transferDetail.insertTransferDetailDataBackup();
            }
            catch (Exception ex)
            {
                //throw ex;
            }

            vehicleObject.CommandSeq = "";
            vehicleObject.Commandid = "";
            vehicleObject.AgvBinaryCommandId = "";
            vehicleObject.AgvCommandResetCount = 0;
            Util.Util.ShowMesagePopup("명령을 재 전송하였습니다");
        }







    }
}