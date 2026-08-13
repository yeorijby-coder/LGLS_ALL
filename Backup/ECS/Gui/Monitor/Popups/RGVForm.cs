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
    public partial class RGVForm : DialogBaseForm
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
        public RGVForm()
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
                    this.labelVehicleName.Text = "RGV1";
                    this.labelVehicleDescription.Text = "(RGV 1호차)";
                    break;
            }
            if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.DOWN)
            {
                this.labelVehicleStatus.Text = "DOWN";
                this.labelVehicleStatus.GradientColorOne = Color.Red;
                this.labelVehicleStatus.GradientColorTwo = Color.Red;
            }
            else if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.IDLE)
            {
                this.labelVehicleStatus.Text = "IDLE";
                this.labelVehicleStatus.GradientColorOne = Color.Yellow;
                this.labelVehicleStatus.GradientColorTwo = Color.Yellow;
            }
            else if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.RUN)
            {
                this.labelVehicleStatus.Text = "RUN";
                this.labelVehicleStatus.GradientColorOne = Color.Lime;
                this.labelVehicleStatus.GradientColorTwo = Color.Lime;
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
            if (vehicleObject == null)
            {
                return ;
            }
            if (string.IsNullOrEmpty(vehicleObject.Commandid) || string.IsNullOrEmpty(vehicleObject.CommandSeq))
            {
                if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.IDLE)
                {
                    labelCommandStatus.Text = "대기";
                }
                else if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.DOWN)
                {
                    labelCommandStatus.Text = "에러";
                }
                else if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.RUN)
                {
                    labelCommandStatus.Text = "작업중";
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

            if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.IDLE)
            {
                labelAdvice.Text = "입/출고 명령이 완료되지 않았습니다. [완료처리]하세요!!";
            }

            if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.DOWN)
            {
                labelAdvice.Text = "크레인의 상태를 확인하고, [명령재전송] 또는 매뉴얼처리하세요!!";
            }

            if (transferData.Tosublocationid.StartsWith("PORT"))
            {
                labelCommandStatus.Text = "출고";
            }
            else
            {
                labelCommandStatus.Text = "입고";
            }

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
            this.labelFrom02.Text = vehicleObject.From02;
            this.labelFrom03.Text = vehicleObject.From03;

            this.labelLocation01.Text = vehicleObject.VehicleLocation01;
            this.labelLocation02.Text = vehicleObject.VehicleLocation02;
            this.labelLocation03.Text = vehicleObject.VehicleLocation03;

            this.labelTo01.Text = vehicleObject.To01;
            this.labelTo02.Text = vehicleObject.To02;
            this.labelTo03.Text = vehicleObject.To03;

            this.labelComplete01.Text = vehicleObject.TransferCompleteLocation01;
            this.labelComplete02.Text = vehicleObject.TransferCompleteLocation02;
            this.labelComplete03.Text = vehicleObject.TransferCompleteLocation03;

            this.labelAlarmCode.Text = vehicleObject.AlarmSetCode.ToString();
        
            if (vehicleObject.IsAvailible == true)
            {
                checkBoxAvailible.Checked = false;
            }
            else
            {
                checkBoxAvailible.Checked = true;
            }
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

            if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.RUN)
            {
                Util.Util.ShowMesagePopup("RUN(작업중) 상태입니다. 작업이 완료될때 까지 기다리십시오.");
                return;
            }
            if (string.IsNullOrEmpty(vehicleObject.Commandid))
            {
                Util.Util.ShowMesagePopup("[완료처리]할 명령이 없습니다.");
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

            Util.Util.ShowMesagePopup("[ " + vehicleObject.Commandid + " - " + vehicleObject.CommandSeq + "] 명령을 완료하였습니다.");
        }

        private void buttonAbnormalComplete_Click(object sender, EventArgs e)
        {
            //this.buttonYes_Click(this, e);
            if (vehicleObject == null)
            {
                return;
            }

            if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.RUN)
            {
                Util.Util.ShowMesagePopup("RUN(작업중) 상태입니다. 작업완료후 [이상종료]처리를 하십시오.");
                return;
            }
            if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.DOWN)
            {
                Util.Util.ShowMesagePopup("DOWN(작업중단) 상태에서는 [이상종료]처리를 할 수 없습니다.");
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
            if (vehicleObject.VehicleState == Vehicle.VEHICLE_STATE.DOWN)
            {
                Util.Util.ShowMesagePopup("[온라인]으로 전환하십시오.");
                return;
            }

            ItineraryData itinerary = new ItineraryData();
            itinerary.getItineraryData(transferDetail.Pathid);

            Port fromPort = ECSDeviceManager.GetPort(itinerary.Fromsublocationid);
            Port toPort = ECSDeviceManager.GetPort(itinerary.Tosublocationid);

            vehicleObject.IO_PALLET_ID = transferData.Palletid;
            vehicleObject.IO_FROM_01 = "00";
            vehicleObject.IO_FROM_02 = "00";
            vehicleObject.IO_FROM_03 = fromPort.ElementId.Split(':')[1];
            vehicleObject.IO_TO_01 = "00";
            vehicleObject.IO_TO_02 = "00";
            vehicleObject.IO_TO_03 = toPort.ElementId.Split(':')[1];
            vehicleObject.IO_TRANSFER_REQUEST = true;

            //transferDetail.Transferdetailstatus = "Wait";
            //transferDetail.Palletidtomove = transferDetail.Palletid;
            //transferDetail.Palletid = "";
            //if (string.IsNullOrEmpty(transferDetail.Palletidtomove))
            //{
            //    transferDetail.Palletidtomove = transferData.Palletid;
            //}
            //transferDetail.Completetime = "";
            //transferDetail.Executetime = "";
            //transferDetail.updateTransferDetailData();
            //vehicleObject.CommandSeq = "";
            //vehicleObject.Commandid = "";
            Util.Util.ShowMesagePopup("명령을 재 전송하였습니다");
        }

        private void checkBoxAvailible_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxAvailible.Checked)
            {
                vehicleObject.IsAvailible = false;
            }
            else
            {
                vehicleObject.IsAvailible = true;
            }
        }







    }
}