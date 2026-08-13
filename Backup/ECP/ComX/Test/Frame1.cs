using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Globalization;

using System.Threading;
using System.Reflection;
using System.Runtime.Remoting;

using ECP.ComX.Secs.Hsms;
using ECP.ComX.Secs.Secs2;
using ECP.ComX.Secs.Secs2.Gem;
using ECP.ComX.Prj;
using ECP.ComX.Secs;
using ECP.ComX.Secs.Secs1;


namespace ECP.ComX.Test
{
    public partial class Frame1 : Form
    {
        public const string CLASS = "Frame1";

        private static string DATE_FORMAT_LOGDATA = "yyyy-MMM-dd HH:mm:ss:fff";
        private static string DATE_FORMAT_FILE = "yyyy-MMM-dd-HHmmssfff";


     
        private const String CONFIG_FILE_NAME = "emu.cfg";

        private static String packageName = ECP.ComX.Secs.Secs2.Name.GetPackagePrefix( "ECP.ComX.Test.Frame1" );       

        private static int sendThreadCount = 0;

        public Logger logger = null;

        public Connection connection = null;

        SecsMessageTransfer smt = null;

        Object tool = null;

        SendMessageThread lastSentMessageThread = null;


        int stressDelayTime = 0;


        Thread stressThread = null;

        bool isStressRunning = false;

        public bool IsStressRunning
        {
            get { return isStressRunning; }
            set { isStressRunning = value; }
        }


        public Frame1()
        {
            InitializeComponent();


            this.SetStyle(ControlStyles.OptimizedDoubleBuffer |
                          ControlStyles.AllPaintingInWmPaint |
                          ControlStyles.UserPaint, true);

            Initialize();

            logger = new Logger(this);
        }

        protected void Initialize()
        {
            this.comboBoxTool.SelectedItem = this.comboBoxTool.Items[0];


        }


        private void Frame1_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.IsStressRunning = false;
            
            this.connection.Dispose();
        }

     

        protected override bool ProcessDialogKey(Keys keyData)
        {
            bool bRet = false;
            switch (keyData)
            {
                case Keys.Up:
                case Keys.Down:
                case Keys.Left:
                case Keys.Right:
                    bRet = true;
                    break;
                default:
                    bRet = base.ProcessDialogKey(keyData);
                    break;
            }
            return bRet;
        }

        public void Log(String text)
        {
            logger.Log(text);
        }

        private delegate void DelegateLog(String message);


        public void LogInvoke(String message)
        {
            try
            {
                this.BeginInvoke(new DelegateLog(LogImpl), message);
            }
            catch (InvalidOperationException)
            {
            }
        }


        public void LogImpl(String msg)
        {
            if (checkBoxLogToStdout.Checked)
            {
                Console.WriteLine(DateTime.Now.ToString(DATE_FORMAT_LOGDATA, DateTimeFormatInfo.InvariantInfo));
                Console.WriteLine(msg);
            }
            else
            {

               msg = msg.Replace("\n", Environment.NewLine);               

               textBoxLogArea.AppendText(msg+Environment.NewLine);

               if (textBoxLogArea.Lines.Length > 50)
               {
                   if (this.checkBoxAutoDeleteLog.Checked)
                   {
                       this.textBoxLogArea.Clear();
                       this.textBoxLogArea.Refresh();
                   }
                   //for (int i = 0; i < textBoxLogArea.Lines.Length-10; i++)
                   //textBoxLogArea.Text = textBoxLogArea.Text.Substring
                   //    (textBoxLogArea.Lines[0].Length + Environment.NewLine.Length);                 
               }                    

               textBoxLogArea.ScrollToCaret();  
            }
        }    


        public class Connection
        {

            HsmsConnection hc = null;
            Secs1Connection sc = null;
            MessageProcesser mp = null;
            Frame1 owner = null;
            
            public Connection(Frame1 owner, bool hsmsType, String host, int port,
                    bool active, int deviceId)
            {
                this.owner = owner;

                if (hsmsType)
                {
                    hc = new HsmsConnection("DEV", host, port, active,
                            new SecsMessageTransfer("DEV"), owner.logger);
                    hc.SetT3Timeout(45000);
                    mp = new MessageProcesser(owner);
                    hc.Connect();
                }
                else
                {
                    sc = new Secs1Connection("DEV", host, port, active,
                            new SecsMessageTransfer("DEV"), owner.logger);
                    mp = new MessageProcesser(owner);
                    sc.Connect();
                }
            }

            public void Dispose()
            {
                if (hc != null)
                {
                    hc.Dispose();
                    mp.Die();
                    hc = null;
                }
                else if (sc != null)
                {
                    sc.Dispose();
                    mp.Die();
                    sc = null;
                }
            }


           
        }


        public String Ask(String message)
        {

            return "";
        }

        public class SendMessageThread
        {
            Frame1 owner = null;
            public SecsMessageTransfer smt;
      
             Thread currentThread = null;

            string selectedMessageName = null;
            
            public SendMessageThread(Frame1 owner)
            {
                this.owner = owner;

                smt = new SecsMessageTransfer(  Convert.ToInt32(owner.textBoxSessionId.Text));
                smt.EstablishSendConnection("DEV");

                currentThread = new Thread(new ThreadStart(Run));

                this.currentThread.Name = "SendMessageThread" + Convert.ToString(sendThreadCount++);
                this.currentThread.IsBackground = true;
               // this.currentThread.Start();
            }

            public void Start()
            {
                this.currentThread.Start();
            }

            public void SetSelectedMessageName(string msgName)
            {
                this.selectedMessageName = msgName;
            }

            public void Run()
            {
                Object msgData = null;

                Secs2Message[] smArray = null;
                
                if( owner.tool != null )
                {
               
                    MethodInfo  fillMessageData;                    
                    try
                    {

                        fillMessageData = owner.tool.GetType().GetMethod("Build" + this.selectedMessageName);
                        msgData = fillMessageData.Invoke( owner.tool,null);


                        if( msgData == null )
                        {
                            return;
                        }
                        else if( msgData is Secs2Message )
                        {
                            smArray = new Secs2Message[]{ (Secs2Message)msgData };
                        }
                        else
                        {
                            smArray = (Secs2Message[])msgData;
                        }
                    }
                    catch( Exception ex )
                    {
                        owner.logger.Log(
                                "Method to build messages could not be found." );
                        return;
                    }

                    try
                    {
                        for( int j=0; j<1; j++ )
                        {
                            for( int i=0; i<smArray.Length; i++ )
                            {
                                smt.SendMessage( smArray[i] );
                            }
                        }

                       

                    }
                    catch( SecsException ex )
                    {
                        owner.logger.Log( ex.ToString() );
                        return;
                    }
                    
                }

                smt.Close();
            }

           

        }


        public class MessageProcesser
        {
            Frame1 owner = null;
            SecsMessageTransfer smt = new SecsMessageTransfer(-1);
            bool threadKilled = false;

            Thread currentThread = null;

            public MessageProcesser(Frame1 owner)
            {
                this.owner = owner;
                smt.EstablishConnection("DEV");                

                currentThread = new Thread(new ThreadStart(Run));
                this.currentThread.Name = "MessageProcesser";
                this.currentThread.IsBackground = true;
                this.currentThread.Start();
            }

            public void Run()
            {
                Secs2Message s2m, reply;

                while (!threadKilled)
                {
                    try
                    {
                        SecsMessage sm = smt.GetMessage(1000);
                        if( sm == null )
                        {
                            continue;
                        }


                        s2m = Secs2Message.NewGEMInstance( sm );

                        if( s2m == null || s2m.GetWbit() )
                        {
                            if( s2m == null )
                            {
                                s2m = new Secs2Message( sm );
                                reply = new S9F3( sm.GetSecsHeader() );
                       
                            
                            }
                            else
                            {
                                reply = s2m.GetSecondary();
                            }
                          
                            Type[] parameterTypes =
                                new Type[]
                                {
                                    typeof(SecsMessage),
                                    typeof(SecsMessage),
                                };


                            MethodInfo fillMessageData;


                            Object[] arguments = new Object[] {s2m, reply};


                            try
                            {
                                fillMessageData = owner.tool.GetType().GetMethod(
                                        s2m.GetDefaultName(), parameterTypes);
                                fillMessageData.Invoke(owner.tool, arguments);
                            }                            
                            catch (Exception e)
                            {
                                Console.WriteLine(e.StackTrace.ToString());
                            }
                            smt.SendMessage(reply);
                        }
                    }
                    catch( SecsException e )
                    {
                        owner.logger.Log( e.Message );
                        
                    }
                    catch( Exception e )
                    {
                        Console.WriteLine(e.StackTrace.ToString());
                    }
                }                
            }

            
            public void Die()
            {
                this.threadKilled = true;
                this.smt.Close();
            }
        }

       

        public class Logger : SecsLogger
        {
            Frame1 host = null;

            // Change flag if output to system.out is(not) needed
            String IN= "I<- ";
            String OUT="O-> ";

            public Logger(Frame1 host)
            {
                this.host = host;
                SetLoggingLevel( SecsLoggingLevel.ALL_LOGGING );
            }


            public void Log(String message)
            {
                message = "[ " + DateTime.Now.ToString() + " ]" + message;

                host.LogInvoke(message);
            }
    

            

            
            public override void Log( int machineId, String id, String message )
            {
                if( IsLogging( SecsLoggingLevel.ERROR_LOGGING ) )
                {
                    Log( "Err: " + id + ": " + message );
                }
            }

           
            public override void Log( int machineId, String id, String message,
                bool sent, byte[] comMessage )
            {
                if( IsLogging( SecsLoggingLevel.ERROR_LOGGING ) )
                {
                    Log( "Err: " + id + ": " + (sent?_out:_in) +
                        message );
                    PrintByteArray( comMessage );
                }
            }

            
            public override void Debug( int machineId, String id, String message )
            {
                if( IsLogging( SecsLoggingLevel.DEBUG_LOGGING ) )
                {
                    Log( "Dbg: " + id + ": " + message );
                }
            }

            
            public override void LogProtocol( int machineId, String id, bool sent, SecsMessage secsMessage )
            {
                if( IsLogging( SecsLoggingLevel.PROTOCOL_LOGGING ) )
                {
                    lock( host.textBoxLogArea )
                    {
                        Log( "Com: " + id + ":" +
                                    (sent?OUT:IN) +
                                    secsMessage.GetSecsHeader().ToString()  );
                    }
                }
            }

            
            public override void LogProtocol( int machineId, String id, bool sent, byte[] secsHeader )
            {
                if( IsLogging( SecsLoggingLevel.PROTOCOL_LOGGING ) )
                {
                    lock (host.textBoxLogArea)
                    {
                        SecsHeader sh = new SecsHeader( secsHeader );
                        Log( "Com: " + id + ":" +
                                    (sent?OUT:IN) + sh.ToString() );
                        PrintByteArray( sh.ToByteArray() );
                    }
                }
            }

            
            public override  void LogProtocol( int machineId, String id, bool sent, byte protocolByte )
            {
                if( IsLogging( SecsLoggingLevel.PROTOCOL_LOGGING ) )
                {
                    Log( "Com: " + id + ":" +
                                    (sent?OUT:IN) + protocolByte );
                }
            }

            
            public override void LogMessage( String id, bool sent, SecsMessage secsMessage )
            {
                if( IsLogging( SecsLoggingLevel.MESSAGE_LOGGING ) )
                {
                    lock (host.textBoxLogArea)
                    {
                        Secs2Message s2m;
                        if( secsMessage is Secs2Message )
                        {
                            s2m = (Secs2Message)secsMessage;
                        }
                        else
                        {
                            s2m = new Secs2Message( secsMessage );
                        }
                        Log( "Com: " + id + " " +
                                (sent?OUT:IN) +
                                (host.checkBoxIndentedLog.Checked ?
                                s2m.ToIndentedSMLString() :
                                s2m.ToString()) );

                        if( host.checkBoxHexLogs.Checked )
                        {
                            PrintByteArray(s2m.ToByteArray());
                        }


                       
                    }
                }
            }

            
            private void PrintByteArray( byte[] byteArray )
            {             
                StringBuilder sb = new StringBuilder();
            
                for( int i=0; i<byteArray.Length; i++ )
                {
                    if( i!=0 && i%20==0 )
                    {
                        Log( sb.ToString() );
                        sb = new StringBuilder();
                    }

                    int b = byteArray[i] & 0xFF;

                    if( b < 16 )
                    {
                        sb.Append( "0" );
                    }
                    sb.Append(String.Format("{0:X} ", b));
                }
                Log( sb.ToString() );
            }
        }

        private void checkBoxConnect_CheckStateChanged(object sender, EventArgs e)
        {
            String host;

            comboBoxSendMessage.Items.Clear();

            if( this.checkBoxConnect.Checked == true && tool == null )
            {
            
                String className = packageName + comboBoxTool.SelectedItem;
                    
                Console.WriteLine(className);


                Assembly asembly = null;

                try
                {
                    asembly = Assembly.Load("ECP");
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.StackTrace.ToString());
                }


                Object[] args = new Object[] { this };


                try
                {
                    Type emulatorClass = asembly.GetType(className);
                    object obj = Activator.CreateInstance(emulatorClass, args);

                    tool = obj;
                    
                }
                catch(Exception ex)
                {
                    logger.Log("Failed to construct tool.");
                    Console.WriteLine(ex.Message.ToString());
                    return;
                }


                Type c = tool.GetType();


                MethodInfo[] mInfoes = c.GetMethods();

                for (int i = 0; i < mInfoes.Length; i++)
                {
                    String methodName = mInfoes[i].Name;


                    if (methodName.StartsWith("Build"))
                    {
                        comboBoxSendMessage.Items.Add(methodName.Substring(5));
                    }
                }


                comboBoxSendMessage.SelectedItem = comboBoxSendMessage.Items[0];
                comboBoxSendMessage.Invalidate();


                host = textBoxHost.Text;


                connection = new Connection(this, checkBoxHSMS.Checked, host, Convert.ToInt32(textBoxPort.Text), checkBoxActive.Checked, -1);

                // make message transfer that has unique id
                smt = new SecsMessageTransfer( Convert.ToInt32(textBoxSessionId.Text));
                smt.EstablishSendConnection( "DEV" );
            }
            else if( tool != null )
            {
                if( lastSentMessageThread != null )
                {
                    lastSentMessageThread.smt.Close();
                }

                if (this.stressThread != null)
                {
                    this.IsStressRunning = false;
                    this.stressThread.Join();
                }

                

                connection.Dispose();
                tool = null;
                smt.Close();
                smt = null;
            }
        }


        
        private delegate void DelegateStress(object sender, EventArgs e);


        public void buttonSend_ClickInvoke()
        {
            try
            {
                this.BeginInvoke(new DelegateStress(buttonSend_Click), null, null);
            }
            catch (InvalidOperationException)
            {
            }
        }


        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (comboBoxSendMessage.SelectedItem != null)
            {
                lastSentMessageThread = new SendMessageThread(this);
                lastSentMessageThread.SetSelectedMessageName(this.comboBoxSendMessage.SelectedItem.ToString());
                lastSentMessageThread.Start();
            }
            else
            {
                MessageBox.Show("Select Message To send");
            }
        }

        private void buttonClearTextBoard_Click(object sender, EventArgs e)
        {
            this.textBoxLogArea.Clear();
            this.textBoxLogArea.Refresh();
        }

        private void buttonStressStart_Click(object sender, EventArgs e)
        {
            if(connection != null)
            this.StartStressThread("Stress Thread");
        }

        private void RunStress()
        {
            
            while (IsStressRunning)
            {            

                this.buttonSend_ClickInvoke();
                if (!textBoxDelayTime.Text.Equals(string.Empty))
                {

                    try
                    {
                        lock (this.textBoxDelayTime)
                        {
                            Thread.Sleep(Convert.ToInt32(this.stressDelayTime));
                        }
                    }
                    catch (Exception e)
                    {

                    }
                }
            }
            
            this.stressThread = null;
        }

        

        public void StartStressThread(string name)
        {
            if (this.stressThread == null)
            {
                this.stressThread = new Thread(new ThreadStart(RunStress));
                this.stressThread.Name = name;
                this.stressThread.IsBackground = true;

                this.IsStressRunning = true;
                this.stressThread.Start();
            }
        }

        private void buttonStressStop_Click(object sender, EventArgs e)
        {
            if (this.stressThread != null)
            {
                this.IsStressRunning = false;
            }
        }

        private void textBoxDelayTime_TextChanged(object sender, EventArgs e)
        {
            if(!this.textBoxDelayTime.Text.Equals(string.Empty))
                this.stressDelayTime = Convert.ToInt32(this.textBoxDelayTime.Text);
        }

        

       

    }
}