using System;
using System.Collections.Generic;
using System.Text;

using System.Data;
using System.Data.SqlClient;

using System.IO;

using System.Globalization;
using System.Configuration;
using System.Collections.Specialized;

using ECP.Global;
using ECP.Global.Device;
using ECP.Db;

namespace ECP.Util
{
    public class ObservableConfigGenerator
    {
        private string savedDirName = string.Empty;

        public string SavedDirName
        {
            get { return savedDirName; }
            set { savedDirName = value; }
        }
             
        public ObservableConfigGenerator()
        {
            this.GenerateSubsystemConfigFiles();
        }

        public void GenerateEquipmentConfigFiles()
        {
            
        }

        public void GenerateModuleConfigFiles()
        {

        }

        public void GenerateSubsystemConfigFiles()
        {
//            this.Generate(SubSystemType.STOCKERPORT);
//            this.Generate(SubSystemType.CLEANERPORT);
//            this.Generate(SubSystemType.PROCESSPORT);
//            this.Generate(SubSystemType.RACKMASTER);
//            this.Generate(SubSystemType.FFU);
//            this.Generate(SubSystemType.SHELF);
//            this.Generate(SubSystemType.LIFTERPORT);
            this.Generate(SubSystemType.VEHICLE);
            this.Generate(EquipmentType.STOCKER);

        }

        public void Generate(EquipmentType elementType)
        {
            string nameSapce = "";
            string className = EquipmentType.ValueToString(elementType);
            string fileName = "StockerSEM" + "Config.cs";

            string observableQueryString = "select distinct observableId, accesstype, dataType from observable \n" +
            "where ownerid ='STOCKER'";

//            string observableQueryString = "select distinct observableId, accesstype, dataType, classname from observable a, equipment b\n" +
//            "where a.ownerId = b.subsystemId and subsystemtype ='" + className + "'";

            string nameSpaceQueryString = "select distinct classname from equipment where equipmenttype ='" +
                className + "'";

            FileStream file = null;
            BufferedStream buffer = null;
            StreamWriter writer = null;


            if (this.savedDirName.Equals(string.Empty))
            {
                NameValueCollection appSettings =
                     ConfigurationManager.AppSettings;
                this.savedDirName = appSettings[ECPProperties.ECP_BASE].ToString() + @"\" + "temp";
            }

            //check the directory exist if not create

            if (!Directory.Exists(savedDirName))
            {
                Directory.CreateDirectory(savedDirName);
            }

            try
            {
                file = new FileStream(savedDirName + @"\" + fileName, FileMode.Create, FileAccess.Write);
                buffer = new BufferedStream(file);
                writer = new StreamWriter(buffer);
            }
            catch (IOException ex)
            {
                Console.WriteLine("IOException encounted when open the File name :" + className + "\n" + ex.Message);
            }
            nameSapce = GetNameSpaceString(nameSpaceQueryString, className);
            WriteHeaderCode(writer, nameSapce, "Stocker");
            List<ObservableInfo> observableInfos = this.GetObservableList(observableQueryString);
            WriteObservableCode(writer, className, observableInfos);
            WriteEndingCode(writer, className);

            writer.Close();
            
        }

        public void Generate(ModuleType moduleType)
        {

        }

        public void Generate(SubSystemType subSystemType)
        {
            string nameSapce = "";
            string className = SubSystemType.ValueToString(subSystemType);
            string fileName = className + "Config.cs";

            string observableQueryString ="select distinct observableId, accesstype, dataType, classname from observable a, subsystem b\n"+ 
            "where a.ownerId = b.subsystemId and subsystemtype ='" + className + "'";

            string nameSpaceQueryString ="select distinct classname from subsystem where subsystemtype ='" + 
                className + "'";

            FileStream file = null;
            BufferedStream buffer = null;
            StreamWriter writer = null;


            if(this.savedDirName.Equals(string.Empty))
            {
                NameValueCollection appSettings =
                     ConfigurationManager.AppSettings;
                this.savedDirName = appSettings[ECPProperties.ECP_BASE].ToString() + @"\" + "temp";
            }

            //check the directory exist if not create

            if (!Directory.Exists(savedDirName))
            {
                Directory.CreateDirectory(savedDirName);
            }

            try
            {
                file = new FileStream(savedDirName + @"\" + fileName, FileMode.Create, FileAccess.Write);
                buffer = new BufferedStream(file);
                writer = new StreamWriter(buffer);                         
            }
            catch (IOException ex )
            {
                Console.WriteLine("IOException encounted when open the File name :"+className +"\n"+ ex.Message);
            }
            nameSapce =  GetNameSpaceString(nameSpaceQueryString, className);
            WriteHeaderCode(writer, nameSapce, className);
            List<ObservableInfo> observableInfos = this.GetObservableList(observableQueryString);
            WriteObservableCode(writer, className, observableInfos);     
            WriteEndingCode(writer, className);

            writer.Close();
        
        }

        public void WriteHeaderCode(StreamWriter writer, string nameSapce, string className)
        {
            string writeData =
                "using System;" + "\n" +
                "using System.Collections.Generic;" + "\n" +
                "using System.Text;" + "\n" +
                "using ECP.Device.Observe;" + "\n\n" +
                "namespace " + nameSapce + "\n" +
                "{" + "\n" +
                "\tpublic partial class " + className + "\n" +
                "\t{" + "\n" +
                "\n";

            WriteLine(writer, writeData, className);

            
        }

        public void WriteObservableCode(StreamWriter writer, string fileName, List<ObservableInfo> observableInfos)
        {

            string writeData =
                    "\t\t#region Properties for Observable Access" + "\n";

            foreach (ObservableInfo observableInfo in observableInfos)
            {
                if (observableInfo.AccessType == AccessType.IN)
                {
                    writeData +=
                     "\t\tpublic Observable @__I_" + observableInfo.ObservableId + "\n" +
                     "\t\t{" + "\n"+
                     "\t\t\tget" + "\n" +
                     "\t\t\t{" + "\n" +
                     "\t\t\t\treturn this.Observables[\""+observableInfo.ObservableId+"\"];" + "\n" +
                     "\t\t\t}" + "\n" +
                     "\t\t}" + "\n";
                }
                if (observableInfo.AccessType == AccessType.OUT)
                {
                    writeData +=
                     "\t\tpublic Observable @__O_" + observableInfo.ObservableId + "\n" +
                     "\t\t{" + "\n" +
                     "\t\t\tget" + "\n" +
                     "\t\t\t{" + "\n" +
                     "\t\t\t\treturn this.Observables[\"" + observableInfo.ObservableId + "\"];" + "\n" +
                     "\t\t\t}" + "\n" +
                     "\t\t}" + "\n";
                }
                if (observableInfo.AccessType == AccessType.IN_OUT)
                {
                    writeData +=
                     "\t\tpublic Observable @__IO_" + observableInfo.ObservableId + "\n" +
                     "\t\t{" + "\n" +
                     "\t\t\tget" + "\n" +
                     "\t\t\t{" + "\n" +
                     "\t\t\t\treturn this.Observables[\"" + observableInfo.ObservableId + "\"];" + "\n" +
                     "\t\t\t}" + "\n" +
                     "\t\t}" + "\n";
                }
            }

            writeData +=
                "\t\t#endregion" + "\n\n" +
                "\t\t#region Properties for Observable Access By Value" + "\n\n";
            
            foreach (ObservableInfo observableInfo in observableInfos)
            {
                if (observableInfo.AccessType == AccessType.IN)
                {
                    writeData +=
                        "\t\tpublic " + GetPrimitiveTypeString(observableInfo.DataType) + " I_" + observableInfo.ObservableId + "\n";
                }
                if (observableInfo.AccessType == AccessType.OUT)
                {
                    writeData +=
                        "\t\tpublic " + GetPrimitiveTypeString(observableInfo.DataType) + " O_" + observableInfo.ObservableId + "\n";
                }
                if (observableInfo.AccessType == AccessType.IN_OUT)
                {
                    writeData +=
                        "\t\tpublic " + GetPrimitiveTypeString(observableInfo.DataType) + " IO_" + observableInfo.ObservableId + "\n";
                }
                writeData += "\t\t{" + "\n";
                //getter
                if(observableInfo.AccessType == AccessType.IN ||
                    observableInfo.AccessType == AccessType.IN_OUT)
                {
                    writeData +=
                     "\t\t\tget" + "\n" +
                     "\t\t\t{" + "\n" +
                     "\t\t\t\treturn this.Observables[\"" + observableInfo.ObservableId + "\"]" + GetCustomTypeString(observableInfo.DataType) + ";\n" +
                     "\t\t\t}" + "\n";
                }

                if (observableInfo.AccessType == AccessType.OUT ||
                    observableInfo.AccessType == AccessType.IN_OUT)
                {
                    writeData +=
                     "\t\t\tset" + "\n" +
                     "\t\t\t{" + "\n" +
                     "\t\t\t\tthis.Observables[\"" + observableInfo.ObservableId + "\"]" + GetCustomTypeString(observableInfo.DataType) + " = value;\n" +
                     "\t\t\t}" + "\n";
                }
                writeData += "\t\t}" + "\n";

               
            }

            writeData +=
                "\t\t#endregion" + "\n";

            WriteLine(writer, writeData, fileName);            
        }

        public void WriteEndingCode(StreamWriter writer, string fileName)
        {
            string writeData = "";

            writeData +=
                "\t}" + "\n" +
                "}\n";

            WriteLine(writer, writeData, fileName); 
        }

        public  List<ObservableInfo> GetObservableList(string sqlstring)
        {
            List<ObservableInfo> observables = new List<ObservableInfo>();

            DBConnection conn = DBControl.GetConnection();

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {
                observables.Add(
                    new ObservableInfo((string)reader["OBSERVABLEID"], 
                    AccessType.StringToValue((string) reader["ACCESSTYPE"]),
                    DataType.StringToValue( (string)reader["DATATYPE"])));
            }

            if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);

            return observables;
        }

        public string GetNameSpaceString(string sqlstring, string classname)
        {
            string nameSapceString = null;

            DBConnection conn = DBControl.GetConnection();

            SqlDataReader reader = conn.ExecuteQuery(sqlstring);

            while (reader.Read())
            {
                nameSapceString = (string)reader["CLASSNAME"];
                break;
            }

            if(nameSapceString != null)
                nameSapceString = nameSapceString.Substring(0, nameSapceString.IndexOf("."+classname));

             if (reader != null) reader.Close();

            DBControl.RemoveDBConection(conn);

            return nameSapceString;
        }


        private  void WriteLine(StreamWriter writer, string theData, string theFileName)
        {
            try
            {
                writer.Write(theData);
                Console.Write(theData);
            }
            catch (IOException ex)
            {
                Console.WriteLine("We have IO Exception while writing to the File "+theFileName+
                    "\n"+ex.Message);
            }
        }

        public string GetPrimitiveTypeString(DataType dataType)
        {
            string strDataType = "";

            switch (dataType.Value())
            {
                case DataType._BOOLEAN:
                    strDataType = "bool";
                    break;
                case DataType._BYTE:
                    strDataType = "byte";
                    break;
                case DataType._SHORT:
                    strDataType = "short";
                    break;
                case DataType._INTEGER:
                    strDataType = "int";
                    break;
                case DataType._LONG:
                    strDataType = "long";
                    break;
                case DataType._DOUBLE:
                    strDataType = "double";
                    break;
                case DataType._STRING:
                    strDataType = "string";
                    break;
                case DataType._SHORT_ARRAY:
                    strDataType = "List<short>";
                    break;
                case DataType._BOOL_ARRAY:
                    strDataType = "List<bool>";
                    break;
            }
            return strDataType;
        }


        public string GetCustomTypeString(DataType dataType)
        {
            string strProperty = "";

            switch (dataType.Value())
            {
                case DataType._BOOLEAN:
                    strProperty = ".AsBoolean";
                    break;
                case DataType._BYTE:
                    strProperty = ".AsByte";
                    break;
                case DataType._SHORT:
                    strProperty = ".AsShort";
                    break;
                case DataType._INTEGER:
                    strProperty = ".AsInteger";
                    break;
                case DataType._LONG:
                    strProperty = ".AsLong";
                    break;
                case DataType._DOUBLE:
                    strProperty = ".AsDouble";
                    break;
                case DataType._STRING:
                    strProperty = ".AsString";
                    break;
                case DataType._SHORT_ARRAY:
                    strProperty = ".AsShortArray";
                    break;
                case DataType._BOOL_ARRAY:
                    strProperty = ".AsBoolArray";
                    break;
            }
            return strProperty;
        }

        public class ObservableInfo
        {
            string observableId;

            public string ObservableId
            {
                get { return observableId; }
                set { observableId = value; }
            }

            AccessType accessType;

            public AccessType AccessType
            {
                get { return accessType; }
                set { accessType = value; }
            }

            DataType dataType;

            public DataType DataType
            {
                get { return dataType; }
                set { dataType = value; }
            }


            public ObservableInfo(string observableId, AccessType accessType, DataType dataType)
            {
                this.observableId = observableId;
                this.accessType = accessType;
                this.dataType = dataType;
            }
        }
    }
}
