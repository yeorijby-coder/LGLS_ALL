using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Service.Movement;
using ECP.Util.Server;
using HECS.Device.Unit;
using HECS.Device;
using ECP.Db.Entity;

namespace HECS.Gui.Widget
{
    public partial class TransferStatusWidget : UserControl
    {
        private Timer timer = new Timer();
        private MovementManager movementManager = null;
        public TransferStatusWidget()
        {
            InitializeComponent();

        }

        void timer_Tick(object sender, EventArgs e)
        {

            RefreshStackerCrane();
            RefreshRGV();
            RefreshAGV();
            RefreshLift();
        }

        private void RefreshRGV()
        {
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                if (vehicle.OwnerId.Contains("RGV") == false)
                {
                    continue;
                }
                if (vehicle.ElementId == "VEHICLE:6")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand05.Text = "";
                        this.labelBatch05.Text = "";
                        this.labelMaterial05.Text = "";
                        this.labelPallet05.Text = "";
                        this.labelFrom05.Text = "";
                        this.labelTo05.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand05.Text = "";
                            this.labelBatch05.Text = "";
                            this.labelMaterial05.Text = "";
                            this.labelPallet05.Text = "";
                            this.labelFrom05.Text = "";
                            this.labelTo05.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand05.Text = transfer.Commandid;
                        this.labelBatch05.Text = transfer.Batchno;
                        this.labelMaterial05.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet05.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom05.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelFrom05.Text = itinerary.Fromsublocationid;
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo05.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelTo05.Text = itinerary.Tosublocationid;
                        }
                    }
                }
            }
        }

        private void RefreshAGV()
        {
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                if (vehicle.OwnerId.Contains("AGV") == false)
                {
                    continue;
                }
                if (vehicle.ElementId == "VEHICLE:1")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand07.Text = "";
                        this.labelBatch07.Text = "";
                        this.labelMaterial07.Text = "";
                        this.labelPallet07.Text = "";
                        this.labelFrom07.Text = "";
                        this.labelTo07.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand07.Text = "";
                            this.labelBatch07.Text = "";
                            this.labelMaterial07.Text = "";
                            this.labelPallet07.Text = "";
                            this.labelFrom07.Text = "";
                            this.labelTo07.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand07.Text = transfer.Commandid;
                        this.labelBatch07.Text = transfer.Batchno;
                        this.labelMaterial07.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet07.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom07.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelFrom07.Text = itinerary.Fromsublocationid;
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo07.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelTo07.Text = itinerary.Tosublocationid;
                        }

                    }
                }
                else if (vehicle.ElementId == "VEHICLE:2")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand08.Text = "";
                        this.labelBatch08.Text = "";
                        this.labelMaterial08.Text = "";
                        this.labelPallet08.Text = "";
                        this.labelFrom08.Text = "";
                        this.labelTo08.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand08.Text = "";
                            this.labelBatch08.Text = "";
                            this.labelMaterial08.Text = "";
                            this.labelPallet08.Text = "";
                            this.labelFrom08.Text = "";
                            this.labelTo08.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand08.Text = transfer.Commandid;
                        this.labelBatch08.Text = transfer.Batchno;
                        this.labelMaterial08.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet08.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom08.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelFrom08.Text = itinerary.Fromsublocationid;
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo08.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelTo08.Text = itinerary.Tosublocationid;
                        }
                    }
                }
                else if (vehicle.ElementId == "VEHICLE:3")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand09.Text = "";
                        this.labelBatch09.Text = "";
                        this.labelMaterial09.Text = "";
                        this.labelPallet09.Text = "";
                        this.labelFrom09.Text = "";
                        this.labelTo09.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand09.Text = "";
                            this.labelBatch09.Text = "";
                            this.labelMaterial09.Text = "";
                            this.labelPallet09.Text = "";
                            this.labelFrom09.Text = "";
                            this.labelTo09.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand09.Text = transfer.Commandid;
                        this.labelBatch09.Text = transfer.Batchno;
                        this.labelMaterial09.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet09.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom09.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelFrom09.Text = itinerary.Fromsublocationid;
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo09.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelTo09.Text = itinerary.Tosublocationid;
                        }
                    }
                }
            }
        }

        private void RefreshLift()
        {
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                if (vehicle.OwnerId.Contains("LIFT") == false)
                {
                    continue;
                }
                if (vehicle.ElementId == "VEHICLE:7")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand06.Text = "";
                        this.labelBatch06.Text = "";
                        this.labelMaterial06.Text = "";
                        this.labelPallet06.Text = "";
                        this.labelFrom06.Text = "";
                        this.labelTo06.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand06.Text = "";
                            this.labelBatch06.Text = "";
                            this.labelMaterial06.Text = "";
                            this.labelPallet06.Text = "";
                            this.labelFrom06.Text = "";
                            this.labelTo06.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand06.Text = transfer.Commandid;
                        this.labelBatch06.Text = transfer.Batchno;
                        this.labelMaterial06.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet06.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom06.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelFrom06.Text = itinerary.Fromsublocationid;
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo06.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            this.labelTo06.Text = itinerary.Tosublocationid;
                        }

                    }
                }
            }
        }

        private void RefreshStackerCrane()
        {

            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                if (vehicle.OwnerId.Contains("STK") == false)
                {
                    continue;
                }
                if (vehicle.ElementId == "VEHICLE:11")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand01.Text = "";
                        this.labelBatch01.Text = "";
                        this.labelMaterial01.Text = "";
                        this.labelPallet01.Text = "";
                        this.labelFrom01.Text = "";
                        this.labelTo01.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand01.Text = "";
                            this.labelBatch01.Text = "";
                            this.labelMaterial01.Text = "";
                            this.labelPallet01.Text = "";
                            this.labelFrom01.Text = "";
                            this.labelTo01.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand01.Text = transfer.Commandid;
                        this.labelBatch01.Text = transfer.Batchno;
                        this.labelMaterial01.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet01.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom01.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Fromsublocationid.Length == 6)
                            {
                                this.labelFrom01.Text = transfer.Fromsublocationid.Substring(0, 2) + "-" + transfer.Fromsublocationid.Substring(2, 2) + "-" + transfer.Fromsublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelFrom01.Text = transfer.Fromsublocationid;
                            }
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo01.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Tosublocationid.Length == 6)
                            {
                                this.labelTo01.Text = transfer.Tosublocationid.Substring(0, 2) + "-" + transfer.Tosublocationid.Substring(2, 2) + "-" + transfer.Tosublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelTo01.Text = transfer.Tosublocationid;
                            }
                        }
                    }
                }
                else if (vehicle.ElementId == "VEHICLE:12")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand02.Text = "";
                        this.labelBatch02.Text = "";
                        this.labelMaterial02.Text = "";
                        this.labelPallet02.Text = "";
                        this.labelFrom02.Text = "";
                        this.labelTo02.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand02.Text = "";
                            this.labelBatch02.Text = "";
                            this.labelMaterial02.Text = "";
                            this.labelPallet02.Text = "";
                            this.labelFrom02.Text = "";
                            this.labelTo02.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand02.Text = transfer.Commandid;
                        this.labelBatch02.Text = transfer.Batchno;
                        this.labelMaterial02.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet02.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom02.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Fromsublocationid.Length == 6)
                            {
                                this.labelFrom02.Text = transfer.Fromsublocationid.Substring(0, 2) + "-" + transfer.Fromsublocationid.Substring(2, 2) + "-" + transfer.Fromsublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelFrom02.Text = transfer.Fromsublocationid;
                            }
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo02.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Tosublocationid.Length == 6)
                            {
                                this.labelTo02.Text = transfer.Tosublocationid.Substring(0, 2) + "-" + transfer.Tosublocationid.Substring(2, 2) + "-" + transfer.Tosublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelTo02.Text = transfer.Tosublocationid;
                            }
                        }
                    }
                }
                else if (vehicle.ElementId == "VEHICLE:13")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand03.Text = "";
                        this.labelBatch03.Text = "";
                        this.labelMaterial03.Text = "";
                        this.labelPallet03.Text = "";
                        this.labelFrom03.Text = "";
                        this.labelTo03.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand03.Text = "";
                            this.labelBatch03.Text = "";
                            this.labelMaterial03.Text = "";
                            this.labelPallet03.Text = "";
                            this.labelFrom03.Text = "";
                            this.labelTo03.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand03.Text = transfer.Commandid;
                        this.labelBatch03.Text = transfer.Batchno;
                        this.labelMaterial03.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet03.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom03.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Fromsublocationid.Length == 6)
                            {
                                this.labelFrom03.Text = transfer.Fromsublocationid.Substring(0, 2) + "-" + transfer.Fromsublocationid.Substring(2, 2) + "-" + transfer.Fromsublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelFrom03.Text = transfer.Fromsublocationid;
                            }
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo03.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Tosublocationid.Length == 6)
                            {
                                this.labelTo03.Text = transfer.Tosublocationid.Substring(0, 2) + "-" + transfer.Tosublocationid.Substring(2, 2) + "-" + transfer.Tosublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelTo03.Text = transfer.Tosublocationid;
                            }
                        }
                    }
                }
                else if (vehicle.ElementId == "VEHICLE:14")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand04.Text = "";
                        this.labelBatch04.Text = "";
                        this.labelMaterial04.Text = "";
                        this.labelPallet04.Text = "";
                        this.labelFrom04.Text = "";
                        this.labelTo04.Text = "";
                    }
                    else
                    {
                        TransferData transfer = MovementManager.getTransferData(vehicle.Commandid);
                        if (transfer == null)
                        {
                            this.labelCommand04.Text = "";
                            this.labelBatch04.Text = "";
                            this.labelMaterial04.Text = "";
                            this.labelPallet04.Text = "";
                            this.labelFrom04.Text = "";
                            this.labelTo04.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand04.Text = transfer.Commandid;
                        this.labelBatch04.Text = transfer.Batchno;
                        this.labelMaterial04.Text = transfer.Materialno.TrimStart('0');
                        this.labelPallet04.Text = transfer.Palletid;

                        ItineraryData itinerary = new ItineraryData();
                        itinerary.getItineraryData(transferDetail.Pathid);

                        if (itinerary.Fromsublocationid.Contains("PORT:"))
                        {
                            this.labelFrom04.Text = itinerary.Fromsublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Fromsublocationid.Length == 6)
                            {
                                this.labelFrom04.Text = transfer.Fromsublocationid.Substring(0, 2) + "-" + transfer.Fromsublocationid.Substring(2, 2) + "-" + transfer.Fromsublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelFrom04.Text = transfer.Fromsublocationid;
                            }
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo04.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Tosublocationid.Length == 6)
                            {
                                this.labelTo04.Text =  transfer.Tosublocationid.Substring(0, 2) + "-" + transfer.Tosublocationid.Substring(2, 2) + "-" + transfer.Tosublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelTo04.Text = transfer.Tosublocationid;
                            }
                        }
                    }
                }
            }
        }

        private void TransferStatusWidget_Load(object sender, EventArgs e)
        {
            if (this.DesignMode)
            {
                return;
            }
            movementManager = Naming.GetMovementManager();
            if (movementManager == null)
            {
                return;
            }
            timer.Tick += new EventHandler(timer_Tick);
            timer.Interval = 2000;
            timer.Enabled = true;
            timer.Start();
        }

        private void label63_Click(object sender, EventArgs e)
        {

        }

        private void labelCommand05_Click(object sender, EventArgs e)
        {

        }

        private void labelBatch05_Click(object sender, EventArgs e)
        {

        }

        private void labelMaterial05_Click(object sender, EventArgs e)
        {

        }

        private void labelPallet05_Click(object sender, EventArgs e)
        {

        }

        private void labelFrom05_Click(object sender, EventArgs e)
        {

        }

        private void labelTo05_Click(object sender, EventArgs e)
        {

        }
    }
}
