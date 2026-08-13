using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using System.Reflection;
using System.Globalization;

using ECP.Util.Client;
using ECP.Service.EventBroker;
using ECP.Event.Device;
using ECP.Event;
using ECP.Util;

namespace ECP.Gui.Util
{
    public partial class EventViewerForm : Form
    {
        private const int DEFAULT_MAX_EVENTS = 300;
        
        int maxEvents = DEFAULT_MAX_EVENTS;

        private static string DATE_FORMAT_EVENTDATA = "yyyy-MMM-dd HH:mm:ss:fff";

        private object eventCount_monitor = new object();

        TreeNode rootNode = null;

        protected string currentSubscriptionSubject = "";

        private bool isPaused = false;


        WrapperRemoteEvent wre = new WrapperRemoteEvent();
  

        public EventViewerForm()
        {
            InitializeComponent();
            Init();
            this.CreateHandle();
           
        }

        public void Init()
        {
            rootNode = new TreeNode("Event", 1, 1);
            rootNode.ImageIndex = 0;
            rootNode.SelectedImageIndex = 0;
            
            rootNode.Name = "root";
            rootNode.Text = "Event";
            rootNode.Expand();
            
            this.eventTree.Nodes.Add(rootNode);

            this.SubscribeToEvent(">");

            this.eventSubjectTextField.Text = this.currentSubscriptionSubject;
            this.InsertTextNode("text", "Suject Changed to : "+ currentSubscriptionSubject);

        }
        public void InsertTextNode(string name, string text)
        {
            DateTime date = DateTime.Now;

            string timeStamp = date.ToString(DATE_FORMAT_EVENTDATA,
                        DateTimeFormatInfo.InvariantInfo);

            TreeNode textNode = new TreeNode();
            textNode.ImageIndex = 1;
            textNode.SelectedImageIndex = 1;

            textNode.Name = name;
            textNode.Text = timeStamp + " "+ text;
            rootNode.Nodes.Insert(0, textNode);
        }


        public TreeNode InsertEventNode(string name,  string text)
        {
            TreeNode eventNode = new TreeNode();           
            eventNode.ImageIndex = 0;            
            eventNode.SelectedImageIndex = 0;

            eventNode.Name = name; 
            eventNode.Text = text;

            rootNode.Nodes.Insert(0, eventNode);      
            return eventNode;
        }

        public void InsertEventPropertyNodes(TreeNode parentNode,
            string name, string text)
        {
            TreeNode propertyNode = new TreeNode();
            propertyNode.ImageIndex = 1;
            propertyNode.SelectedImageIndex = 1;

            propertyNode.Name = name;
            propertyNode.Text = text;

            parentNode.Nodes.Add(propertyNode);
        }

        public void RemoveOldestEventNode()
        {
            if(this.rootNode.LastNode != null)
            this.rootNode.LastNode.Remove();
          
        }

        public void Test()
        {
            for (int i = 0; i < 10; i++)
            {
                TreeNode eventNode = this.InsertEventNode("event"+i.ToString(),
                    "2008.03.21 Device.Subsystem.stockerPort"+i.ToString());
                for (int j = 0; j < 5; j++)
                {
                    this.InsertEventPropertyNodes(eventNode, "property"+j.ToString(),
                        "Carrier ID : CST No : "+j.ToString());
                }
            }
        }

        private void applyButton_Click(object sender, EventArgs e)
        {
            string newSubject = this.eventSubjectTextField.Text.Trim();

            if (!newSubject.Equals(this.currentSubscriptionSubject))
            {
                this.SubscribeToEvent(newSubject);
            }
        }

        protected void SubscribeToEvent(string subscriptionSubject)
        {

            if (!Helper.IsNullOrEmpty(this.currentSubscriptionSubject))
            {
                this.UnsubscribeFromEvents(this.currentSubscriptionSubject);
            }
            

            EventBrokerManager ebm = Naming.GetEventBrokerManager();

            RemoteEvent re = ebm.GetRemoteEventForSubscribe(subscriptionSubject);

            //WrapperRemoteEvent wre = new WrapperRemoteEvent();

            if (re == null) return;

            try
            {
                re.RemoteEventReceived += new RemoteEventHandler(wre.WrapperRemoteEventReceivedHandler);

            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message.ToString());
                Console.WriteLine(ex.StackTrace);
            }

            //Associate wrapper event with current form event handler.
            wre.WrapperRemoteEventReceived += new RemoteEventHandler(ReceiveECPEvent);
            this.currentSubscriptionSubject = subscriptionSubject;

            this.currectSubscriptionSubjectLabel.Text = subscriptionSubject;

            this.InsertTextNode("text", "Subject Changed to" + " " + subscriptionSubject);

        }

        public void UnsubscribeFromEvents(string subscriptionSubject)
        {
            EventBrokerManager ebm = Naming.GetEventBrokerManager();

            RemoteEvent re = ebm.GetRemoteEventForUnsubscribe(subscriptionSubject);

            //WrapperRemoteEvent wre = new WrapperRemoteEvent();

            if (re == null) return;

            try
            {
                re.RemoteEventReceived -= new RemoteEventHandler(wre.WrapperRemoteEventReceivedHandler);
                wre.WrapperRemoteEventReceived -= new RemoteEventHandler(ReceiveECPEvent);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message.ToString());
            }

        }

        private delegate void DelegateProcessingECPEvent(ECPEvent ecpEvent);

        public void ReceiveECPEvent(ECPEvent ecpEvent)
        {
            if (!this.isPaused)
            {
                this.BeginInvoke(new DelegateProcessingECPEvent(ProcessingECPEvent), ecpEvent);
            }
        }

        public void ProcessingECPEvent(ECPEvent ecpEvent)
        {
            Type type = ecpEvent.GetType();

            PropertyInfo[] pi = type.GetProperties();


            DateTime date = DateTime.Now;

            string timeStamp = date.ToString(DATE_FORMAT_EVENTDATA, 
                        DateTimeFormatInfo.InvariantInfo);
            string subject = ecpEvent.GetSubject();
            
            TreeNode eventNode = this.InsertEventNode("EventNode", timeStamp +" "+subject);

            foreach (PropertyInfo prop in pi)
            { 
                this.InsertEventPropertyNodes(eventNode, "EventPropertyNode", prop.Name +"::"+ 
                    prop.ToString()+ " "+ prop.GetValue(ecpEvent, null));
            }
    
            while (this.rootNode.Nodes.Count> maxEvents - 1)
            {
                this.RemoveOldestEventNode();
            }
        }
    

        private void TestEventViewerForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.UnsubscribeFromEvents(this.currentSubscriptionSubject);
            this.Dispose();
            this.Close();
        }

        private void ClearButton_Click(object sender, EventArgs e)
        {
            string message = "Are you sure you want to clear all events! ";
            string caption = "Event Viewer";

            MessageBoxButtons buttons = MessageBoxButtons.YesNo;
            DialogResult result;

            // Displays the MessageBox.

            result = MessageBox.Show(this, message, caption, buttons);

            if (result == DialogResult.Yes)
            {
                this.rootNode.Nodes.Clear();

            }
            else if (result == DialogResult.No) 
            {

            }
        }

        private void PauseButton_Click(object sender, EventArgs e)
        {
            string pausedText = this.isPaused ? "Viewer Resumed" : "Viewer Paused";

            this.InsertTextNode("text", pausedText);

            this.isPaused = !this.isPaused;

            string buttonText = this.isPaused ? "Resume" : "Pause";

            this.pauseButton.Text = buttonText;
        }
    }
}