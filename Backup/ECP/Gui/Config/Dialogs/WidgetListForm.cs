using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using ECP.Global.Gui;
using ECP.Gui.Widget;

namespace ECP.Gui.Config.Dialogs
{
    public partial class WidgetListForm : Form
    {
        ECPConfigPanel parent = null;
        Label beforeSelectedLabel = null;

        public WidgetListForm(ECPConfigPanel parent)
        {
            InitializeComponent();

            this.SetBounds(parent.Location.X + 10, parent.Location.Y + 200, this.Width, this.Height);

            this.parent = parent;
            LoadWidgetList();
            
        }

        private void LoadWidgetList()
        {
            //List<WidgetData> widgetDatas = new List<WidgetData>();
            //widgetDatas = parent.GetWidgetDatas();

            //foreach (WidgetData widgetData in widgetDatas)
            //{
            //    Label widgetListLabel = new Label();

            //    widgetListLabel.BorderStyle = BorderStyle.FixedSingle;
            //    widgetListLabel.Font = new Font("굴림", 9F, FontStyle.Bold, GraphicsUnit.Point, ((byte)(129)));
            //    widgetListLabel.Location = new Point(3, 0);
            //    widgetListLabel.Size = new Size(155, 15);
            //    widgetListLabel.Text = widgetData.elementId;
            //    widgetListLabel.TextAlign = ContentAlignment.MiddleCenter;
            //    widgetListLabel.BackColor = Color.LightGray;
            //    widgetListLabel.Click += new EventHandler(this.HandleWidgetListClickEvent);

            //    widgetListFlowPanel.Controls.Add(widgetListLabel);
            //}

            BaseWidget widget;

            for (int i = 0; i < parent.Controls.Count; i++)
            {
                Control c = parent.Controls[i];

                if (c is BaseWidget)
                {
                    widget = (BaseWidget)c;
                    
                    Label widgetListLabel = new Label();

                    widgetListLabel.BorderStyle = BorderStyle.FixedSingle;
                    widgetListLabel.Font = new Font("굴림", 9F, FontStyle.Bold, GraphicsUnit.Point, ((byte)(129)));
                    widgetListLabel.Location = new Point(3, 0);
                    widgetListLabel.Size = new Size(155, 15);
                    if (widget is TextWidget)
                    {
                        widgetListLabel.Text = widget.WidgetText;
                        widgetListLabel.Name = widget.ElementId;
                    }
                    else
                    {
                        widgetListLabel.Text = widget.ElementId;
                    }
                    widgetListLabel.TextAlign = ContentAlignment.MiddleCenter;
                    widgetListLabel.BackColor = Color.LightGray;
                    widgetListLabel.Click += new EventHandler(this.HandleWidgetListClickEvent);

                    widgetListFlowPanel.Controls.Add(widgetListLabel);
                }
            }
        }

        private void HandleWidgetListClickEvent(Object sender, EventArgs e)
        {
            Label selectedLabel = (Label)sender;
            BaseWidget widget;

            selectedLabel.BorderStyle = BorderStyle.Fixed3D;
            selectedLabel.BackColor = Color.Gray;

            if (this.beforeSelectedLabel != null)
            {
                this.beforeSelectedLabel.BorderStyle = BorderStyle.FixedSingle;
                this.beforeSelectedLabel.BackColor = Color.LightGray;
            }
            else
            {
                //do nothing
            }

            for (int i = 0; i < parent.Controls.Count; i++)
            {
                Control c = parent.Controls[i];

                if (c is BaseWidget)
                {
                    widget = (BaseWidget)c;

                    if (selectedLabel.Text.Equals(widget.ElementId))
                    {
                        parent.selection.Clear();
                        widget.Selected = true;
                        parent.selection.Add(widget);
                    }
                    else if (selectedLabel.Text.Equals(widget.WidgetText))
                    {
                        if (selectedLabel.Name.Equals(widget.ElementId))
                        {
                            parent.selection.Clear();
                            widget.Selected = true;
                            parent.selection.Add(widget);
                        }
                        else
                        {
                            widget.Selected = false;
                        }
                    }
                    else
                    {
                        widget.Selected = false;
                    }
                }
            }

            this.beforeSelectedLabel = selectedLabel;
        }

        private void WidgetListForm_FormClosed(object sender, FormClosedEventArgs e)
        {

        }

        
    }
}