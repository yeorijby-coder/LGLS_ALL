using System;
using System.Collections.Generic;
using System.Text;

namespace HECS.Gui.Monitor.Controls
{

    public delegate void ControlClick(object sender, ControlEvent e);

    public class ControlEvent :EventArgs
    {
        private string _objectID = "";
        //private string[] _keyValue = "";    // 이 부분은 나중에 메인쪽으로 빼내야 함.
        public ControlEvent(string _objectID)
        {
            this._objectID = _objectID;
        }
        public string ObjectID
        {
            get
            {
                return _objectID;
            }
            set
            {
                _objectID = value;
            }
        }
    }
}
