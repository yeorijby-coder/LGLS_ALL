using System;
using System.Collections.Generic;
using System.Text;

namespace TSK_COMM_IOSCH
{
    public class AlarmInfo
    {
	    public int      _nALCD;
	    public int      _nALID;
	    public string   _strALTX;		// Max 40자리
	    public string   _strUnitID;
	    public string   _strCarrierLoc;
	    public string   _strHelp;
	    public string   _strALTXEng;
	    public string   _strALTXKor;
	    public string   _strErrorID;
        public int      _nStkUnitState;
        public string   _strRecoveryOPT;
    }
}
