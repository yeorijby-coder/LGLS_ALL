using System;
using System.Collections.Generic;
using System.Text;

namespace TSK_HostCom
{
    //================================================================================================
    // [LGLS] IMS(WMS) 스테이션 코드 해석 기준 전환 모듈
    //
    //  배경 : IMS(WMS)가 송수신하는 스테이션 코드의 의미가 두 계보로 갈려 있어 어느 한쪽으로
    //         고정하면 반송 오류가 발생한다.
    //
    //   (1) ECS 기준  : 구 ECS 구축시드 DB(LglsMCS.TB_CODEMASTER CODECLASS='022', 2010~2011)
    //                   101=C/V#11(TR#22)  102=C/V#12(TR#24)  103=C/V#13(TR#26)
    //                   104=C/V#14(TR#29)  105=C/V#15(TR#30)  201~205=S/C#1~5
    //
    //   (2) WMS 기준  : WMS 운영 DB(LGLS_WMS.MC_STA_MST, WH_TYP='AA01', 2017 실측)
    //                   101=외부 입고 전용 입출고대(C/V#11 TR#22)
    //                   102=Picking 작업대 → 출고 도착 TR#29 / 재입고 출발 TR#30
    //                   103=제품 입고대(TR#24)   104=원부자재 불출대(TR#26)
    //                   110=실온 S/C#1 통로 C/V
    //                   ※ WMS 에는 105 코드가 없으므로 피킹 관련 보고는 102 로 되돌려 준다.
    //
    //  WCS 내부 코드 = 100 + 트랙번호  (예: TR#22 → 122, TR#30 → 130, 크레인은 901~905)
    //   근거 : cv_data(MC_NO 101~132 = TR#1~32), cv_def_inf(흐름정의),
    //          dest_pos_def(출고 목적지 1022/1026/1029, 901~905),
    //          job_mst_his(wh_typ=10 실적 121,153건이 12x/13x 체계 사용)
    //
    //  전환 방법 : EcsComA.ini  [Host] StationMapMode = ECS | WMS   (기본값 WMS)
    //              개별 코드는 [StationMap_ECS] / [StationMap_WMS] 섹션에서 재정의 가능하므로
    //              현장에서 재컴파일 없이 조정할 수 있다.
    //
    //  INI 표기 : IMS코드 = 출발내부코드 , 도착내부코드     (한 개만 쓰면 출발/도착 동일)
    //             예)  102 = 105,104
    //================================================================================================

    public enum EN_STA_MAP_MODE : int
    {
        enEcs = 0,      // 구 ECS 구축시드 DB 기준
        enWms = 1       // WMS 운영 DB 기준
    }

    public static class modStationMap
    {
        // 현재 적용 기준 (INI [Host]StationMapMode)
        public static EN_STA_MAP_MODE g_enMode = EN_STA_MAP_MODE.enWms;

        // IMS코드 → 내부코드.  값[0]=출발(Source), 값[1]=도착(Dest)
        private static Dictionary<int, int[]> m_dicToInner = new Dictionary<int, int[]>();

        // 내부코드 → IMS코드 (상위 보고용 역변환)
        private static Dictionary<int, int> m_dicToIms = new Dictionary<int, int>();

        // 로그/화면 표기용 명칭
        private static Dictionary<int, string> m_dicDesc = new Dictionary<int, string>();

        public static string ModeName
        {
            get { return (g_enMode == EN_STA_MAP_MODE.enEcs) ? "ECS(구 ECS DB 기준)" : "WMS(WMS 운영DB 기준)"; }
        }

        #region 기준별 기본 매핑 적재
        private static void SetDefault(EN_STA_MAP_MODE enMode)
        {
            m_dicToInner.Clear();
            m_dicToIms.Clear();
            m_dicDesc.Clear();

            if (enMode == EN_STA_MAP_MODE.enEcs)
            {
                //--- 구 ECS 구축시드 DB 기준 (TB_CODEMASTER '022' 의 장비 배정) -----------------
                AddMap(101, 122, 122, "C/V#11 입출고대(TR#22)");
                AddMap(102, 124, 124, "C/V#12 입고대(TR#24)");
                AddMap(103, 126, 126, "C/V#13 출고대(TR#26)");
                AddMap(104, 129, 129, "C/V#14 피킹 출고대(TR#29)");
                AddMap(105, 130, 130, "C/V#15 피킹 입고대(TR#30)");
            }
            else
            {
                //--- WMS 운영 DB 기준 -----------------------------------------------------------
                AddMap(101, 122, 122, "외부 입고 전용 입출고대(C/V#11 TR#22, 입출고 겸용)");

                // ★ 핵심 차이 : 102(Picking 작업대)는 방향에 따라 실제 트랙이 다르다.
                //    WCS 경로정의(cv_def_inf) 및 실 작업이력(job_mst_his) 확인 결과
                //      · 피킹 출고 도착 : TR#29 (내부 129)   ← 902/903 → 129  14,244건
                //      · 재입고   출발 : TR#30 (내부 130)   ← 130 → 901/902/903  19,013건
                //    TR#29 에서 출발하는 입고 작업은 이력상 0건이고 경로정의도 없으므로
                //    (C/V#14 는 127→129 출고 흐름만, C/V#15 가 130→131 입고 흐름을 가짐)
                //    재입고 출발은 TR#30 으로 변환한다.
                //    ※ WMS 는 이를 하나의 "피킹 작업대"로 인식하므로 보고 시 102 로 되돌린다.
                AddMap(102, 130, 129, "Picking 작업대(출고 도착 TR#29 / 재입고 출발 TR#30)");

                // 103/104 는 설비 용도(입고대/출고대) 기준으로 배정한다.
                //   103 제품 입고대    → TR#24 (C/V#12)
                //   104 원부자재 불출대 → TR#26 (C/V#13)
                //   ※ 종전 WmsStationToMcNo() 는 103=126 / 104=124 였으나(작업이력 기준),
                //      2026-08-24 협의로 설비 용도 기준(103=124 / 104=126)으로 변경.
                //      되돌리려면 INI [StationMap_WMS] 에  103=126 / 104=124 지정.
                AddMap(103, 124, 124, "제품 입고대(C/V#12 TR#24)");
                AddMap(104, 126, 126, "원부자재 불출대(C/V#13 TR#26)");

                // [LGLS 2026-08-22 계승] 명세(20100311) 밖 확장 코드.
                //   C/V#2 는 S/C#1 통로 겸용(방향전환형)이지만 WMS 작업대가 아니어서 명세에 코드가 없다.
                //   시뮬레이터에서 방향을 강제 전환할 수 있도록 105 를 C/V#2(TR#3)에 배정한다
                //   (현장 WMS 는 105 를 보내지 않으므로 기존 동작에 영향 없음).
                AddMap(105, 103, 103, "C/V#2 (S/C#1 통로 겸용, TR#3) - 시뮬 방향전환용 확장코드");

                // C/V IO 모드 전환(M 메시지) 전용 스테이션.
                //   명세서 표기 "실온 Stacker Crane 1호기 C/V" = S/C#1 통로 C/V(C/V#2).
                AddMap(110, 103, 103, "실온 S/C#1 통로 C/V(C/V#2 TR#3)");

                // 상위 보고 시 피킹존 트랙은 모두 102 로 되돌린다 (WMS 는 130 을 모른다).
                m_dicToIms[129] = 102;
                m_dicToIms[130] = 102;
            }

            //--- 하이랙(S/C) : 두 기준 공통 --------------------------------------------------
            //    수신은 001~005(명세서 표기) / 201~205(구 ECS DB 표기) 를 모두 받아
            //    내부 크레인 번호 901~905 로 변환한다.
            for (int i = 1; i <= 5; i++)
            {
                AddMap(i, 900 + i, 900 + i, string.Format("Hi-Rack S/C #{0}", i));
                AddMap(200 + i, 900 + i, 900 + i, string.Format("Hi-Rack S/C #{0}", i));
            }

            //    역변환(상위 보고)은 기준별로 다르다.
            //      ECS 기준 : 901~905 → 201~205 (구 ECS DB 표기)
            //      WMS 기준 : 901~905 → 001~005 (인터페이스 명세서 표기)
            //    ※ WMS 운영 DB 의 크레인 코드는 미확인 항목이므로, 상위와 값이 다르면
            //       INI [StationMap_xxx] 의 20x= / 00x= 항목으로 조정한다.
            int nCraneBase = (enMode == EN_STA_MAP_MODE.enEcs) ? 200 : 0;
            for (int i = 1; i <= 5; i++)
            {
                m_dicToIms[900 + i] = nCraneBase + i;
            }
        }

        private static void AddMap(int nIms, int nInnerSrc, int nInnerDest, string strDesc)
        {
            m_dicToInner[nIms] = new int[] { nInnerSrc, nInnerDest };
            m_dicDesc[nIms] = strDesc;

            // 역변환 기본값 : 먼저 등록된 것을 우선 (뒤에서 명시적으로 덮어쓸 수 있다)
            if (m_dicToIms.ContainsKey(nInnerSrc) == false) m_dicToIms[nInnerSrc] = nIms;
            if (m_dicToIms.ContainsKey(nInnerDest) == false) m_dicToIms[nInnerDest] = nIms;
        }
        #endregion

        #region INI 적재
        /// <summary>
        /// EcsComA.ini 에서 기준을 읽고, 기준별 기본 매핑 적재 후 개별 재정의를 반영한다.
        /// modCmLib.ReadInitProfile() 에서 1회 호출한다.
        /// </summary>
        public static void LoadFromIni()
        {
            StringBuilder sb = new StringBuilder(256);

            modDefAPI.GetPrivateProfileString("Host", "StationMapMode", "WMS", sb, sb.Capacity, modDefApp.MAIN_INI);
            string strMode = sb.ToString().Trim().ToUpper();

            g_enMode = (strMode == "ECS") ? EN_STA_MAP_MODE.enEcs : EN_STA_MAP_MODE.enWms;

            SetDefault(g_enMode);

            // 개별 코드 재정의 : [StationMap_ECS] / [StationMap_WMS]
            string strSection = (g_enMode == EN_STA_MAP_MODE.enEcs) ? "StationMap_ECS" : "StationMap_WMS";
            LoadOverride(strSection);

            // 상태보고 대표 설비 산출 후, INI 의 REPORT_MAIN_nnn 지정으로 덮어쓴다
            BuildReportMain();
            LoadReportMainOverride(strSection);
        }

        /// <summary>[StationMap_xxx] 의 REPORT_MAIN_nnn = 내부코드 항목을 반영</summary>
        private static void LoadReportMainOverride(string strSection)
        {
            foreach (int nIms in new List<int>(m_dicReportMain.Keys))
            {
                StringBuilder sb = new StringBuilder(64);
                string strKey = string.Format("REPORT_MAIN_{0:000}", nIms);
                modDefAPI.GetPrivateProfileString(strSection, strKey, "", sb, sb.Capacity, modDefApp.MAIN_INI);

                string strVal = sb.ToString().Trim();
                if (strVal.Length == 0) continue;

                int nCmt = strVal.IndexOfAny(new char[] { ';', '#' });
                if (nCmt >= 0) strVal = strVal.Substring(0, nCmt).Trim();

                int nMain;
                if (int.TryParse(strVal, out nMain)) m_dicReportMain[nIms] = nMain;
            }
        }

        /// <summary>
        /// 지정 섹션의 "IMS코드 = 출발,도착" 항목을 읽어 기본 매핑을 덮어쓴다.
        /// 존재하지 않는 섹션이면 아무 것도 하지 않는다.
        /// </summary>
        private static void LoadOverride(string strSection)
        {
            // 섹션 전체를 한 번에 읽는다(항목 사이는 NULL 로 구분됨)
            byte[] bytBuff = new byte[8192];
            int nLen = modDefAPI.GetPrivateProfileSection(strSection, bytBuff, bytBuff.Length, modDefApp.MAIN_INI);
            if (nLen <= 0) return;

            string strAll = Encoding.Default.GetString(bytBuff, 0, nLen);
            string[] arrLine = strAll.Split('\0');

            foreach (string strLineRaw in arrLine)
            {
                string strLine = strLineRaw.Trim();
                if (strLine.Length == 0) continue;
                if (strLine.StartsWith(";") || strLine.StartsWith("#")) continue;

                int nEq = strLine.IndexOf('=');
                if (nEq <= 0) continue;

                string strKey = strLine.Substring(0, nEq).Trim();
                string strVal = strLine.Substring(nEq + 1).Trim();

                // 주석 꼬리 제거
                int nCmt = strVal.IndexOfAny(new char[] { ';', '#' });
                if (nCmt >= 0) strVal = strVal.Substring(0, nCmt).Trim();

                int nIms;
                if (int.TryParse(strKey, out nIms) == false) continue;

                string[] arrVal = strVal.Split(',');
                int nSrc, nDest;
                if (int.TryParse(arrVal[0].Trim(), out nSrc) == false) continue;

                if (arrVal.Length >= 2)
                {
                    if (int.TryParse(arrVal[1].Trim(), out nDest) == false) nDest = nSrc;
                }
                else
                {
                    nDest = nSrc;
                }

                m_dicToInner[nIms] = new int[] { nSrc, nDest };
                m_dicToIms[nSrc] = nIms;
                m_dicToIms[nDest] = nIms;
                if (m_dicDesc.ContainsKey(nIms) == false)
                    m_dicDesc[nIms] = "(INI 재정의)";
            }
        }
        #endregion

        #region 변환
        /// <summary>
        /// IMS(WMS) 수신 스테이션 코드를 WCS 내부 코드로 변환한다.
        /// </summary>
        /// <param name="strImsCode">수신 코드 (3자리 문자열)</param>
        /// <param name="bSource">true=출발지(Source), false=도착지(Dest)</param>
        /// <param name="strInner">변환된 내부 코드 (3자리 문자열)</param>
        /// <returns>매핑표에 존재하면 true. false 인 경우 strInner 는 수신값 그대로 반환한다.</returns>
        public static bool ToInner(string strImsCode, bool bSource, ref string strInner)
        {
            strInner = strImsCode;

            int nIms;
            if (int.TryParse(strImsCode, out nIms) == false) return false;
            if (nIms == 0) return true;   // 0 = 미지정(랙 셀만 사용하는 경우)

            int[] arrInner;
            if (m_dicToInner.TryGetValue(nIms, out arrInner) == false) return false;

            int nInner = bSource ? arrInner[0] : arrInner[1];
            strInner = string.Format("{0:000}", nInner);
            return true;
        }

        /// <summary>
        /// WCS 내부 스테이션 코드를 IMS(WMS) 보고용 코드로 역변환한다.
        /// 매핑이 없으면 입력값을 그대로 돌려준다.
        /// </summary>
        public static string ToIms(string strInnerCode)
        {
            int nInner;
            if (int.TryParse(strInnerCode, out nInner) == false) return strInnerCode;

            int nIms;
            if (m_dicToIms.TryGetValue(nInner, out nIms) == false) return strInnerCode;

            return string.Format("{0:000}", nIms);
        }

        /// <summary>
        /// 상태보고 시 이 내부 코드를 상위로 보고해야 하는지 여부.
        ///
        /// WMS 기준에서는 서로 다른 내부 코드(예: 104 C/V#14, 105 C/V#15)가 같은 IMS 코드(102)로
        /// 역변환되므로 그대로 보내면 동일 스테이션에 대한 상태 전문이 2건 나가 상위가 혼란해진다.
        /// 같은 IMS 코드를 공유하는 내부 코드 중 가장 작은 값(대표 설비) 1건만 보고한다.
        /// 대표 설비는 [StationMap_xxx] 의 REPORT_MAIN_nnn 항목으로 재지정할 수 있다.
        ///   예)  REPORT_MAIN_102 = 105     ; 102 보고는 C/V#15 상태로 내보낸다
        /// </summary>
        public static bool IsReportTarget(string strInnerCode)
        {
            int nInner;
            if (int.TryParse(strInnerCode, out nInner) == false) return true;

            int nIms;
            if (m_dicToIms.TryGetValue(nInner, out nIms) == false) return true;   // 매핑 없으면 그대로 보고

            int nMain;
            if (m_dicReportMain.TryGetValue(nIms, out nMain) == false) return true;

            return (nInner == nMain);
        }

        /// <summary>IMS 코드별 상태보고 대표 내부코드</summary>
        private static Dictionary<int, int> m_dicReportMain = new Dictionary<int, int>();

        /// <summary>
        /// 역변환 테이블을 기준으로 IMS 코드별 대표 내부코드(최솟값)를 산출한다.
        /// SetDefault / LoadOverride 이후 1회 호출.
        /// </summary>
        private static void BuildReportMain()
        {
            m_dicReportMain.Clear();
            foreach (KeyValuePair<int, int> kv in m_dicToIms)
            {
                int nInner = kv.Key;
                int nIms = kv.Value;

                int nCur;
                if (m_dicReportMain.TryGetValue(nIms, out nCur) == false || nInner < nCur)
                    m_dicReportMain[nIms] = nInner;
            }
        }

        /// <summary>내부 코드가 크레인(하이랙)인지 여부</summary>
        public static bool IsCrane(string strInnerCode)
        {
            int nInner;
            if (int.TryParse(strInnerCode, out nInner) == false) return false;
            return (nInner >= 900);
        }

        /// <summary>로그 표기용 스테이션 명칭</summary>
        public static string GetDesc(string strImsCode)
        {
            int nIms;
            if (int.TryParse(strImsCode, out nIms) == false) return "";

            string strDesc;
            if (m_dicDesc.TryGetValue(nIms, out strDesc) == false) return "";
            return strDesc;
        }

        /// <summary>현재 적재된 매핑 전체를 로그용 문자열로 반환</summary>
        public static string DumpMap()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("[스테이션 매핑 기준] " + ModeName);
            foreach (KeyValuePair<int, int[]> kv in m_dicToInner)
            {
                if (kv.Key < 100) continue;     // 001~005 는 201~205 와 중복이므로 생략
                string strDesc = m_dicDesc.ContainsKey(kv.Key) ? m_dicDesc[kv.Key] : "";
                if (kv.Value[0] == kv.Value[1])
                    sb.Append(string.Format("  {0:000}→{1:000} {2}", kv.Key, kv.Value[0], strDesc));
                else
                    sb.Append(string.Format("  {0:000}→출발{1:000}/도착{2:000} {3}", kv.Key, kv.Value[0], kv.Value[1], strDesc));
            }
            return sb.ToString();
        }
        #endregion
    }
}
