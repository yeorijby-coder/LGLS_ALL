using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace HECS
{
    public class Splasher
    {
        static SplashForm splashForm = new SplashForm();
        static Thread splashThread = null;

        //	internally used as a thread function - showing the form and
        //	starting the messageloop for it
        static void ShowThread()
        {
            //splashForm = new SplashForm();
            Application.Run(splashForm);
        }

        //	public Method to show the SplashForm
        static public void Show()
        {
            if (splashThread != null)
                return;

            splashThread = new Thread(new ThreadStart(Splasher.ShowThread));
            splashThread.IsBackground = true;
            splashThread.Start();
            Thread.Sleep(100);
            Splasher.Status = "-----------------------------------------";
            Thread.Sleep(100);
            Splasher.Status = " LG 생명과학 ezMCS";
            Thread.Sleep(100);
            Splasher.Status = "-----------------------------------------";
            Thread.Sleep(100);
            Splasher.Status = "Stocker Controller 구동환경을 체크합니다.";

        }

        //	public Method to hide the SplashForm
        static public void Close()
        {
            Thread.Sleep(3000);
            if (splashThread == null) return;
            if (splashForm == null) return;

            try
            {
                splashForm.Invoke(new MethodInvoker(splashForm.Hide));
                splashForm.Invoke(new MethodInvoker(splashForm.Close));
            }
            catch (Exception ex)
            {
                System.Console.WriteLine("Splasher:" + ex.StackTrace);
            }
            splashThread = null;
            splashForm = null;
        }

        //	public Method to set or get the loading Status
        static public string Status
        {
            set
            {
                if (splashForm == null)
                {
                    return;
                }

                splashForm.StatusInfo = value;
            }
            get
            {
                if (splashForm == null)
                {
                    throw new InvalidOperationException("Splash Form not on screen");
                }
                return splashForm.StatusInfo;
            }
        }
    }
}
