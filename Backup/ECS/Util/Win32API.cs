using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace HECS.Util
{
    public class Win32API
    {
        public const int WM_NCLBUTTONDOWN = 0xA1;
        public const int HT_CAPTION = 0x2;
        [DllImportAttribute("user32.dll")]
        public static extern int SendMessage(IntPtr hWnd, int Msg, int wParam, int lParam);
        [DllImportAttribute("user32.dll")]
        public static extern bool ReleaseCapture();

        [DllImport("kernel32.dll")]
        private static extern int GetPrivateProfileString(    // ini Read 함수
            String section,
            String key,
            String def,
            StringBuilder retVal,
            int size,
            String filePath);
        [DllImport("kernel32.dll")]
        private static extern long WritePrivateProfileString(  // ini Write 함수
                    String section,
                    String key,
                    String val,
                    String filePath);

        public static long IniWriteValue(String Section, String Key, String Value, string avsPath)
        {
            long returnValue = WritePrivateProfileString(Section, Key, Value, avsPath);
            return returnValue;
        }

        public static String IniReadValue(String Section, String Key, string avsPath)
        {
            int MAX_SIZE = 2000;
            StringBuilder temp = new StringBuilder(MAX_SIZE);

            int i = GetPrivateProfileString(Section, Key, "", temp, MAX_SIZE, avsPath);

            if (temp == null)
            {
                return null;
            }
            else
            {
                return temp.ToString();
            }
        }



    }
}
