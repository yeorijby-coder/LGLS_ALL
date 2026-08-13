using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using ECP.Service.Movement;
using ECP.Util.Client;
using HECS.Device.Unit;
using HECS.Device;
using ECP.Db.Entity;

namespace HECS.Gui.Widget
{
    public partial class TransferStatusWidget01 : UserControl
    {
        private Timer timer = new Timer();
        private MovementManager movementManager = null;

        public TransferStatusWidget01()
        {
            InitializeComponent();

        }

        void timer_Tick(object sender, EventArgs e)
        {

            RefreshStackerCrane();
            RefreshRGV();
        }

        private void RefreshRGV()
        {
            foreach (Vehicle vehicle in ECSDeviceManager.Vehicles.Values)
            {
                if (vehicle.OwnerId.Contains("RGV") == false)
                {
                    continue;
                }
                if (vehicle.ElementId == "VEHICLE:1")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand06.Text = "";
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
                            this.labelMaterial06.Text = "";
                            this.labelPallet06.Text = "";
                            this.labelFrom06.Text = "";
                            this.labelTo06.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand06.Text = transfer.Commandid;
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
                            this.labelMaterial01.Text = "";
                            this.labelPallet01.Text = "";
                            this.labelFrom01.Text = "";
                            this.labelTo01.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand01.Text = transfer.Commandid;
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
                            this.labelMaterial02.Text = "";
                            this.labelPallet02.Text = "";
                            this.labelFrom02.Text = "";
                            this.labelTo02.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand02.Text = transfer.Commandid;
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
                            this.labelMaterial03.Text = "";
                            this.labelPallet03.Text = "";
                            this.labelFrom03.Text = "";
                            this.labelTo03.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand03.Text = transfer.Commandid;
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
                            this.labelMaterial04.Text = "";
                            this.labelPallet04.Text = "";
                            this.labelFrom04.Text = "";
                            this.labelTo04.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand04.Text = transfer.Commandid;
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
                                this.labelTo04.Text = transfer.Tosublocationid.Substring(0, 2) + "-" + transfer.Tosublocationid.Substring(2, 2) + "-" + transfer.Tosublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelTo04.Text = transfer.Tosublocationid;
                            }
                        }
                    }
                }
                else if (vehicle.ElementId == "VEHICLE:15")
                {
                    if (string.IsNullOrEmpty(vehicle.Commandid))
                    {
                        this.labelCommand05.Text = "";
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
                            this.labelMaterial05.Text = "";
                            this.labelPallet05.Text = "";
                            this.labelFrom05.Text = "";
                            this.labelTo05.Text = "";
                            continue;
                        }
                        TransferDetailData transferDetail = new TransferDetailData();
                        transferDetail.getTransferDetailData(vehicle.Commandid, vehicle.CommandSeq);

                        this.labelCommand05.Text = transfer.Commandid;
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
                            if (transfer.Fromsublocationid.Length == 6)
                            {
                                this.labelFrom05.Text = transfer.Fromsublocationid.Substring(0, 2) + "-" + transfer.Fromsublocationid.Substring(2, 2) + "-" + transfer.Fromsublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelFrom05.Text = transfer.Fromsublocationid;
                            }
                        }

                        if (itinerary.Tosublocationid.Contains("PORT:"))
                        {
                            this.labelTo05.Text = itinerary.Tosublocationid.Replace("PORT:", "");
                        }
                        else
                        {
                            if (transfer.Tosublocationid.Length == 6)
                            {
                                this.labelTo05.Text =  transfer.Tosublocationid.Substring(0, 2) + "-" + transfer.Tosublocationid.Substring(2, 2) + "-" + transfer.Tosublocationid.Substring(4, 2);
                            }
                            else
                            {
                                this.labelTo05.Text = transfer.Tosublocationid;
                            }
                        }
                    }
                }
            }
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

        private void TransferStatusWidget01_Load(object sender, EventArgs e)
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
    }
}
