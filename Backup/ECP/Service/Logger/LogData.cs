using System;
using System.Collections.Generic;
using System.Text;
using ECP.Service.Logger;

namespace ECP.Service.Logger
{
    public class LogData
    {
        public Category category = null;
        public string method = null;
        public string device = null;
        public string sourceId = null;
        public string trackingId = null;
        public string messageName = null;
        public string data = null;
        public long time = 0;
        public string binaryLink = null;
        public string level = null;
        public string assemblyName = null;
        public string className = null;
        public byte[] binaryData = null;

        public LogData ()
        {
        }

        public LogData(Category _category, string _method,
          string _device, string _sourceId,
          string _trackingId, string _messageName, 
          string _data, long _time, string _binaryLink, 
          string _level, string _assemblyName, string _className, byte[] _binaryData)
        {
        category = _category;
        method = _method;
        device = _device;
        sourceId = _sourceId;
        trackingId = _trackingId;
        messageName = _messageName;
        data = _data;
        time = _time;
        binaryLink = _binaryLink;
        level = _level;
        assemblyName = _assemblyName;
        className = _className;
        binaryData = _binaryData;
        } 
    }
}
