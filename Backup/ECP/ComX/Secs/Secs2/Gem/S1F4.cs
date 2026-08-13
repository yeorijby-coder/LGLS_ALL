using System;
using System.Collections.Generic;
using System.Text;

namespace ECP.ComX.Secs.Secs2.Gem
{
    public class S1F4  : Secs2Message
    {
        public const String STATUS_VARIABLE_VALUE_LIST = "0";

        
        public S1F4( SecsHeader secsHeader )
            :base( secsHeader, new ItemL() )
        {
      
        }

       
        public S1F4( SecsMessage secsMessage )
            :base( secsMessage )
        {
          
        }

       
        public void AddSV(Item variableValue)
        {
            Item variableItemList = Find(STATUS_VARIABLE_VALUE_LIST);
            variableItemList.Add(variableValue);
        }

        
        public int GetSVCount()
        {
            return Find(STATUS_VARIABLE_VALUE_LIST).Count();
        }

        
        public String GetSV(int index)
        {
            return Find(STATUS_VARIABLE_VALUE_LIST + "." + index).GetString();
        }

       
        public Item GetSVItem(int index)
        {
            return Find(STATUS_VARIABLE_VALUE_LIST + "." + index);
        }
    }
}
