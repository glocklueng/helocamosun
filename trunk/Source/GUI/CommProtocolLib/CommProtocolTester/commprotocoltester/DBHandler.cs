using System;
using System.Collections.Generic;
using System.Text;
using System.Data;
using System.Data.OleDb;
using System.Windows.Forms;

namespace commprotocoltester
{
    class DBHandler
    {
        string dbpath;

        public DBHandler(string dbpath)
        {

            this.dbpath = dbpath;
        }
        /// <summary>
        /// Import the table specified in tableName in the database specified by dbpath 
        /// into a DataTable
        /// </summary>
        /// <param name="tableName">Name of the table to import</param>
        /// <returns></returns>
        public DataTable GetDBData(string tableName)
        {
            string connection_string;
            string query;
            DataTable DbImport = new DataTable();
            DbImport.TableName = tableName;

            query = "SELECT * FROM (" + tableName + ");";

            connection_string = "Provider=Microsoft.Jet.OLEDB.4.0; Data Source=" + dbpath + ";";

            OleDbConnection Connection = new OleDbConnection(connection_string);

            OleDbDataAdapter accessOleAdapter = new OleDbDataAdapter(query, Connection);

            accessOleAdapter.Fill(DbImport);


            DbImport.TableName = tableName;

            return DbImport;
        }
        /// <summary>
        /// Merge all changes in DataTable sourceData to the imported DataSet
        /// </summary>
        /// <param name="sourceData">DataTable that contains changes</param>
        public void MergeChanges(DataTable sourceData)
        {
            string connection_string;
            string query;
            DataTable DestinationDataTable = new DataTable();

            query = "SELECT * FROM (" + sourceData.TableName + ");";

            connection_string = "Provider=Microsoft.Jet.OLEDB.4.0; Data Source=" + dbpath + ";";
            OleDbConnection Connection = new OleDbConnection(connection_string);
            OleDbDataAdapter accessOleAdapter = new OleDbDataAdapter(query, Connection);
            OleDbCommandBuilder myCB = new OleDbCommandBuilder(accessOleAdapter);
            Connection.Open();

            accessOleAdapter.Fill(DestinationDataTable);
            DestinationDataTable.TableName = sourceData.TableName;

            DestinationDataTable.Merge(sourceData, true, MissingSchemaAction.Ignore);

            try
            {

                accessOleAdapter.Update(DestinationDataTable.GetChanges());
                DestinationDataTable.AcceptChanges();
                sourceData.AcceptChanges();
            }
            catch (Exception E)
            {
                MessageBox.Show(E.Message);
                
            }
            Connection.Close();
        }
                /// <summary>
        /// Executes SQL commands that do not return data (ie: are not queries; 
        ///	Can execute INSERT statements, etc.). Connection string set up with OLEDB.
        /// </summary>
        /// <param name="szSQLstatement">a string representing the sql non-query command to execute</param>
        /// <returns>Returns true if the SQL was executed without error; otherwise returns false</returns>
        public bool ExecuteSQLNonQuery(String szSQLstatement)
        {
            String szConnectString;
            StringBuilder sbStrBuilder = new StringBuilder();

            szConnectString = sbStrBuilder.AppendFormat
                ("Provider=Microsoft.Jet.OLEDB.4.0; Data Source={0};", dbpath).ToString();

            OleDbConnection cConnect = new OleDbConnection(szConnectString);
            OleDbCommand cCommand = new OleDbCommand(szSQLstatement, cConnect);

            try
            {
                cCommand.Connection.Open();
                cCommand.ExecuteNonQuery();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
            cCommand.Connection.Close();
            // SQL command execution completed without errors
            return true;
        }
    }
    
}
