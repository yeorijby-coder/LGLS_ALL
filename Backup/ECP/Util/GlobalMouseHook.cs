using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace ECP.Util
{
    /*
    public class GlobalMouseHook
    {
        #region Constant, Structure and Delegate Definitions

        /// <summary>
        /// defines the callback type for the hook
        /// </summary>
        public delegate int mouseHookProc(int code, int wParam, ref mouseHookStruct lParam);

        /// <summary>
        /// defines struct about mouse point location
        /// </summary>
        public struct point
        {
            public int x;
            public int y;
        }

        public struct mouseHookStruct
        {
            public point pt;
            public uint mouseData;
            public uint flags;
            public uint time;
            public IntPtr dwExtraInfo;
        }

        const int WM_MOUSE_LL = 14;
        const int WM_LBUTTONDOWN = 0x0201;
        const int WM_LBUTTONUP = 0x0202;
        const int WM_MOUSEMOVE = 0x0200;
        const int WM_MOUSEWHEEL = 0x020A;
        const int WM_RBUTTONDOWN = 0x0204;
        const int WM_RBUTTONUP = 0x0205;

        #endregion

        #region Instance Variables

        /// <summary>
        /// Handle to the hook, need this to unhook and call the next hook
        /// </summary>
        IntPtr hhook = IntPtr.Zero;

        #endregion

        #region Events

        /// <summary>
        /// Occurs when one of the hooked mouse moved
        /// if you need more event about mouse hooking you can add event definitions at this place
        /// </summary>
        public event MouseEventHandler MouseMove;
        public event MouseEventHandler MouseLButtonClick;
        public event MouseEventHandler MouseRButtonClick;

        #endregion

        #region Constructors and Destructors

        private mouseHookProc mhp;

        /// <summary>
        /// Initializes a new instance of the <see cref="globalMouseHook"/> class and installs the mouse hook.
        /// </summary>
        public GlobalMouseHook()
        {
            mhp = new mouseHookProc(hookProc);
            hook();
        }

        ~GlobalMouseHook()
        {
            unhook();
        }

        #endregion

        #region Public Methods

        /// <summary>
        /// Installs the global hook
        /// </summary>
        public void hook()
        {
            IntPtr hInstance = LoadLibrary("User32");
            hhook = SetWindowsHookEx(WM_MOUSE_LL, mhp, hInstance, 0);
        }

        /// <summary>
        /// Uninstalls the global hook
        /// </summary>
        public void unhook()
        {
            UnhookWindowsHookEx(hhook);
        }

        /// <summary>
        /// The callback for the mouse hook
        /// </summary>
        /// <param name="code">The hook code, if it isn't >= 0, the function shouldn't do anyting</param>
        /// <param name="wParam">The event type</param>
        /// <param name="lParam">The mousehook event information</param>
        /// <returns></returns>
        public int hookProc(int code, int wParam, ref mouseHookStruct lParam)
        {
            if (code >= 0)
            {
                if ((wParam == WM_MOUSEMOVE) && (MouseMove != null))
                {
                    MouseEventArgs mea = new MouseEventArgs(MouseButtons.None, 0, lParam.pt.x, lParam.pt.y, 0);
                    MouseMove(this, mea);
                }
                else if ((wParam == WM_LBUTTONUP) && (MouseLButtonClick != null))
                {
                    MouseEventArgs mea = new MouseEventArgs(MouseButtons.Left, 1, lParam.pt.x, lParam.pt.y, 0);
                    MouseLButtonClick(this, mea);
                }
                else if ((wParam == WM_RBUTTONUP) && (MouseLButtonClick != null))
                {
                    MouseEventArgs mea = new MouseEventArgs(MouseButtons.Right, 1, lParam.pt.x, lParam.pt.y, 0);
                    MouseRButtonClick(this, mea);
                }
            }

            return CallNextHookEx(hhook, code, wParam, ref lParam);
        }

        #endregion

        #region DLL imports

        /// <summary>
        /// Sets the windows hook, do the desired event, one of hInstance or threadId must be non-null
        /// </summary>
        /// <param name="idHook">The id of the event you want to hook</param>
        /// <param name="callback">The callback.</param>
        /// <param name="hInstance">The handle you want to attach the event to, can be null</param>
        /// <param name="threadId">The thread you want to attach the event to, can be null</param>
        /// <returns>a handle to the desired hook</returns>
        [DllImport("user32.dll")]
        static extern IntPtr SetWindowsHookEx(int idHook, mouseHookProc callBack, IntPtr hInstance, uint dwThreadId);

        /// <summary>
        /// Unhooks the windows hook.
        /// </summary>
        /// <param name="hInstance">The hook handle that was returned from SetWindowsHookEx</param>
        /// <returns>True if successful, false otherwise</returns>
        [DllImport("user32.dll")]
        static extern bool UnhookWindowsHookEx(IntPtr hInstance);

        /// <summary>
        /// Calls the next hook.
        /// </summary>
        /// <param name="idHook">The hook id</param>
        /// <param name="nCode">The hook code</param>
        /// <param name="wParam">The wparam.</param>
        /// <param name="lParam">The lparam.</param>
        /// <returns></returns>
        [DllImport("user32.dll")]
        static extern int CallNextHookEx(IntPtr idHook, int nCode, int wParam, ref mouseHookStruct lParam);

        /// <summary>
        /// Loads the library.
        /// </summary>
        /// <param name="lpFileName">Name of the library</param>
        /// <returns>A handle to the library</returns>
        [DllImport("kernel32.dll")]
        static extern IntPtr LoadLibrary(string lpFileName);

        #endregion
    }
     */
}
