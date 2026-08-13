using System;
using System.Collections.Generic;
using System.Text;
using ECP.Global;
using System.Data.SqlClient;
using ECP.Db.Entity;
using ECP.Db;
using ECP.Util;

namespace ECP.BizRule
{
    [Serializable]
    public class TransferSummary : ECPObject 
    {
        private string commandtype = "";

        public string Commandtype
        {
            get { return commandtype; }
            set { commandtype = value; }
        }
        private string fromposition = "";

        public string Fromposition
        {
            get { return fromposition; }
            set { fromposition = value; }
        }
        private string frompositionndesc = "";

        public string Frompositionndesc
        {
            get { return frompositionndesc; }
            set { frompositionndesc = value; }
        }
        private string toposition = "";

        public string Toposition
        {
            get { return toposition; }
            set { toposition = value; }
        }
        private string topositiondesc = "";

        public string Topositiondesc
        {
            get { return topositiondesc; }
            set { topositiondesc = value; }
        }
        private string request = "";

        public string Request
        {
            get { return request; }
            set { request = value; }
        }
        private string complete = "";

        public string Complete
        {
            get { return complete; }
            set { complete = value; }
        }
        private string transferring = "";

        public string Transferring
        {
            get { return transferring; }
            set { transferring = value; }
        }
        private string wait = "";

        public string Wait
        {
            get { return wait; }
            set { wait = value; }
        }

        private string dbConnectionString = "";
        public TransferSummary()
        {
            DBConnectionValue dbValue = new DBConnectionValue();
            dbConnectionString = dbValue.getGlobalDBConnectionString();
        }

        //반송 이력 조회
        public List<TransferSummary> getTransferSummaryData(string dateString)
        {
            List<TransferSummary> list = new List<TransferSummary>();
            DBConnection theDB = null;
            SqlDataReader reader = null;

            string strDate = "%" + dateString.Substring(1, 7) + "%";
            string strSql = getTransferSummarySQL(strDate);

            try
            {
                theDB = DBControl.GetConnection(dbConnectionString);
                reader = theDB.ExecuteQuery(strSql);

                while (reader.Read())
                {
                    TransferSummary trasnferSummaryData = new TransferSummary();
                    trasnferSummaryData.setTransferSummaryData(reader);

                    list.Add(trasnferSummaryData);
                }
            }
            catch (SqlException se)
            {
                throw se;
            }
            finally
            {
                if (reader != null) reader.Close();
                if (theDB != null)
                {
                    DBControl.RemoveDBConection(theDB);
                }
            }

            return list;
        }

        public void setTransferSummaryData(SqlDataReader reader)
        {
            this.Commandtype = Helper.PreventNull((string)reader["COMMANDTYPE"]);
            this.Fromposition = Helper.PreventNull((string)reader["FROMPOSITION"]);
            this.Frompositionndesc = Helper.PreventNull((string)reader["FROMPOSITIONNDESC"]);
            this.Toposition = Helper.PreventNull((string)reader["TOPOSITION"]);
            this.Topositiondesc = Helper.PreventNull((string)reader["TOPOSITIONDESC"]);
            this.Request = ((int)reader["REQUEST"]).ToString();
            this.Complete = ((int)reader["COMPLETE"]).ToString();
            this.Transferring = ((int)reader["TRANSFERRING"]).ToString();
            this.Wait = ((int)reader["WAIT"]).ToString();
        }

        public string getTransferSummarySQL(string strDate)
        {
            string strSql = "";

            strSql += "" +
"  SELECT  commandtype                                                                                         " +
"         ,fromposition                                                                                        " +
"         ,frompositionndesc                                                                                   " +
"         ,toposition                                                                                          " +
"         ,topositiondesc                                                                                      " +
"         ,ISNULL(request     , 0)    request                                                                  " +
"         ,ISNULL(complete    , 0)    complete                                                                 " +
"         ,ISNULL(transferring, 0)    transferring                                                             " +
"         ,ISNULL(wait        , 0)    wait                                                                     " +
"    FROM (                                                                                                    " +
"  				SELECT  commandtype                                                                                  " +
"  							 ,fromposition                                                                                 " +
"  				       ,(SELECT value                                                                                " +
"  				           FROM TB_CODEMASTER                                                                        " +
"  				          WHERE codeclass = '001'                                                                    " +
"  				            AND code = fromposition                                                                  " +
"  				        ) frompositionndesc                                                                          " +
"  							 ,toposition                                                                                   " +
"  				       ,(SELECT value                                                                                " +
"  				           FROM TB_CODEMASTER                                                                        " +
"  				          WHERE codeclass = '001'                                                                    " +
"  				            AND code = toposition                                                                    " +
"  				        ) topositiondesc                                                                             " +
"  				       ,SUM(CASE transferstatus WHEN 'Complete' THEN count ELSE 0 END)                               " +
"  				        + SUM(CASE transferstatus WHEN 'Transferring' THEN count ELSE 0 END)                         " +
"  				        + SUM(CASE transferstatus WHEN 'Wait' THEN count ELSE 0 END) AS request                      " +
"  				       ,SUM(CASE transferstatus WHEN 'Complete' THEN count ELSE 0 END) AS complete                   " +
"  				       ,SUM(CASE transferstatus WHEN 'Transferring' THEN count ELSE 0 END) AS transferring           " +
"  				       ,SUM(CASE transferstatus WHEN 'Wait' THEN count ELSE 0 END) AS wait                           " +
"  				  FROM (                                                                                             " +
"  								SELECT  fromposition                                                                         " +
"  								       ,toposition                                                                           " +
"  								       ,transferstatus                                                                       " +
"  				               ,materialno                                                                           " +
"  											 ,commandtype                                                                          " +
"  				               ,batchno                                                                              " +
"  								       ,count(*) as count                                                                    " +
"  								  FROM (                                                                                     " +
"  												SELECT  ISNULL(B.sapcommandid, A.commandid) AS reqno                                 " +
"  												       ,ISNULL(B.ordertype,'') AS ordertype                                          " +
"  												       ,A.commandid                                                                  " +
"  												       ,A.commandtype                                                                " +
"  												       ,A.palletid                                                                   " +
"  												       ,ISNULL(A.materialno,'') AS materialno                                        " +
"  												       ,ISNULL(A.batchno,'') AS batchno                                              " +
"  												       ,ISNULL(B.sapqty,ISNULL(A.qty,'0')) AS sapqty                                 " +
"  												       ,ISNULL(B.qty,ISNULL(A.qty,'0')) AS qty                                       " +
"  												       ,(SELECT value                                                                " +
"  												           FROM TB_UDPROPERTY                                                        " +
"  												          WHERE entityid = A.fromdeviceid                                            " +
"  												            AND udpropertycd = 'START_DEVICE_01'                                     " +
"  												         ) fromposition                                                              " +
"  												       ,A.fromdeviceid                                                               " +
"  												       ,(SELECT value                                                                " +
"  												           FROM TB_UDPROPERTY                                                        " +
"  												          WHERE entityid = A.todeviceid                                              " +
"  												            AND udpropertycd = 'END_DEVICE_01'                                       " +
"  												         ) toposition                                                                " +
"  												       ,A.todeviceid                                                                 " +
"  												       ,A.transferstatus AS transferstatus                                           " +
"  												FROM   TB_TRANSFER AS A                                                              " +
"  												       LEFT OUTER JOIN                                                               " +
"  												       TB_TRANSFERDIVISION AS B                                                      " +
"  												       ON A.commandid = B.ecscommandid                                               " +
"  											   WHERE   A.commandid LIKE '" + strDate + "'                                            " +
"  												UNION ALL                                                                            " +
"  												SELECT DISTINCT ISNULL(B.sapcommandid, A.commandid) AS reqno                         " +
"  												       ,ISNULL(B.ordertype,'') AS ordertype                                          " +
"  												       ,A.commandid                                                                  " +
"  												       ,A.commandtype                                                                " +
"  												       ,A.palletid                                                                   " +
"  												       ,ISNULL(A.materialno,'') AS materialno                                        " +
"  												       ,ISNULL(A.batchno,'') AS batchno                                              " +
"  												       ,ISNULL(B.sapqty,ISNULL(A.qty,'0')) AS sapqty                                 " +
"  												       ,ISNULL(B.qty,ISNULL(A.qty,'0')) AS qty                                       " +
"  												       ,(SELECT value                                                                " +
"  												           FROM TB_UDPROPERTY                                                        " +
"  												          WHERE entityid = A.fromdeviceid                                            " +
"  												            AND udpropertycd = 'START_DEVICE_01'                                     " +
"  												         ) fromposition                                                              " +
"  												       ,A.fromdeviceid                                                               " +
"  												       ,(SELECT value                                                                " +
"  												           FROM TB_UDPROPERTY                                                        " +
"  												          WHERE entityid = A.todeviceid                                              " +
"  												            AND udpropertycd = 'END_DEVICE_01'                                       " +
"  												         ) toposition                                                                " +
"  												       ,A.todeviceid                                                                 " +
"  												       ,'Complete' AS transferstatus                                                 " +
"  												FROM   TB_TRANSFERHIST AS A                                                          " +
"  												       LEFT OUTER JOIN                                                               " +
"  												       TB_TRANSFERDIVISIONHIST AS B                                                  " +
"  												       ON A.commandid = B.ecscommandid                                               " +
"  												WHERE A.executetime <> ''                                                            " +
"  												  AND A.executetime IS NOT NULL                                                      " +
"  												  AND A.completetime <> ''                                                           " +
"  												  AND A.completetime IS NOT NULL                                                     " +
"  												  AND A.commandid LIKE '" + strDate + "'                                             " +
"  											 ) as inner_1                                                                          " +
"  								GROUP BY fromposition, toposition, commandtype, materialno, batchno, transferstatus          " +
"  								) as inner_2                                                                                 " +
"  				GROUP BY commandtype, fromposition, toposition                                                       " +
"  				UNION ALL                                                                                            " +
"  				SELECT  commandtype                                                                                  " +
"  				       ,'' as fromposition                                                                           " +
"  				       ,'' as frompositionndesc                                                                      " +
"  				       ,'' as toposition                                                                             " +
"  				       ,'' as topositiondesc                                                                         " +
"  				       ,SUM(request) as request                                                                      " +
"  				       ,SUM(complete) as complete                                                                    " +
"  				       ,SUM(transferring) as transferring                                                            " +
"  				       ,SUM(wait) as wait                                                                            " +
"  				  FROM (                                                                                             " +
"  								SELECT  commandtype                                                                          " +
"  											 ,fromposition                                                                         " +
"  								       ,(SELECT value                                                                        " +
"  								           FROM TB_CODEMASTER                                                                " +
"  								          WHERE codeclass = '001'                                                            " +
"  								            AND code = fromposition                                                          " +
"  								        ) frompositionndesc                                                                  " +
"  											 ,toposition                                                                           " +
"  								       ,(SELECT value                                                                        " +
"  								           FROM TB_CODEMASTER                                                                " +
"  								          WHERE codeclass = '001'                                                            " +
"  								            AND code = toposition                                                            " +
"  								        ) topositiondesc                                                                     " +
"  								       ,SUM(CASE transferstatus WHEN 'Complete' THEN count ELSE 0 END)                       " +
"  								        + SUM(CASE transferstatus WHEN 'Transferring' THEN count ELSE 0 END)                 " +
"  								        + SUM(CASE transferstatus WHEN 'Wait' THEN count ELSE 0 END) AS request              " +
"  								       ,SUM(CASE transferstatus WHEN 'Complete' THEN count ELSE 0 END) AS complete           " +
"  								       ,SUM(CASE transferstatus WHEN 'Transferring' THEN count ELSE 0 END) AS transferring   " +
"  								       ,SUM(CASE transferstatus WHEN 'Wait' THEN count ELSE 0 END) AS wait                   " +
"  								  FROM (                                                                                     " +
"  												SELECT  fromposition                                                                 " +
"  												       ,toposition                                                                   " +
"  												       ,transferstatus                                                               " +
"  								               ,materialno                                                                   " +
"  															 ,commandtype                                                                  " +
"  								               ,batchno                                                                      " +
"  												       ,count(*) as count                                                            " +
"  												  FROM (                                                                             " +
"  																SELECT  ISNULL(B.sapcommandid, A.commandid) AS reqno                         " +
"  																       ,ISNULL(B.ordertype,'') AS ordertype                                  " +
"  																       ,A.commandid                                                          " +
"  																       ,A.commandtype                                                        " +
"  																       ,A.palletid                                                           " +
"  																       ,ISNULL(A.materialno,'') AS materialno                                " +
"  																       ,ISNULL(A.batchno,'') AS batchno                                      " +
"  																       ,ISNULL(B.sapqty,ISNULL(A.qty,'0')) AS sapqty                         " +
"  																       ,ISNULL(B.qty,ISNULL(A.qty,'0')) AS qty                               " +
"  																       ,(SELECT value                                                        " +
"  																           FROM TB_UDPROPERTY                                                " +
"  																          WHERE entityid = A.fromdeviceid                                    " +
"  																            AND udpropertycd = 'START_DEVICE_01'                             " +
"  																         ) fromposition                                                      " +
"  																       ,A.fromdeviceid                                                       " +
"  																       ,(SELECT value                                                        " +
"  																           FROM TB_UDPROPERTY                                                " +
"  																          WHERE entityid = A.todeviceid                                      " +
"  																            AND udpropertycd = 'END_DEVICE_01'                               " +
"  																         ) toposition                                                        " +
"  																       ,A.todeviceid                                                         " +
"  																       ,A.transferstatus AS transferstatus                                   " +
"  																FROM   TB_TRANSFER AS A                                                      " +
"  																       LEFT OUTER JOIN                                                       " +
"  																       TB_TRANSFERDIVISION AS B                                              " +
"  																       ON A.commandid = B.ecscommandid                                       " +
"  											                   WHERE   A.commandid LIKE '" + strDate + "'                                    " +
"  																UNION ALL                                                                    " +
"  																SELECT DISTINCT ISNULL(B.sapcommandid, A.commandid) AS reqno                 " +
"  																       ,ISNULL(B.ordertype,'') AS ordertype                                  " +
"  																       ,A.commandid                                                          " +
"  																       ,A.commandtype                                                        " +
"  																       ,A.palletid                                                           " +
"  																       ,ISNULL(A.materialno,'') AS materialno                                " +
"  																       ,ISNULL(A.batchno,'') AS batchno                                      " +
"  																       ,ISNULL(B.sapqty,ISNULL(A.qty,'0')) AS sapqty                         " +
"  																       ,ISNULL(B.qty,ISNULL(A.qty,'0')) AS qty                               " +
"  																       ,(SELECT value                                                        " +
"  																           FROM TB_UDPROPERTY                                                " +
"  																          WHERE entityid = A.fromdeviceid                                    " +
"  																            AND udpropertycd = 'START_DEVICE_01'                             " +
"  																         ) fromposition                                                      " +
"  																       ,A.fromdeviceid                                                       " +
"  																       ,(SELECT value                                                        " +
"  																           FROM TB_UDPROPERTY                                                " +
"  																          WHERE entityid = A.todeviceid                                      " +
"  																            AND udpropertycd = 'END_DEVICE_01'                               " +
"  																         ) toposition                                                        " +
"  																       ,A.todeviceid                                                         " +
"  																       ,'Complete' AS transferstatus                                         " +
"  																FROM   TB_TRANSFERHIST AS A                                                  " +
"  																       LEFT OUTER JOIN                                                       " +
"  																       TB_TRANSFERDIVISIONHIST AS B                                          " +
"  																       ON A.commandid = B.ecscommandid                                       " +
"  																WHERE A.executetime <> ''                                                    " +
"  																  AND A.executetime IS NOT NULL                                              " +
"  																  AND A.completetime <> ''                                                   " +
"  																  AND A.completetime IS NOT NULL                                             " +
"  																  AND A.commandid LIKE '" + strDate + "'                                     " +
"  															 ) as inner_1                                                                  " +
"  												GROUP BY fromposition, toposition, commandtype, materialno, batchno, transferstatus  " +
"  												) as inner_2                                                                         " +
"  								GROUP BY commandtype, fromposition, toposition                                               " +
"  				       ) as inner_3                                                                                  " +
"  				GROUP BY commandtype                                                                                 " +
"  				UNION ALL                                                                                            " +
"  				SELECT  'Total' as commandtype                                                                       " +
"  				       ,'' as fromposition                                                                           " +
"  				       ,'' as frompositionndesc                                                                      " +
"  				       ,'' as toposition                                                                             " +
"  				       ,'' as topositiondesc                                                                         " +
"  				       ,SUM(request) as request                                                                      " +
"  				       ,SUM(complete) as complete                                                                    " +
"  				       ,SUM(transferring) as transferring                                                            " +
"  				       ,SUM(wait) as wait                                                                            " +
"  				  FROM (                                                                                             " +
"  								SELECT  commandtype                                                                          " +
"  											 ,fromposition                                                                         " +
"  								       ,(SELECT value                                                                        " +
"  								           FROM TB_CODEMASTER                                                                " +
"  								          WHERE codeclass = '001'                                                            " +
"  								            AND code = fromposition                                                          " +
"  								        ) frompositionndesc                                                                  " +
"  											 ,toposition                                                                           " +
"  								       ,(SELECT value                                                                        " +
"  								           FROM TB_CODEMASTER                                                                " +
"  								          WHERE codeclass = '001'                                                            " +
"  								            AND code = toposition                                                            " +
"  								        ) topositiondesc                                                                     " +
"  								       ,SUM(CASE transferstatus WHEN 'Complete' THEN count ELSE 0 END)                       " +
"  								        + SUM(CASE transferstatus WHEN 'Transferring' THEN count ELSE 0 END)                 " +
"  								        + SUM(CASE transferstatus WHEN 'Wait' THEN count ELSE 0 END) AS request              " +
"  								       ,SUM(CASE transferstatus WHEN 'Complete' THEN count ELSE 0 END) AS complete           " +
"  								       ,SUM(CASE transferstatus WHEN 'Transferring' THEN count ELSE 0 END) AS transferring   " +
"  								       ,SUM(CASE transferstatus WHEN 'Wait' THEN count ELSE 0 END) AS wait                   " +
"  								  FROM (                                                                                     " +
"  												SELECT  fromposition                                                                 " +
"  												       ,toposition                                                                   " +
"  												       ,transferstatus                                                               " +
"  								               ,materialno                                                                   " +
"  															 ,commandtype                                                                  " +
"  								               ,batchno                                                                      " +
"  												       ,count(*) as count                                                            " +
"  												  FROM (                                                                             " +
"  																SELECT  ISNULL(B.sapcommandid, A.commandid) AS reqno                         " +
"  																       ,ISNULL(B.ordertype,'') AS ordertype                                  " +
"  																       ,A.commandid                                                          " +
"  																       ,A.commandtype                                                        " +
"  																       ,A.palletid                                                           " +
"  																       ,ISNULL(A.materialno,'') AS materialno                                " +
"  																       ,ISNULL(A.batchno,'') AS batchno                                      " +
"  																       ,ISNULL(B.sapqty,ISNULL(A.qty,'0')) AS sapqty                         " +
"  																       ,ISNULL(B.qty,ISNULL(A.qty,'0')) AS qty                               " +
"  																       ,(SELECT value                                                        " +
"  																           FROM TB_UDPROPERTY                                                " +
"  																          WHERE entityid = A.fromdeviceid                                    " +
"  																            AND udpropertycd = 'START_DEVICE_01'                             " +
"  																         ) fromposition                                                      " +
"  																       ,A.fromdeviceid                                                       " +
"  																       ,(SELECT value                                                        " +
"  																           FROM TB_UDPROPERTY                                                " +
"  																          WHERE entityid = A.todeviceid                                      " +
"  																            AND udpropertycd = 'END_DEVICE_01'                               " +
"  																         ) toposition                                                        " +
"  																       ,A.todeviceid                                                         " +
"  																       ,A.transferstatus AS transferstatus                                   " +
"  																FROM   TB_TRANSFER AS A                                                      " +
"  																       LEFT OUTER JOIN                                                       " +
"  																       TB_TRANSFERDIVISION AS B                                              " +
"  																       ON A.commandid = B.ecscommandid                                       " +
"  											                   WHERE   A.commandid LIKE '" + strDate + "'                                    " +
"  																UNION ALL                                                                    " +
"  																SELECT DISTINCT ISNULL(B.sapcommandid, A.commandid) AS reqno                 " +
"  																       ,ISNULL(B.ordertype,'') AS ordertype                                  " +
"  																       ,A.commandid                                                          " +
"  																       ,A.commandtype                                                        " +
"  																       ,A.palletid                                                           " +
"  																       ,ISNULL(A.materialno,'') AS materialno                                " +
"  																       ,ISNULL(A.batchno,'') AS batchno                                      " +
"  																       ,ISNULL(B.sapqty,ISNULL(A.qty,'0')) AS sapqty                         " +
"  																       ,ISNULL(B.qty,ISNULL(A.qty,'0')) AS qty                               " +
"  																       ,(SELECT value                                                        " +
"  																           FROM TB_UDPROPERTY                                                " +
"  																          WHERE entityid = A.fromdeviceid                                    " +
"  																            AND udpropertycd = 'START_DEVICE_01'                             " +
"  																         ) fromposition                                                      " +
"  																       ,A.fromdeviceid                                                       " +
"  																       ,(SELECT value                                                        " +
"  																           FROM TB_UDPROPERTY                                                " +
"  																          WHERE entityid = A.todeviceid                                      " +
"  																            AND udpropertycd = 'END_DEVICE_01'                               " +
"  																         ) toposition                                                        " +
"  																       ,A.todeviceid                                                         " +
"  																       ,'Complete' AS transferstatus                                         " +
"  																FROM   TB_TRANSFERHIST AS A                                                  " +
"  																       LEFT OUTER JOIN                                                       " +
"  																       TB_TRANSFERDIVISIONHIST AS B                                          " +
"  																       ON A.commandid = B.ecscommandid                                       " +
"  																WHERE A.executetime <> ''                                                    " +
"  																  AND A.executetime IS NOT NULL                                              " +
"  																  AND A.completetime <> ''                                                   " +
"  																  AND A.completetime IS NOT NULL                                             " +
"  																  AND A.commandid LIKE '" + strDate + "'                                     " +
"  															 ) as inner_1                                                                  " +
"  												GROUP BY fromposition, toposition, commandtype, materialno, batchno, transferstatus  " +
"  												) as inner_2                                                                         " +
"  								GROUP BY commandtype, fromposition, toposition                                               " +
"  				       ) as inner_3                                                                                  " +
"          ) as inner_4                                                                                        " +
"  ORDER BY commandtype, fromposition                                                                          ";
            return strSql;
        }
    }
}
