using System;

namespace WcsCommon
{
    // ─────────────────────────────────────────────────────────────────────────
    // [LGLS 2026-09-09] WCS 버전 - 화면 표기의 단일 기준
    //
    //   종전에는 프로그램마다 날짜가 박혀 있어(HOST "Ver.2021.01.28",
    //   IO "2021.01.30", Client "2020.11.04 (V.01)") 같은 시스템인데 버전이
    //   제각각으로 보였다. 여기 한 줄만 고치면 TASK 쪽 표기가 함께 바뀐다.
    //   (C++ Client 는 Ecs\EcsDef.h 의 WCS_VERSION_STR 을 같은 값으로 둔다)
    //
    //   cLogCols.cs / cTaskLog.cs 와 같이 네 프로젝트에 링크로 공유된다.
    // ─────────────────────────────────────────────────────────────────────────
    public static class cWcsVer
    {
        /// <summary>표시용 버전 (예 : 1.0)</summary>
        public const string VERSION = "1.0";

        /// <summary>제품명</summary>
        public const string PRODUCT = "LGLS WCS";

        /// <summary>"LGLS WCS 1.0"</summary>
        public static string Title { get { return PRODUCT + " " + VERSION; } }

        /// <summary>상태표시줄용 "Ver.1.0"</summary>
        public static string Short { get { return "Ver." + VERSION; } }
    }
}
