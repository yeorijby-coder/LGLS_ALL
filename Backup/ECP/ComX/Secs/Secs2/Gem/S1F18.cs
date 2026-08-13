using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S1F18 : Secs2Message
    {
       
        public S1F18(SecsHeader secsHeader) 
            : this(secsHeader, (byte)0)
        {
            
        }

       
        public S1F18(SecsHeader secsHeader, byte onlack)
            : base(secsHeader, new ItemB(onlack))
        {
            
        }

       
        public void SetONLACK(byte onlack)
        {
            ItemB onlackItem = (ItemB)Find("0");
            onlackItem.SetValue(onlack);
        }

        
        public byte GetONLACK()
        {
            ItemB onlack = (ItemB)Find("0");
            return (byte)((int)onlack.GetValue());
        }

        
        public S1F18(SecsMessage secsMessage)
            : base (secsMessage)
        {
           
        }
    }
}
