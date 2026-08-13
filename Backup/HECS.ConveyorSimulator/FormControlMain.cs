using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using HECS.ConveyorSimulator.Devices;
using System.Threading;
using HECS.ConveyorSimulator.Controls;
using ECP.Device.Observe;
using ECP.Boot;
using HECS.ComX.Sockets;

namespace HECS.ConveyorSimulator
{
    public partial class FormControlMain : FormBase
    {
        HECSClientSocket socket = new HECSClientSocket();

        Thread threadCycleTest = null;
        public FormControlMain()
        {
            InitializeComponent();
            InitializeForm();
        }

        protected void InitializeForm()
        {
            //// textBoxSpeed.Text = "100";
            //comboBoxConveyortList.Items.Clear();
            //foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
            //{
            //    comboBoxConveyortList.Items.Add(conveyor.DeviceId + "," + conveyor.HostId);
            //}
            InitializeTabpage();
            LoadConveyorControl();
            LoadStockerControl();
            LoadRGVControl();
            //LoadLiftControl();
            //LoadAGVControl();
        }


        private void InitializeTabpage()
        {
            this.tabControl.TabPages.Clear();

            this.tabControl.TabPages.Add("Conveyor", "Conveyor");
            TabPage conveyorTabPage = this.tabControl.TabPages["Conveyor"];
            FlowLayoutPanel conveyorPanel = new FlowLayoutPanel();
            conveyorPanel.AutoScroll = true;
            conveyorPanel.Padding = new Padding(1, 1, 1, 1);
            conveyorTabPage.Controls.Add(conveyorPanel);
            conveyorPanel.Dock = DockStyle.Fill;


            this.tabControl.TabPages.Add("Stocker", "Stocker");
            TabPage stockerTabPage = this.tabControl.TabPages["Stocker"];
            FlowLayoutPanel stockerPanel = new FlowLayoutPanel();
            stockerPanel.AutoScroll = true;
            stockerPanel.Padding = new Padding(1, 1, 1, 1);
            stockerTabPage.Controls.Add(stockerPanel);
            stockerPanel.Dock = DockStyle.Fill;



            this.tabControl.TabPages.Add("RGV", "RGV");
            TabPage rgvTabPage = this.tabControl.TabPages["RGV"];
            FlowLayoutPanel rgvPanel = new FlowLayoutPanel();
            rgvPanel.AutoScroll = true;
            rgvPanel.Padding = new Padding(1, 1, 1, 1);
            rgvTabPage.Controls.Add(rgvPanel);
            rgvPanel.Dock = DockStyle.Fill;

            //this.tabControl.TabPages.Add("Lift", "Lift");
            //TabPage liftTabPage = this.tabControl.TabPages["Lift"];
            //FlowLayoutPanel liftPanel = new FlowLayoutPanel();
            //liftPanel.AutoScroll = true;
            //liftPanel.Padding = new Padding(1, 1, 1, 1);
            //liftTabPage.Controls.Add(liftPanel);
            //liftPanel.Dock = DockStyle.Fill;
        }
        private void LoadStockerControl()
        {
            FlowLayoutPanel container = (FlowLayoutPanel)this.tabControl.TabPages["Stocker"].Controls[0];
            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                if (vehicle.Ownerid.Contains("STKSEM"))
                {
                    VehicleControl control = new VehicleControl();
                    control.VehicleId = vehicle.DeviceId;
                    control.Size = new Size(344, 244);
                    container.Controls.Add(control);

                    Dictionary<string, Conveyor> conveyorDictionary = DeviceManager.GetConverByOwnerID(vehicle.Ownerid);
                    foreach (Conveyor conveyor in conveyorDictionary.Values)
                    {
                        if (conveyor.DeviceId.EndsWith("S"))
                        {
                            continue;
                        }
                        ConveyorControl conveyorControl = new ConveyorControl();
                        conveyorControl.ConveyorId = conveyor.DeviceId;
                        conveyorControl.Size = new Size(309, 202);
                        container.Controls.Add(conveyorControl);
                    }
                }

            }
        }

        private void LoadRGVControl()
        {
            FlowLayoutPanel container = (FlowLayoutPanel)this.tabControl.TabPages["RGV"].Controls[0];
            foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            {
                if (vehicle.Ownerid.Contains("RGVSEM"))
                {
                    VehicleControl control = new VehicleControl();
                    control.VehicleId = vehicle.DeviceId;
                    control.Size = new Size(344, 244);
                    container.Controls.Add(control);
                    Dictionary<string, Conveyor> conveyorDictionary = DeviceManager.GetConverByOwnerID(vehicle.Ownerid);
                    foreach (Conveyor conveyor in conveyorDictionary.Values)
                    {
                        ConveyorControl conveyorControl = new ConveyorControl();
                        conveyorControl.ConveyorId = conveyor.DeviceId.Substring(0, conveyor.DeviceId.Length - 1);
                        conveyorControl.Size = new Size(309, 202);
                        container.Controls.Add(conveyorControl);
                    }
                }
            }
        }
        private void LoadAGVControl()
        {
            //Panel container = (Panel)this.tabControl.TabPages["AGV"].Controls[0];
            //AGVSEMControl control = new AGVSEMControl();
            //DeviceManager.AgvSEMControl = control;
            //control.Size = new Size(800, 600);
            //control.Dock = DockStyle.Fill;
            //container.Controls.Add(control);
        }
        private void LoadLiftControl()
        {
            //FlowLayoutPanel container = (FlowLayoutPanel)this.tabControl.TabPages["Lift"].Controls[0];
            //foreach (Vehicle vehicle in DeviceManager.Vehicles.Values)
            //{
            //    if (vehicle.Ownerid.Contains("LIFTSEM"))
            //    {
            //        VehicleControl control = new VehicleControl();
            //        control.VehicleId = vehicle.DeviceId;
            //        control.Size = new Size(344, 244);
            //        container.Controls.Add(control);
            //        Dictionary<string, Conveyor> conveyorDictionary = DeviceManager.GetConverByOwnerID(vehicle.Ownerid);
            //        foreach (Conveyor conveyor in conveyorDictionary.Values)
            //        {
            //            ConveyorControl conveyorControl = new ConveyorControl();
            //            conveyorControl.ConveyorId = conveyor.DeviceId.Substring(0, conveyor.DeviceId.Length - 1);
            //            conveyorControl.Size = new Size(309, 202);
            //            container.Controls.Add(conveyorControl);
            //        }
            //    }
            //}
        }

        private void LoadConveyorControl()
        {
            FlowLayoutPanel container = (FlowLayoutPanel)this.tabControl.TabPages["Conveyor"].Controls[0];
            foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
            {
                if (conveyor.Observables.Count == 0)
                {
                    continue;
                }

                if (conveyor.DeviceId.Split(':').Length != 2)
                {
                    continue;
                }
                else
                {
                    int convertResult = 0;
                    if (int.TryParse(conveyor.DeviceId.Split(':')[1], out convertResult) == false)
                    {
                        continue;
                    }
                }

                ConveyorControl control = new ConveyorControl();
                control.ConveyorId = conveyor.DeviceId;
                control.Size = new Size(309, 202);
                container.Controls.Add(control);
            }


        }

        private void buttonInitialize_Click(object sender, EventArgs e)
        {
            System.Console.WriteLine(DeviceManager.Conveyors.Count);
        }

        private void buttonCycleTest_Click(object sender, EventArgs e)
        {
            if (threadCycleTest == null)
            {
                threadCycleTest = new Thread(new ThreadStart(CycleTest));
                threadCycleTest.Start();
            }
            if (threadCycleTest.ThreadState == ThreadState.Suspended)
            {

                threadCycleTest.Resume();


            }
            else
            {
                threadCycleTest.Suspend();
            }
        }

        private void CycleTest()
        {
            while (true)
            {
                foreach (Conveyor conveyor in DeviceManager.Conveyors.Values)
                {
                    if (conveyor.Observables.ContainsKey("OPERATION_MODE") && (bool)conveyor.Observables["OPERATION_MODE"].Value == false)
                    {
                        WriteObservable(conveyor, "OPERATION_MODE", true);
                    }
                    else
                    {
                        WriteObservable(conveyor, "OPERATION_MODE", false);
                    }
                    if (conveyor.Observables.ContainsKey("PALLET_EXIST_FLAG_01") && (bool)conveyor.Observables["PALLET_EXIST_FLAG_01"].Value == false)
                    {
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_01", true);
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_02", true);
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_03", true);
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_04", true);
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_05", true);
                    }
                    else
                    {
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_01", false);
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_02", false);
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_03", false);
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_04", false);
                        WriteObservable(conveyor, "PALLET_EXIST_FLAG_05", false);
                    }

                    if (conveyor.Observables.ContainsKey("LOAD_COMPLETE_01") && (bool)conveyor.Observables["LOAD_COMPLETE_01"].Value == false)
                    {
                        WriteObservable(conveyor, "LOAD_COMPLETE_01", true);
                    }
                    else
                    {
                        WriteObservable(conveyor, "LOAD_COMPLETE_01", false);
                    }
                    if (conveyor.Observables.ContainsKey("LOAD_COMPLETE_02") && (bool)conveyor.Observables["LOAD_COMPLETE_02"].Value == false)
                    {
                        WriteObservable(conveyor, "LOAD_COMPLETE_02", true);
                    }
                    else
                    {
                        WriteObservable(conveyor, "LOAD_COMPLETE_02", false);
                    }
                    if (conveyor.Observables.ContainsKey("UNLOAD_COMPLETE_01") && (bool)conveyor.Observables["UNLOAD_COMPLETE_01"].Value == false)
                    {
                        WriteObservable(conveyor, "UNLOAD_COMPLETE_01", true);
                    }
                    else
                    {
                        WriteObservable(conveyor, "UNLOAD_COMPLETE_01", false);
                    }
                    if (conveyor.Observables.ContainsKey("UNLOAD_COMPLETE_02") && (bool)conveyor.Observables["UNLOAD_COMPLETE_02"].Value == false)
                    {
                        WriteObservable(conveyor, "UNLOAD_COMPLETE_02", true);
                    }
                    else
                    {
                        WriteObservable(conveyor, "UNLOAD_COMPLETE_02", false);
                    }

                    if (conveyor.Observables.ContainsKey("WAIT_IN") && (bool)conveyor.Observables["WAIT_IN"].Value == false)
                    {
                        WriteObservable(conveyor, "WAIT_IN", true);
                    }
                    else
                    {
                        WriteObservable(conveyor, "WAIT_IN", false);
                    }
                    if (conveyor.Observables.ContainsKey("WAIT_OUT") && (bool)conveyor.Observables["WAIT_OUT"].Value == false)
                    {
                        WriteObservable(conveyor, "WAIT_OUT", true);
                    }
                    else
                    {
                        WriteObservable(conveyor, "WAIT_OUT", false);
                    }
                }

                string[] inputPort = new string[] { "63", "2", "14", "16", "27", "31", "73", "77", "60", "58", "56", "53", "49", "42", "46" };
                foreach (string portid in inputPort)
                {
                    Conveyor targetConveyor = DeviceManager.GetConveyorHasPort("PORT:" + portid);
                    if (targetConveyor == null)
                    {
                        continue;
                    }
                    if (targetConveyor.Observables.ContainsKey("IN_READY_02") && (bool)targetConveyor.Observables["IN_READY_02"].Value == false)
                    {
                        WriteObservable(targetConveyor, "IN_READY_02", true);
                    }
                    else
                    {
                        WriteObservable(targetConveyor, "IN_READY_02", false);
                    }
                }


                Thread.Sleep(5000);
            }
        }
        private static void WriteObservable(Conveyor conveyor, string key, object value)
        {
            if (conveyor.Observables.ContainsKey(key))
            {
                conveyor.Observables[key].Value = value;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {


            System.Console.WriteLine("YYYYYYY:" + socket.RegisteredObservable.Count);

            //packet.Direction = "1";
            //packet.CmdID = "601";
            //packet.ObjID = "20090409125959001";
            HECSPacket packet = new HECSPacket(22);
            packet.Header = "0060120090409125959001";
            packet.Body = "000100010100000101050001".PadRight(50);
            packet.BodyList.Add("1", packet.Body);
            packet.CheckSum = "74";
            //socket.Write(packet);

            System.Console.WriteLine("SocketState:" + socket.socketState.ToString());


            socket.Write(packet);

        }

        private void buttonInitialize_Click_1(object sender, EventArgs e)
        {

        }
    }
}