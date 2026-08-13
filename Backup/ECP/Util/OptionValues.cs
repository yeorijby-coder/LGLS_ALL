using System;
using System.Collections.Generic;
using System.Text;
using ECP.Db;


namespace ECP.Util
{
    /// <summary>
    /// 각 장비별 필요한 옵션사항들을 쉽게 추가하고, 삭제할 수 있다. 
    /// Add, Get, Delete 등의 세개의 메서드만 제공한다. 
    /// Option Value 이므로, 키값의 중복이 발생하는 경우에는 Update가 발생한다.
    /// DeadZone의 설정은 이 테이블을 사용하게 될 텐데, 데이터베이스에 Zone정보를 입력하게 되면
    /// RunTime 중에 바로 적용할 수도 있다. 
    /// </summary>
    public static class OptionValues 
    {

        //private static bool  isOptionTableExist = false; 
        //public static bool AddValue(string elementId, string keyField, string value)
        //{
        //    if (isOptionTableExist == false)
        //    {
        //        CheckOptionTable(); 
        //    }
        //    DBConnection connection = DBControl.GetConnection();
           

        //    return true; 
            
        //}

        
        
        //public static string GetValue(string elementId, string keyField)
        //{
        //    if (isOptionTableExist == false)
        //    {
        //        CheckOptionTable();
        //    }

        //    return null; 

        //}

        //private static void CheckOptionTable()
        //{
        //    if (isOptionTableExist == false)
        //    {
        //        CheckOptionTable();
        //    }

        //    return false;

        //}
        //private static void CreateOptionTable()
        //{

        //    string sqlString = " CREATE TABLE OPTIONTABLE " + 
        //                       " ( " + 
        //                      " ELEMENTID VARCHAR(255) NOT NULL," + 
        //                      " KEYFIELD VARCHAR(255) ," + 
        //                      " VALUE VARCHAR(255) , " + 
        //                      " VALUE2 VARCHAR(255) , " + 
        //                      " VALUE3 VARCHAR(255) , " + 
        //                      " VALUE4 VARCHAR(255) , " + 
        //                      " DESCRIPTION VARCHAR(255)," + 
        //                      " PRIMARY KEY (ELEMENTID, KEYFIELD) " + 
        //                      " ) " ; 
        //    isOptionTableExist = true;
        //}
    }
}
