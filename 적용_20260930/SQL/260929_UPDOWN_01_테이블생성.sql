/* ============================================================================
   Client 프로그램 배포(Upload / Download) 용 테이블
   2026-09-29

   Upload  (WmsUp.exe)     : 고친 Client 파일을 DB 에 올린다
   Download(EcsClient.exe) : Client 를 띄우기 전에 DB 와 견주어 바뀐 것만 내려받는다

   현장 DB 는 SQL Server 2008 이다. SEQUENCE 는 2012 부터라 쓸 수 없어,
   채번을 1행짜리 테이블(DN_SEQ)로 둔다. 2012 이상에서도 그대로 돈다.

   몇 번을 돌려도 같은 결과가 되게 썼다(이미 있으면 건너뛴다).
   ★자료가 들어 있는 테이블은 지우지 않는다★ - 지우려면 아래 주석의 DROP 을 직접 쓴다.
   ============================================================================ */

USE [LGLS_MCS_IO]
GO

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

/* ── 다운로드 마스터 : 한 번의 업로드가 한 행 ─────────────────────────────── */
IF NOT EXISTS (SELECT 1 FROM sys.tables WHERE name = 'DN_MST')
BEGIN
    CREATE TABLE [dbo].[DN_MST](
        [DN_NO]     [numeric](4, 0) NOT NULL,   -- 업로드 번호(채번)
        [DN_PGM]    [varchar](50)       NULL,   -- 프로그램 구분  예) WCS
        [DN_DIR]    [varchar](100)      NULL,   -- 내려받을 폴더
        [DN_INF]    [varchar](128)      NULL,   -- 설명
        [UP_CPT_NM] [varchar](30)       NULL,   -- 올린 PC 이름
        [UP_DNT]    [datetime]          NULL,   -- 올린 시각
        CONSTRAINT [DN_MST_PK] PRIMARY KEY CLUSTERED ([DN_NO] ASC)
    ) ON [PRIMARY]
    PRINT 'DN_MST 만들었습니다.'
END
ELSE
    PRINT 'DN_MST 는 이미 있습니다 - 그대로 둡니다.'
GO

/* ── 업로드 파일 : 파일 하나가 한 행, 내용은 UP_DAT 에 그대로 들어간다 ────── */
IF NOT EXISTS (SELECT 1 FROM sys.tables WHERE name = 'UP_DOWN')
BEGIN
    CREATE TABLE [dbo].[UP_DOWN](
        [DN_NO]    [numeric](4, 0)  NOT NULL,   -- DN_MST.DN_NO
        [DN_LN]    [numeric](3, 0)  NOT NULL,   -- 파일 순번
        [UP_FNM]   [varchar](256)       NULL,   -- 올릴 때의 전체 경로
        [DN_NM]    [varchar](100)       NULL,   -- 파일 이름 (내려받을 때 쓴다)
        [DN_SIZE]  [numeric](10, 0)     NULL,   -- 바이트
        [DN_VER]   [numeric](5, 0)      NULL,   -- 판 번호 - 큰 것이 최신
        [UP_DAT]   [varbinary](max)     NULL,   -- ★파일 내용★
        [DN_PGM]   [varchar](50)        NULL,   -- 프로그램 구분
        [UP_DT]    [datetime]           NULL,   -- 올린 시각
        [DN_DIR]   [varchar](256)       NULL,   -- 내려받을 하위 폴더
        CONSTRAINT [UP_DOWN_PK] PRIMARY KEY CLUSTERED ([DN_NO] ASC, [DN_LN] ASC)
    ) ON [PRIMARY]
    PRINT 'UP_DOWN 만들었습니다.'
END
ELSE
    PRINT 'UP_DOWN 는 이미 있습니다 - 그대로 둡니다.'
GO

/* 내려받기는 "파일 이름으로 최신 판 찾기" 가 대부분이라 그 길을 내 준다 */
IF NOT EXISTS (SELECT 1 FROM sys.indexes WHERE name = 'UP_DOWN_IX1' AND object_id = OBJECT_ID('UP_DOWN'))
BEGIN
    CREATE INDEX [UP_DOWN_IX1] ON [dbo].[UP_DOWN] ([DN_NM] ASC, [DN_VER] ASC)
    PRINT 'UP_DOWN_IX1 만들었습니다.'
END
GO

/* ── 채번 ─────────────────────────────────────────────────────────────────
   SQL Server 2008 에는 SEQUENCE 가 없다. 1행짜리 표로 대신한다.
   UPDATE 한 문장 안에서 올리고 받아 오므로, 둘이 동시에 눌러도 번호가 겹치지 않는다.
   9999 를 넘으면 1 로 돌아간다(종전 SEQUENCE 의 CYCLE 과 같다).                */
IF NOT EXISTS (SELECT 1 FROM sys.tables WHERE name = 'DN_SEQ')
BEGIN
    CREATE TABLE [dbo].[DN_SEQ](
        [SEQ_NO] [numeric](4, 0) NOT NULL
    ) ON [PRIMARY]
    INSERT INTO [dbo].[DN_SEQ] ([SEQ_NO]) VALUES (0)
    PRINT 'DN_SEQ 만들었습니다.'
END
ELSE
    PRINT 'DN_SEQ 는 이미 있습니다 - 그대로 둡니다.'
GO

PRINT ''
PRINT '완료. 확인 :'
PRINT '  SELECT * FROM DN_MST;  SELECT DN_NO, DN_LN, DN_NM, DN_SIZE, DN_VER, UP_DT FROM UP_DOWN;'
GO

/* ── 되돌리려면(자료가 지워진다 - 필요할 때만 직접 실행) ──────────────────
DROP TABLE [dbo].[UP_DOWN];
DROP TABLE [dbo].[DN_MST];
DROP TABLE [dbo].[DN_SEQ];
   ------------------------------------------------------------------------ */
