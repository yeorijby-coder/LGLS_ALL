using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using HECS.Device.Unit;
using System.Runtime.InteropServices;

namespace HECS.Gui.Widget
{
    public partial class EquipmentWidget : HECSBaseWidget
    {
        private string equipmentId = "";

        public string EquipmentId
        {
            get
            {
                return equipmentId;
            }
            set
            {
                equipmentId = value;
            }
        }
        public EquipmentWidget()
        {

            InitializeComponent();
            this.SetStyle(ControlStyles.SupportsTransparentBackColor
                    | ControlStyles.AllPaintingInWmPaint
                    | ControlStyles.OptimizedDoubleBuffer 
                    | ControlStyles.ContainerControl
                    | ControlStyles.ResizeRedraw , true);
        }

        private Conveyor conveyorObject = null;

        public Conveyor ConveyorObject
        {
            get
            {
                return conveyorObject;
            }
            set
            {
                conveyorObject = value;
                if (conveyorObject != null)
                {
                    if (ECP.Global.GlobalConstant.START_MODE == "ECS")
                    {
                        conveyorObject.PropertyChanged += new PropertyChangedEventHandler(Object_PropertyChanged);
                    }
                }
            }
        }

        void Object_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            Invalidate();
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            //base.OnPaint(e);
            if (conveyorObject != null)
            {
                if (conveyorObject.IsAvailible == true)
                {

                    if (conveyorObject.OperationMode == Conveyor.CONVEYOR_OPERATION_MODE.AUTO)
                    {
                        this.WidgetBackColor = Color.Lime;
                        this.WidgetForeColor = Color.Lime;
                        this.BackColor = Color.Lime;
                        this.BackColor = Color.Lime;
                    }
                    else
                    {
                        this.WidgetBackColor = Color.Red;
                        this.WidgetForeColor = Color.Red;
                        this.BackColor = Color.Red;
                        this.BackColor = Color.Red;
                    }
                }
                else
                {
                    this.WidgetBackColor = Color.Black;
                    this.WidgetForeColor = Color.Black;
                    this.BackColor = Color.Black;
                    this.BackColor = Color.Black;
                }
                
            }
        }

    }
}
