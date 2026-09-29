
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NpgsqlTypes;

using System.Threading;
using System.Net;
using System.Collections;
using Microsoft.VisualBasic;
using System.Data.OleDb;
using System.Data.SqlClient;
using System.IO;
using Npgsql;
//최초작성자	: 권혁찬
//작성일		: 20171115
//화면개요		: WMS UPLOAD 프로그램(VB=>C# 컨버팅)
//수정이력		: 
namespace WmsUp
{
	public partial class WmsUp : Form
	{
		public WmsUp()
		{
			InitializeComponent();
		}

		//---------------------------------------------
		// 선언부
		//--------------------------------------------
		private int m_id_ypos;
		private string m_strSql;
		private int m_iSelcnt;
		private int m_iSelcnt2;
		private string m_strProgram;
		public string m_strDN_INF = "";

		//---------------------------------------------
		// 메인화면 로딩 및 초기화
		//---------------------------------------------
		private void WmsUp_Load(object sender, EventArgs e)
		{
			this.StatusLabel1.Text = "";
			this.StatusLabel2.Text = "";

			modCom.g_strPcIni_Path = System.IO.Path.GetFullPath(string.Copy(modCom.UP_INI));

			fp.InitSpread(ref fpSh, "다운로드 공통정보", false);
			fp2.InitSpread(ref fp2Sh, "다운로드 상세정보", false);
			subForm_init();
			gconDbinit();
			CUserDb Bdb = new CUserDb();

			modDateTime.getServerDate(ref Bdb, dtpFrDate, dtpFrTime, dtpToDate, dtpToTime);

			dtpFrDate.Value = dtpFrDate.Value.AddMonths(-3);
			dtpFrTime.Value = dtpFrTime.Value.AddMonths(-3);

            dtpToTime.Value = dtpToTime.Value.AddHours(+23);
            dtpToTime.Value = dtpToTime.Value.AddMinutes(+59);
            dtpToTime.Value = dtpToTime.Value.AddSeconds(+59);
		}

		private void WmsUp_Activated(object sender, EventArgs e)
		{
			gconDbinit();
			CUserDb Bdb = new CUserDb();
		}

		//---------------------------------------------
		// 목록 이벤트 처리부
		//---------------------------------------------
		private void fp_CellClick(object sender, EventArgs e)
		{
			if (fpSh.Rows.Count < 1)
			{
				Label2.Text = "";
				return;
			}

			QueryGrid2();
			Label2.Text = modCom.CRLF + " - 수정내역 - " + modCom.CRLF + modCom.CRLF + fp.GetGridText(fpSh.ActiveRowIndex, "DN_INF");
		}

		private void fp2_EnterCell(object sender, EventArgs e)
		{
			if (fpSh.Rows.Count < 1)
			{
				Label2.Text = "";
				return;
			}

			QueryGrid2();
			Label2.Text = modCom.CRLF + " - 수정내역 - " + modCom.CRLF + modCom.CRLF + fp.GetGridText(fpSh.ActiveRowIndex, "DN_INF");
		}
		
		//---------------------------------------------
		// 컨트롤 이벤트
		//---------------------------------------------
		private void Button5_Click(object sender, EventArgs e)
		{
			dtpToDate.Value = dtpFrDate.Value;
			dtpToTime.Value = dtpFrTime.Value;
		}

		private void btnSearch_Click(object sender, EventArgs e)
		{
#if oracle
			OleDbCommand cmd = new OleDbCommand();
#elif mssql
			SqlCommand cmd = new SqlCommand();
#elif postgresql
            NpgsqlCommand cmd = new NpgsqlCommand();
#endif

            string strPi_dwn = null;
			string strP_dwn = null;

			int iDialogRst = 0;

			CUserDb Bdb = new CUserDb();

			StatusLabel1.Text = "서버에 연결중";

			Application.DoEvents();

			StatusLabel1.Text = "서버 연결완료.";
			StatusLabel2.Text = modCom.g_strPcNm;

			StatusLabel1.Text = "테이블 자료 검색 중";

#if oracle
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + "    SELECT *                    ";
			m_strSql += modCom.CRLF + "      FROM TAB                  ";
			m_strSql += modCom.CRLF + "     WHERE TNAME = 'DN_MST'     ";
#elif mssql				
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + " SELECT *                                     ";
			m_strSql += modCom.CRLF + "   FROM sysobjects                            ";
			m_strSql += modCom.CRLF + "  WHERE ID = object_id('DN_MST')              ";
			m_strSql += modCom.CRLF + "    AND OBJECTPROPERTY(ID, 'IsUserTable') = 1 ";
#elif postgresql				
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + " SELECT *                                  ";
			m_strSql += modCom.CRLF + "   FROM information_schema.tables          ";
			m_strSql += modCom.CRLF + "  WHERE table_name = 'UP_DOWN'              ";
#endif
            m_iSelcnt = Bdb.ExcuteQry(m_strSql);
			
#if oracle
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + "    SELECT *                    ";
			m_strSql += modCom.CRLF + "      FROM TAB                  ";
			m_strSql += modCom.CRLF + "     WHERE TNAME = 'UP_DOWN'     ";
#elif mssql
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + " SELECT *                                     ";
			m_strSql += modCom.CRLF + "   FROM sysobjects                            ";
			m_strSql += modCom.CRLF + "  WHERE ID = object_id('UP_DOWN')              ";
			m_strSql += modCom.CRLF + "    AND OBJECTPROPERTY(ID, 'IsUserTable') = 1 ";
#elif postgresql				
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + " SELECT *                                  ";
			m_strSql += modCom.CRLF + "   FROM information_schema.tables          ";
			m_strSql += modCom.CRLF + "  WHERE table_name = 'dn_dtl'              ";
#endif
            m_iSelcnt2 = Bdb.ExcuteQry(m_strSql);


			if (m_iSelcnt <= 0 | m_iSelcnt2 <= 0)
			{
				StatusLabel1.Text = "Table이 없습니다. ";

				Application.DoEvents();

				iDialogRst = (int)MessageBox.Show("DOWNLOAD Table이 존재하지 않습니다." + modCom.CRLF + "Table을 생성 하시겠습니까?.", "Upload현황", MessageBoxButtons.OKCancel, MessageBoxIcon.Error);
				if (iDialogRst == (int)System.Windows.Forms.DialogResult.OK)
				{
					//______________________________________________________________________________
					// 다운로드 마스터 테이블 삭제 및 생성 (CASCASE)
					// 테이블이 존재하면 지우고 다시 만듬.
					//______________________________________________________________________________
#if oracle

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "    ALTER TABLE DN_MST                                                  ";
					m_strSql += modCom.CRLF + "     DROP PRIMARY KEY CASCADE                                           ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "     DROP TABLE DN_MST CASCADE CONSTRAINTS                              ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);
					//----------------------------------------------------------------------
					// MST 생성
					//----------------------------------------------------------------------
					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "CREATE TABLE DN_MST                                             ";
					m_strSql += modCom.CRLF + "(                                                               ";
					m_strSql += modCom.CRLF + "  DN_NO      NUMBER(4)                          NOT NULL,       ";
					m_strSql += modCom.CRLF + "  DN_PGM     VARCHAR2(50 BYTE),                                 ";
					m_strSql += modCom.CRLF + "  DN_DIR     VARCHAR2(100 BYTE),                                ";
					m_strSql += modCom.CRLF + "  DN_INF     VARCHAR2(128 BYTE),                                ";
					m_strSql += modCom.CRLF + "  UP_CPT_NM  VARCHAR2(30 BYTE),                                 ";
					m_strSql += modCom.CRLF + "  UP_DNT     DATE,                                              ";
					m_strSql += modCom.CRLF + "          CONSTRAINT DN_MST_PK PRIMARY KEY (DN_NO)                      ";
					m_strSql += modCom.CRLF + "                     USING INDEX TABLESPACE USERS                       ";
					m_strSql += modCom.CRLF + "                     STORAGE(INITIAL 16K NEXT 8K)                       ";
					m_strSql += modCom.CRLF + ")                                                               ";
					m_strSql += modCom.CRLF + "TABLESPACE USERS                                                ";
					m_strSql += modCom.CRLF + "PCTUSED    40                                                   ";
					m_strSql += modCom.CRLF + "PCTFREE    10                                                   ";
					m_strSql += modCom.CRLF + "INITRANS   1                                                    ";
					m_strSql += modCom.CRLF + "MAXTRANS   255                                                  ";
					m_strSql += modCom.CRLF + "STORAGE    (                                                    ";
					m_strSql += modCom.CRLF + "            INITIAL          64K                                ";
					m_strSql += modCom.CRLF + "            NEXT             1M                                 ";
					m_strSql += modCom.CRLF + "            MINEXTENTS       1                                  ";
					m_strSql += modCom.CRLF + "            MAXEXTENTS       UNLIMITED                          ";
					m_strSql += modCom.CRLF + "            PCTINCREASE      0                                  ";
					m_strSql += modCom.CRLF + "            BUFFER_POOL      DEFAULT                            ";
					m_strSql += modCom.CRLF + "           )                                                    ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);

					//￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣
					// 다운로드 디테일 테이블 삭제 및 생성 (CASCASE)
					// 테이블이 존재하면 지우고 다시 만듬.
					//______________________________________________________________________________

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "    ALTER TABLE UP_DOWN                                                  ";
					m_strSql += modCom.CRLF + "     DROP PRIMARY KEY CASCADE                                           ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "     DROP TABLE UP_DOWN CASCADE CONSTRAINTS                              ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);
					//----------------------------------------------------------------------
					// DTL 생성
					//----------------------------------------------------------------------
					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "CREATE TABLE UP_DOWN                                             ";
					m_strSql += modCom.CRLF + "(                                                               ";
					m_strSql += modCom.CRLF + "  DN_NO    NUMBER(4)   NOT NULL ,                               ";
					m_strSql += modCom.CRLF + "  DN_LN    NUMBER(3)           NOT NULL ,                       ";
					m_strSql += modCom.CRLF + "  UP_FNM   VARCHAR2(100 BYTE),                                  ";
					m_strSql += modCom.CRLF + "  DN_NM    VARCHAR2(50 BYTE),                                   ";
					m_strSql += modCom.CRLF + "  DN_SIZE  NUMBER(10),                                          ";
					m_strSql += modCom.CRLF + "  DN_VER   NUMBER(5),                                           ";
					m_strSql += modCom.CRLF + "  UP_DAT   BLOB,                                                ";
					m_strSql += modCom.CRLF + "          CONSTRAINT UP_DOWN_PK PRIMARY KEY (DN_NO, DN_LN)               ";
					m_strSql += modCom.CRLF + "                     USING INDEX TABLESPACE USERS                       ";
					m_strSql += modCom.CRLF + "                     STORAGE(INITIAL 16K NEXT 8K)                       ";
					m_strSql += modCom.CRLF + ")                                                               ";
					m_strSql += modCom.CRLF + "LOB (UP_DAT) STORE AS (                                         ";
					m_strSql += modCom.CRLF + "  TABLESPACE USERS                                              ";
					m_strSql += modCom.CRLF + "  ENABLE       STORAGE IN ROW                                   ";
					m_strSql += modCom.CRLF + "  CHUNK       8192                                              ";
					m_strSql += modCom.CRLF + "  RETENTION                                                     ";
					m_strSql += modCom.CRLF + "  NOCACHE                                                       ";
					m_strSql += modCom.CRLF + "  LOGGING)                                                      ";
					m_strSql += modCom.CRLF + "TABLESPACE USERS                                                ";
					m_strSql += modCom.CRLF + "PCTUSED    0                                                    ";
					m_strSql += modCom.CRLF + "PCTFREE    10                                                   ";
					m_strSql += modCom.CRLF + "INITRANS   1                                                    ";
					m_strSql += modCom.CRLF + "MAXTRANS   255                                                  ";
					m_strSql += modCom.CRLF + "STORAGE    (                                                    ";
					m_strSql += modCom.CRLF + "            INITIAL          32M                                ";
					m_strSql += modCom.CRLF + "            NEXT             16M                                ";
					m_strSql += modCom.CRLF + "            MINEXTENTS       1                                  ";
					m_strSql += modCom.CRLF + "            MAXEXTENTS       UNLIMITED                          ";
					m_strSql += modCom.CRLF + "            PCTINCREASE      0                                  ";
					m_strSql += modCom.CRLF + "            BUFFER_POOL      DEFAULT                            ";
					m_strSql += modCom.CRLF + "           )                                                    ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);


					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "    DROP SEQUENCE STD_USER.DN_SEQ ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);
					//----------------------------------------------------------------------
					// 시퀀스생성
					//----------------------------------------------------------------------
					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "          CREATE SEQUENCE STD_USER.DN_SEQ    ";
					m_strSql += modCom.CRLF + "               START WITH 1                  ";
					m_strSql += modCom.CRLF + "                 MAXVALUE 9999               ";
					m_strSql += modCom.CRLF + "                 MINVALUE 1                  ";
					m_strSql += modCom.CRLF + "                    CYCLE                    ";
					m_strSql += modCom.CRLF + "                  NOCACHE                    ";
					m_strSql += modCom.CRLF + "                  NOORDER                    ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false);
#elif mssql
					//------------------------------------------------------------------------------
					// 다운로드 마스터 테이블 삭제 및 생성 (CASCASE)
					// 테이블이 존재하면 지우고 다시 만듬.
					//------------------------------------------------------------------------------

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "   USE [" + modCom.g_strDatabase + "]                ";
					m_strSql += modCom.CRLF + "       DROP TABLE [dbo].[DN_MST]                  ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);
					//------------------------------------------------------------------------------
					// MST 생성
					//------------------------------------------------------------------------------
					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "USE [" + modCom.g_strDatabase + "]                                                                                                              ";
					m_strSql += modCom.CRLF + "SET ANSI_NULLS ON                                                                                                                        ";
					m_strSql += modCom.CRLF + "SET QUOTED_IDENTIFIER ON                                                                                                                 ";
					m_strSql += modCom.CRLF + "SET ANSI_PADDING ON                                                                                                                      ";
					m_strSql += modCom.CRLF + "CREATE TABLE [dbo].[DN_MST](                                                                                                             ";
					m_strSql += modCom.CRLF + "               [DN_NO] [numeric](4, 0) NOT NULL,                                                                                     ";
					m_strSql += modCom.CRLF + "               [DN_PGM] [varchar](50) NULL,                                                                                          ";
					m_strSql += modCom.CRLF + "               [DN_DIR] [varchar](100) NULL,                                                                                         ";
					m_strSql += modCom.CRLF + "               [DN_INF] [varchar](128) NULL,                                                                                         ";
					m_strSql += modCom.CRLF + "               [UP_CPT_NM] [varchar](30) NULL,                                                                                       ";
					m_strSql += modCom.CRLF + "               [UP_DNT] [datetime] NULL,                                                                                                 ";
					m_strSql += modCom.CRLF + "        CONSTRAINT [DN_MST_PK] PRIMARY KEY CLUSTERED                                                                                     ";
					m_strSql += modCom.CRLF + "(                                                                                                                                        ";
					m_strSql += modCom.CRLF + "[DN_NO] ASC                                                                                                                          ";
					m_strSql += modCom.CRLF + ")WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]    ";
					m_strSql += modCom.CRLF + ") ON [PRIMARY]                                                                                                                           ";
					m_strSql += modCom.CRLF + "SET ANSI_PADDING OFF                                                                                                                     ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "   USE [" + modCom.g_strDatabase + "]                ";
					m_strSql += modCom.CRLF + "       DROP TABLE [dbo].[UP_DOWN]                  ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);

					//------------------------------------------------------------------------------
					// DTL 생성
					//------------------------------------------------------------------------------

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "USE [" + modCom.g_strDatabase + "]                                                                                                           ";
					m_strSql += modCom.CRLF + "SET ANSI_NULLS ON                                                                                                                     ";
					m_strSql += modCom.CRLF + "SET QUOTED_IDENTIFIER ON                                                                                                              ";
					m_strSql += modCom.CRLF + "SET ANSI_PADDING ON                                                                                                                   ";
					// [LGLS 2026-09-29] Download 가 읽는 컬럼 세 개(DN_PGM/UP_DT/DN_DIR)가 빠져 있었다.
					//   UP_DAT 은 파일 내용이라 varbinary 여야 한다 - varchar 에 넣으면 내용이 깨진다.
					m_strSql += modCom.CRLF + "CREATE TABLE [dbo].[UP_DOWN](                                                                                                          ";
					m_strSql += modCom.CRLF + "               [DN_NO] [numeric](4, 0) NOT NULL,                                                                                  ";
					m_strSql += modCom.CRLF + "               [DN_LN] [numeric](3, 0) NOT NULL,                                                                                  ";
					m_strSql += modCom.CRLF + "               [UP_FNM] [varchar](256) NULL,                                                                                      ";
					m_strSql += modCom.CRLF + "               [DN_NM] [varchar](100) NULL,                                                                                       ";
					m_strSql += modCom.CRLF + "               [DN_SIZE] [numeric](10, 0) NULL,                                                                                   ";
					m_strSql += modCom.CRLF + "               [DN_VER] [numeric](5, 0) NULL,                                                                                     ";
					m_strSql += modCom.CRLF + "               [UP_DAT] [varbinary](max) NULL,                                                                                    ";
					m_strSql += modCom.CRLF + "               [DN_PGM] [varchar](50) NULL,                                                                                       ";
					m_strSql += modCom.CRLF + "               [UP_DT] [datetime] NULL,                                                                                           ";
					m_strSql += modCom.CRLF + "               [DN_DIR] [varchar](256) NULL,                                                                                      ";
					m_strSql += modCom.CRLF + " CONSTRAINT [UP_DOWN_PK] PRIMARY KEY CLUSTERED                                                                                         ";
					m_strSql += modCom.CRLF + "(                                                                                                                                     ";
					m_strSql += modCom.CRLF + "[DN_NO] ASC,                                                                                                                      ";
					m_strSql += modCom.CRLF + "[DN_LN] ASC                                                                                                                       ";
					m_strSql += modCom.CRLF + ")WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY] ";
					m_strSql += modCom.CRLF + ") ON [PRIMARY] TEXTIMAGE_ON [PRIMARY]                                                                                                 ";
					m_strSql += modCom.CRLF + "SET ANSI_PADDING OFF                                                                                                                  ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);
					//------------------------------------------------------------------------------
					// 시퀀스생성
					//------------------------------------------------------------------------------
					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "   USE [" + modCom.g_strDatabase + "]                ";
					// [LGLS 2026-09-29] SEQUENCE 는 SQL Server 2012 부터다. 현장은 2008 이라 못 쓴다.
					//   1행짜리 표로 대신한다 - UPDATE 한 문장 안에서 올리고 받으므로 번호가 겹치지 않는다.
					m_strSql += modCom.CRLF + "   CREATE TABLE [dbo].[DN_SEQ]( [SEQ_NO] [numeric](4, 0) NOT NULL )  ";
					m_strSql += modCom.CRLF + "   INSERT INTO [dbo].[DN_SEQ] ([SEQ_NO]) VALUES (0)                  ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false);
#elif postgresql

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "     DROP TABLE DN_MST                         ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);
					//----------------------------------------------------------------------
					// MST 생성
					//----------------------------------------------------------------------
					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "CREATE TABLE dn_mst (                                             ";
					m_strSql += modCom.CRLF + "  dn_no int4 NOT NULL,       ";
					m_strSql += modCom.CRLF + "  dn_pgm varchar(50) NOT NULL,                                 ";
					m_strSql += modCom.CRLF + "  dn_dir varchar(100) NULL,                                ";
					m_strSql += modCom.CRLF + "  dn_inf varchar(128) NULL,                                ";
					m_strSql += modCom.CRLF + "  up_cpt_nm varchar(30) NULL,                                 ";
					m_strSql += modCom.CRLF + "  up_dnt timestamp NULL,                                ";
					m_strSql += modCom.CRLF + "  CONSTRAINT dn_mst_pkey PRIMARY KEY (dn_no) )           ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);

                    m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "CREATE INDEX dn_mst_idx ON dn_mst USING btree (dn_no NULLS FIRST)   ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);

					//￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣
					// 다운로드 디테일 테이블 삭제 및 생성 (CASCASE)
					// 테이블이 존재하면 지우고 다시 만듬.
					//______________________________________________________________________________

					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "     DROP DROP TABLE dn_dtl                                ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false, false);
					//----------------------------------------------------------------------
					// DTL 생성
					//----------------------------------------------------------------------
					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "CREATE TABLE dn_dtl (                                             ";
					m_strSql += modCom.CRLF + "  dn_no int4 NOT NULL,                              ";
					m_strSql += modCom.CRLF + "  dn_ln varchar(3) NOT NULL,                      ";
					m_strSql += modCom.CRLF + "  up_fnm varchar(100) NULL,                                ";
					m_strSql += modCom.CRLF + "  dn_nm varchar(50) NULL,                                  ";
					m_strSql += modCom.CRLF + "  dn_size varchar(10) NULL,                                   ";
					m_strSql += modCom.CRLF + "  dn_ver varchar(5) NULL,                                       ";
					m_strSql += modCom.CRLF + "  up_dat bytea NULL,                                          ";
					m_strSql += modCom.CRLF + "  CONSTRAINT dn_dtl_pkey PRIMARY KEY (dn_no, dn_ln))             ";
					
					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);


					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "    DROP SEQUENCE public.dn_seq ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);
					//----------------------------------------------------------------------
					// 시퀀스생성
					//----------------------------------------------------------------------
					m_strSql = modCom.CRLF + "";
					m_strSql += modCom.CRLF + "          CREATE SEQUENCE public.dn_seq    ";
					m_strSql += modCom.CRLF + "               INCREMENT BY 1                 ";
					m_strSql += modCom.CRLF + "                 MINVALUE 1              ";
					m_strSql += modCom.CRLF + "                 MAXVALUE 9999                 ";

					m_iSelcnt = Bdb.ExcuteNonQry(m_strSql, false);
#endif
                }
				QueryGrid();

				return;
			}

			StatusLabel1.Text = "테이블 자료 검색 중";

			fpSh.RowCount = 0;

			strPi_dwn = new string(' ', 100);
			modCom.ReadInitProfile_C(ref strPi_dwn, "DOWNLOAD PROGRAM", "CNT");
			m_strProgram = "";

			for (int i = 1; i <= Convert.ToInt32(strPi_dwn); i++)
			{
				strP_dwn = new string(' ', 100);
				modCom.ReadInitProfile_C(ref strP_dwn, "DOWNLOAD PROGRAM", i.ToString());
				if (string.IsNullOrEmpty(m_strProgram))
				{
					m_strProgram = strP_dwn;
				}
				else
				{
					m_strProgram += "','" + strP_dwn;
				}
			}

			QueryGrid();

			modCom.gconDb.Close();

		}

		private void btnDel_Click(object sender, EventArgs e)
		{

			CUserDb Bdb = new CUserDb();
			int iDN_NO = 0;
			int iRowIdx = -1;
			iRowIdx = fpSh.ActiveRowIndex;

			if (fpSh.RowCount < 1) {
				modCmWork.ShowError("삭제할 데이터를 선택하여 주십시오.", this.Text);
				return;
			}

			iDN_NO = Convert.ToInt32(fp.GetGridText(iRowIdx, "DN_NO"));

			try
			{
				Bdb.BeginTrans();

				m_strSql = modCom.CRLF + "";
				m_strSql += modCom.CRLF + "    DELETE FROM DN_MST                       ";
				m_strSql += modCom.CRLF + "     WHERE DN_NO = " + iDN_NO + "       ";


				m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);

				if (m_iSelcnt < 0)
				{
					throw new Exception("다운로드 공통 테이블의 데이터를 DELETE 중 에러가 발생하였습니다.");
				}

				m_strSql = modCom.CRLF + "";
				m_strSql += modCom.CRLF + "    DELETE FROM UP_DOWN                       ";
				m_strSql += modCom.CRLF + "     WHERE DN_NO = " + iDN_NO + "       ";

				m_iSelcnt = Bdb.ExcuteNonQry(m_strSql);

				if (m_iSelcnt < 0)
				{
					throw new Exception("다운로드 상세 테이블의 데이터를 DELETE 중 에러가 발생하였습니다.");
				}

				Bdb.CommitTrans();

				modCmWork.ShowMsg("삭제되었습니다.", this.Text);
				QueryGrid();
			}
			catch (Exception ex)
			{
				Bdb.RollbackTrans();
				modCmWork.ShowError(ex.Message, this.Text);
			}
		}

		private void btnEnd_Click(object sender, EventArgs e)
		{
			this.Close();
		}

		private void btnSet_Click(object sender, EventArgs e)
		{
			WmsSet sForm = new WmsSet();

			sForm.ShowDialog();
		}

		private void btnUpload_Click(object sender, EventArgs e)
		{
			FileUp frmFileUp = new FileUp();

			frmFileUp.ShowDialog();
		}
		//---------------------------------------------
		// 함수
		//---------------------------------------------

		//Db연결을 한다.
		public void gconDbinit()
		{
			//DB 서버 접속
			if (modCom.gconDb == null)
			{
				if (!modCom.DBLogIn(ref modCom.gconDb))
				{
					//DB서버 접속 실패시 프로그램을 종료한다.
					System.Environment.Exit(0);
				}
			}
			else
			{
				if (modCom.gconDb.State == ConnectionState.Closed)
				{
					if (!modCom.DBLogIn(ref modCom.gconDb))
					{
						//DB서버 접속 실패시 프로그램을 종료한다.
						System.Environment.Exit(0);
					}
				}
			}
		}

		private void subForm_init()
		{
			string pTitle = null;
			string pSet_Btn = null;
			string pDn_St = null;

			////--------------------------------
			//// Caption 적용
			////--------------------------------
			pTitle = new string(' ', 100);
			modCom.ReadInitProfile_C(ref pTitle, "APPLICATION", "TITLE");

			this.Text = pTitle;

			//'//--------------------------------
			//'// ICon 적용
			//'//--------------------------------
			//pico = New String(" ", 100)
			//ReadInitProfile_C(pico, "APPLICATION", "ICON")

			//If Dir(pico) <> "" Then
			//    ' Create icon.
			//    Dim newIcon As New Icon(pico)

			//    Me.Icon = newIcon
			//End If

			////--------------------------------
			//// 설정 Button 적용
			////--------------------------------
			pSet_Btn = new string(' ', 100);
			modCom.ReadInitProfile_C(ref pSet_Btn, "APPLICATION", "SETUP_BTN");

			if (pSet_Btn == "2")
			{
				this.btnSet.Enabled = false;
			}
			else
			{
				this.btnSet.Enabled = true;
			}

			////--------------------------------
			//// DownLoad 시작 적용
			////--------------------------------
			pDn_St = new string(' ', 100);
			modCom.ReadInitProfile_C(ref pDn_St, "APPLICATION", "DOWN_LOAD_PATH_SET");

		}

		private void QueryGrid()
		{
			CUserDb QBdb = new CUserDb(true);

			string strIN_DT_FROM = null;
			string strIN_DT_TO = null;

			strIN_DT_FROM = modDateTime.GetDateCode(dtpFrDate.Value) + modDateTime.GetTimeCode(dtpFrTime.Value);
			strIN_DT_TO = modDateTime.GetDateCode(dtpToDate.Value) + modDateTime.GetTimeCode(dtpToTime.Value);
#if oracle
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + "            SELECT DN_NO                                                   ";
			m_strSql += modCom.CRLF + "                 , DN_PGM                                                  ";
			m_strSql += modCom.CRLF + "                 , DN_DIR                                                  ";
			m_strSql += modCom.CRLF + "                 , DN_INF                                                  ";
			m_strSql += modCom.CRLF + "                 , UP_CPT_NM                                               ";
			m_strSql += modCom.CRLF + "                 , UP_DNT                                                  ";
			m_strSql += modCom.CRLF + "              FROM DN_MST                                                  ";
			m_strSql += modCom.CRLF + "             WHERE DN_PGM IN ('" + m_strProgram + "')                      ";
			m_strSql += modCom.CRLF + "               AND UP_DNT >= '" + strIN_DT_FROM + "'                       ";
			m_strSql += modCom.CRLF + "               AND UP_DNT <= '" + strIN_DT_TO + "'                         ";
			m_strSql += modCom.CRLF + "          ORDER BY UP_DNT DESC                                             ";
#elif mssql
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + "            SELECT DN_NO                                                   ";
			m_strSql += modCom.CRLF + "                 , DN_PGM                                                  ";
			m_strSql += modCom.CRLF + "                 , DN_DIR                                                  ";
			m_strSql += modCom.CRLF + "                 , DN_INF                                                  ";
			m_strSql += modCom.CRLF + "                 , UP_CPT_NM                                               ";
			m_strSql += modCom.CRLF + "                 , UP_DNT                                                  ";
			m_strSql += modCom.CRLF + "              FROM DN_MST                                                  ";
			m_strSql += modCom.CRLF + "             WHERE DN_PGM IN ('" + m_strProgram + "')                      ";
			// [LGLS 2026-09-29] wms_sf_Get_DateTime_KMS 는 앞 현장의 함수다 - 여기엔 없다.
			//   같은 값(YYYYMMDDHHMMSS)을 표준 변환으로 만든다.
			m_strSql += modCom.CRLF + "               AND REPLACE(REPLACE(REPLACE(CONVERT(VARCHAR(20), UP_DNT, 120),'-',''),':',''),' ','') >= '" + strIN_DT_FROM + "' ";
			m_strSql += modCom.CRLF + "               AND REPLACE(REPLACE(REPLACE(CONVERT(VARCHAR(20), UP_DNT, 120),'-',''),':',''),' ','') <= '" + strIN_DT_TO + "' ";
			m_strSql += modCom.CRLF + "          ORDER BY UP_DNT DESC                                             ";
#elif postgresql
			m_strSql = modCom.CRLF + "";
			m_strSql += modCom.CRLF + "            SELECT DN_NO                                                   ";
			m_strSql += modCom.CRLF + "                 , DN_PGM                                                  ";
			m_strSql += modCom.CRLF + "                 , DN_DIR                                                  ";
			m_strSql += modCom.CRLF + "                 , DN_INF                                                  ";
			m_strSql += modCom.CRLF + "                 , UP_CPT_NM                                               ";
			m_strSql += modCom.CRLF + "                 , UP_DNT                                                  ";
			m_strSql += modCom.CRLF + "              FROM DN_MST                                                  ";
			m_strSql += modCom.CRLF + "             WHERE DN_PGM IN ('" + m_strProgram + "')                      ";
            m_strSql += modCom.CRLF + "   AND TO_CHAR(UP_DNT , 'YYYYMMDDHH24MISS') BETWEEN '" + strIN_DT_FROM + "' AND '" + strIN_DT_TO + "'";
            m_strSql += modCom.CRLF + "          ORDER BY UP_DNT DESC      ";
#endif

			try
			{
				m_iSelcnt = QBdb.ExcuteQry(m_strSql);

				if (m_iSelcnt < 0)
				{
					throw new Exception("다운로드 공통정보 테이블 조회중 DB 에러가 발생하였습니다.");
				}
				else if (m_iSelcnt == 0)
				{
					StatusLabel1.Text = "자료가 없습니다.";
					modCmWork.ShowMsgGrid(ref fpSh, "조회건수:" + 0);
					modCmWork.ShowMsgGrid(ref fp2Sh, "조회건수:" + 0);
					fpSh.DataSource = QBdb.dtMain;
					fp2Sh.DataSource = QBdb.dtMain;
					m_strDN_INF = "";
					Label2.Text = "";
				}
				else if (m_iSelcnt > 0)
				{
					fpSh.DataSource = QBdb.dtMain;
					modCmWork.ShowMsgGrid(ref fpSh, "조회 건수: " + m_iSelcnt.ToString());
					QueryGrid2();
					m_strDN_INF = QBdb.dtMain.Rows[0]["DN_INF"].ToString().Trim();
					Label2.Text = modCom.CRLF + " - 수정내역 - " + modCom.CRLF + modCom.CRLF + m_strDN_INF;
					StatusLabel1.Text = "조회 작업 완료.";
				}

			}
			catch (Exception ex)
			{
				modCmWork.ShowError(ex.Message, "조회 에러");
			}
		}


		private void QueryGrid2()
		{
			CUserDb QBdb2 = new CUserDb(true);
			int iDN_NO = 0;
			int iRowIdx = 0;
			iRowIdx = fpSh.ActiveRowIndex;

			if (iRowIdx != -1)
			{
				iDN_NO = Convert.ToInt32(fp.GetGridText(iRowIdx, "DN_NO"));
			}


			try
			{
				m_strSql = modCom.CRLF + "";
				m_strSql += modCom.CRLF + "    SELECT DN_NO                            ";
				m_strSql += modCom.CRLF + "         , DN_LN                            ";
				m_strSql += modCom.CRLF + "         , UP_FNM                           ";
				m_strSql += modCom.CRLF + "         , DN_NM                            ";
				m_strSql += modCom.CRLF + "         , DN_SIZE                          ";
				m_strSql += modCom.CRLF + "         , DN_VER                           ";
				m_strSql += modCom.CRLF + "      FROM UP_DOWN                           ";
				m_strSql += modCom.CRLF + "     WHERE DN_NO = '" + iDN_NO + "'         ";
				m_strSql += modCom.CRLF + "  ORDER BY DN_NO, DN_LN                     ";

				m_iSelcnt = QBdb2.ExcuteQry(m_strSql);

				if (m_iSelcnt < 0)
				{
					throw new Exception("다운로드 상세정보 테이블 조회중 DB 에러가 발생하였습니다.");
				}
				else if (m_iSelcnt > 0)
				{
					fp2Sh.DataSource = QBdb2.dtMain;
					modCmWork.ShowMsgGrid(ref fp2Sh, "조회 건수: " + m_iSelcnt.ToString());
				}

			}
			catch (Exception ex)
			{
				modCmWork.ShowError(ex.Message, "조회 에러");
			}
		}

		private void btnDownload_Click(object sender, EventArgs e)
		{					
#if oracle	
				OleDbCommand cmd = new OleDbCommand();
            
#elif mssql
				SqlCommand cmd = new SqlCommand();

#elif postgresql
				NpgsqlCommand cmd = new NpgsqlCommand();

#endif
                int iPictureCol = 6;
				// the column # of the BLOB field
			
				ushort Attributes = 0;
				ushort NewAttributes = 0;
				ushort uaReadOnly = 0;
				//// b_ReadOnly
				bool blReadOnly = false;
				bool blArchive = false;
				bool blSystem = false;
				bool blHidden = false;
			
				int iDN_NO = 0;
				int iRowIdx = 0;
				int iCnt = 0;
			
				string strWK_DN_PGM = null;
				string strWK_UP_FNM = null;
				string strDOWN_PATH = null;
				string strDown_Name = null;
				string strWK_DN_FNM = "";
			
				double dblWK_DN_SIZE = 0;
				double dblWK_DN_VER = 0;
			
				bool bldown_load_ok = false;
			
				iRowIdx = fpSh.ActiveRowIndex;
			
				if (iRowIdx != -1) {
					iDN_NO = Convert.ToInt32(fp.GetGridText(iRowIdx, "DN_NO"));
				} else {
					return;
				}
			
				try {
					cmd.Connection = modCom.gconDb;
					cmd.CommandText = "";
					cmd.CommandText += "            SELECT A.DN_PGM                                   ";
					cmd.CommandText += "                 , B.UP_FNM                                   ";
					cmd.CommandText += "                 , B.DN_VER                                   ";
					cmd.CommandText += "                 , B.DN_SIZE                                  ";
					cmd.CommandText += "                 , B.DN_NM                                    ";
					cmd.CommandText += "                 , A.DN_DIR                                   ";
					cmd.CommandText += "                 , B.UP_DAT                                   ";
					cmd.CommandText += "                 , B.DN_NO                                    ";
					cmd.CommandText += "              FROM DN_MST A                                   ";
					cmd.CommandText += "        INNER JOIN UP_DOWN B                                   ";
					cmd.CommandText += "                ON A.DN_NO = B.DN_NO                          ";
					cmd.CommandText += "             WHERE A.DN_NO = '" + iDN_NO + "'               ";
					cmd.CommandText += "          ORDER BY A.DN_PGM , B.UP_FNM                        ";
			
					if (cmd.Connection.State ==  ConnectionState.Closed) {
						throw new Exception("연결이 닫힌 상태입니다. 다운로드를 재시도 하십시오.");
					}
#if oracle
					OleDbDataReader dr_s = cmd.ExecuteReader();
                    
#elif mssql
					SqlDataReader dr_s = cmd.ExecuteReader();

#elif postgresql
					NpgsqlDataReader dr_s = cmd.ExecuteReader();
                     
#endif
                    iCnt = 0;
					if (dr_s.HasRows == true) {
						while ((dr_s.Read())) {
							strWK_DN_PGM = dr_s.GetString(0);
							// DN_PGM
							strWK_UP_FNM = dr_s.GetString(1);
							// UP_FNM
#if oracle
							dblWK_DN_VER = (double)dr_s.GetDecimal(2);
							// DN_VER
							dblWK_DN_SIZE = (double)dr_s.GetDecimal(3);
							// DN_SIZE
#elif mssql
							dblWK_DN_VER = (double)dr_s.GetDecimal(2);
							// DN_VER
							dblWK_DN_SIZE = (double)dr_s.GetDecimal(3);
							// DN_SIZE
#elif postgresql
							dblWK_DN_VER = (double)dr_s.GetDecimal(2);
							// DN_VER
							dblWK_DN_SIZE = (double)dr_s.GetDecimal(3);
							// DN_SIZE
#endif
                            strDown_Name = dr_s.GetString(4);
							// DN_NM
							strDOWN_PATH = dr_s.GetString(5);
							// DN_DIR
			
							strWK_DN_FNM = strDOWN_PATH + "\\" + strDown_Name;
			
							bldown_load_ok = Client_Dir_Create(strWK_DN_FNM);
			
							blReadOnly = false;
							blArchive = false;
							blSystem = false;
							blHidden = false;
			
							if (strDown_Name == FileSystem.Dir(strWK_DN_FNM)) {
								////-------------------------------------------------
								//// 파일의 속성이 ReadOnly이면 오류가 발생하므로
								//// 속성을 ReadOnly = False로 변경한다.
								////-------------------------------------------------
								Attributes = (ushort)FileSystem.GetAttr(strWK_DN_FNM);
								blReadOnly = ((FileAttribute)Attributes & Constants.vbReadOnly) == Constants.vbReadOnly;
								blArchive = ((FileAttribute)Attributes & Constants.vbArchive) == Constants.vbArchive;
								blSystem = ((FileAttribute)Attributes & Constants.vbSystem) == Constants.vbSystem;
								blHidden = ((FileAttribute)Attributes & Constants.vbHidden) == Constants.vbHidden;
			
 

								if (blReadOnly == true) {
									System.IO.File.SetAttributes(strWK_DN_FNM, System.IO.FileAttributes.Normal);  
								}
							}
			
							byte[] b = new byte[dr_s.GetBytes(iPictureCol, 0, null, 0, int.MaxValue)];
			
			
							dr_s.GetBytes(iPictureCol, 0, b, 0, b.Length);
			
							System.IO.FileStream fs = new System.IO.FileStream(strWK_DN_FNM, System.IO.FileMode.Create, System.IO.FileAccess.Write);
			
							fs.Write(b, 0, b.Length);
			
							fs.Close();
			
			
						}
					}
					modCmWork.ShowMsg("다운로드가 완료되었습니다.", this.Text);
					dr_s.Close();
					QueryGrid();
				} catch (Exception ex) {
					modCmWork.ShowError(ex.Message, "조회 에러");
				}

		}


		private bool Client_Dir_Create(string pFileName)
		{
			string Client_Down_DIR = null;
			string Make_Dir = null;
			string Imsi_Dir = null;
			int iPos = 0;
			int jPos = 0;

			iPos = Strings.InStrRev(pFileName, "\\");
			if (iPos <= 0)
			{
				MessageBox.Show("Download file 에러입니다.", "Download 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}

			Client_Down_DIR = Strings.Left(pFileName, iPos);

			if (Strings.Right(Client_Down_DIR, 1) == "\\")
			{
				Client_Down_DIR = Strings.Left(Client_Down_DIR, Strings.Len(Client_Down_DIR) - 1);
			}

			Make_Dir = Client_Down_DIR;

			while (true)
			{
				if (!Directory.Exists(Make_Dir))
				{
					jPos = Strings.InStrRev(pFileName, "\\");

					if (jPos <= 0)
					{
						MessageBox.Show("Download 디렉토리를 만들 수 없습니다.", "Download 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
						return false;
					}

					Client_Down_DIR = Strings.Left(pFileName, iPos);
				}
				else
				{
					if (Make_Dir == Client_Down_DIR)
					{
						return true;
					}
				}

				jPos = Strings.Len(Make_Dir) + 1;
				Imsi_Dir = Strings.Mid(Client_Down_DIR, jPos + 1, Strings.Len(Client_Down_DIR) - jPos);
				iPos = Strings.InStr(Imsi_Dir, "\\");

				if (iPos > 0)
				{
					Make_Dir += "\\" + Strings.Mid(Imsi_Dir, 1, iPos - 1);
				}
				else
				{
					Make_Dir += "\\" + Imsi_Dir;
				}

				DirectoryInfo DirInfo = Directory.CreateDirectory(Make_Dir);


				if (!DirInfo.Exists)
				{
					MessageBox.Show("신규 Download 디렉토리를 만들 수 없습니다.", "Download 에러", MessageBoxButtons.OK, MessageBoxIcon.Error);
					return false;
				}
			}
			return true;
		}
	}
}
