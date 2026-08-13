using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;
using ECP.Db.Entity;
using ECP.Global;
using System.Windows.Forms;
using System.IO;
using System.Reflection;

namespace ECP.Util
{
    [Serializable]
    public class ExcelExport : ECPObject
    {
        public ExcelExport()
        {
        }

        public Boolean saveCSVFile(DataGridView dataGridView, string className)
        {
//            string strFilePath = "";
//            FileStream fileStream = null;
//            StreamWriter streanWriter = null;

//            try
//            {
////                SaveFileDialog saveFileDialog = new SaveFileDialog();
////                saveFileDialog.Filter = "CSV ÆÄÀÏ(*.csv)|*.csv";

////                if (saveFileDialog.ShowDialog() == DialogResult.OK)
////               {
////                    strFilePath = saveFileDialog.FileName;
////                }

//                strFilePath = @"C:\Temp\" + className + "_" + DateTime.Now.ToString("yyyyMMddHHmmssfff") + ".csv";
//                fileStream = new FileStream(strFilePath, FileMode.OpenOrCreate, FileAccess.Write);
//                streanWriter = new StreamWriter(fileStream, System.Text.Encoding.Default);

//                int iCnt = dataGridView.RowCount;

//                for (int inx = 0; inx < iCnt; inx++)
//                {
//                    DataGridViewRow row = dataGridView.Rows[inx];

//                    int iRow = row.Cells.Count;
//                    string writeStringLine = "";
//                    string writeHeadLine = "";

//                    for (int jnx = 0; jnx < iRow; jnx++)
//                    {
//                        if (inx == 0)
//                        {
//                            writeHeadLine += dataGridView.Columns[jnx].HeaderText;
//                            if (jnx != (iRow - 1))
//                            {
//                                writeHeadLine += ",";
//                            }
//                        }

//                        writeStringLine += (string)row.Cells[jnx].Value;

//                        if (jnx != (iRow - 1))
//                        {
//                            writeStringLine += ",";
//                        }

//                    }
//                    streanWriter.WriteLine(writeHeadLine);
//                    streanWriter.Write(writeStringLine);
//                }
//            }
//            catch (Exception ex)
//            {
//                return false;
//            }
//            finally
//            {
//                if (streanWriter != null) streanWriter.Close();
//                if (fileStream != null) fileStream.Dispose();
//            }

            return true;
        }

        public Boolean saveExcelFile(DataGridView dataGridView, string className, bool captions)
        {
            //SaveFileDialog saveFileDialog = new SaveFileDialog();

            //saveFileDialog.FileName = className;
            //saveFileDialog.DefaultExt = "xls";
            //saveFileDialog.Filter = "Excel files (*.xls)|*.xls";
            //saveFileDialog.InitialDirectory = "c:\\";

            //DialogResult result = saveFileDialog.ShowDialog();

            //if (result == DialogResult.OK)
            //{
            //    int num = 0;
            //    object missingType = Type.Missing;

            //    Excel.Application objApp;
            //    Excel._Workbook objBook;
            //    Excel.Workbooks objBooks;
            //    Excel.Sheets objSheets;
            //    Excel._Worksheet objSheet;
            //    Excel.Range range;

            //    string[] headers = new string[dataGridView.ColumnCount];
            //    string[] columns = new string[dataGridView.ColumnCount];

            //    for (int c = 0; c < dataGridView.ColumnCount; c++)
            //    {
            //        headers[c] = dataGridView.Rows[0].Cells[c].OwningColumn.HeaderText.ToString();
            //        num = c + 65;
            //        columns[c] = Convert.ToString((char)num);
            //    }

            //    try
            //    {
            //        objApp = new Excel.Application();
            //        objBooks = objApp.Workbooks;
            //        objBook = objBooks.Add(Missing.Value);
            //        objSheets = objBook.Worksheets;
            //        objSheet = (Excel._Worksheet)objSheets.get_Item(1);

            //        if (captions)
            //        {
            //            for (int c = 0; c < dataGridView.ColumnCount; c++)
            //            {
            //                range = objSheet.get_Range(columns[c] + "1", Missing.Value);
            //                range.set_Value(Missing.Value, headers[c]);
            //            }
            //        }

            //        for (int i = 0; i < dataGridView.RowCount - 1; i++)
            //        {
            //            for (int j = 0; j < dataGridView.ColumnCount; j++)
            //            {
            //                range = objSheet.get_Range(columns[j] + Convert.ToString(i + 2),
            //                                                       Missing.Value);
            //                range.set_Value(Missing.Value,
            //                                      dataGridView.Rows[i].Cells[j].Value.ToString());
            //            }
            //        }

            //        objApp.Visible = false;
            //        objApp.UserControl = false;

            //        objBook.SaveAs(@saveFileDialog.FileName,
            //                  Microsoft.Office.Interop.Excel.XlFileFormat.xlWorkbookNormal,
            //                  missingType, missingType, missingType, missingType,
            //                  Microsoft.Office.Interop.Excel.XlSaveAsAccessMode.xlNoChange,
            //                  missingType, missingType, missingType, missingType, missingType);
            //        objBook.Close(false, missingType, missingType);

            //        Cursor.Current = Cursors.Default;

            //        //MessageBox.Show("Save Success!!!");
            //    }
            //    catch (Exception theException)
            //    {
            //        //String errorMessage;
            //        //errorMessage = "Error: ";
            //        //errorMessage = String.Concat(errorMessage, theException.Message);
            //        //errorMessage = String.Concat(errorMessage, " Line: ");
            //        //errorMessage = String.Concat(errorMessage, theException.Source);

            //        //MessageBox.Show(errorMessage, "Error");
            //        return false;
            //    }
            //}

            return true;
        }
    }
}
