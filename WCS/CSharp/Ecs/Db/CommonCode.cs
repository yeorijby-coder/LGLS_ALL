using System;
using System.Collections.Generic;

namespace Ecs.Db
{
    /// <summary>
    /// C++ COMMON_CODE 테이블 + CLib::GetCommonCodeLang 대응.
    /// (cdx_cd 코드분류, ccd_cd 코드값, wh_typ 창고, ccd_nm_kor 한글명)을 캐시해
    /// 코드값을 한글명으로 변환한다. 예: JOB_TYP '1' → "[01]입고".
    /// </summary>
    public class CommonCode
    {
        // key = "CDX|CCD|WH" → 한글명
        private readonly Dictionary<string, string> _kor =
            new(StringComparer.OrdinalIgnoreCase);

        public int Count => _kor.Count;
        public void Clear() => _kor.Clear();

        // wh_typ 컬럼에 "10," 처럼 콤마/공백이 섞인 데이터가 있어 정규화한다.
        private static string NormWh(string wh) => (wh ?? "").Trim().TrimEnd(',').Trim();

        public void Set(string cdx, string ccd, string wh, string kor)
            => _kor[$"{cdx}|{ccd}|{NormWh(wh)}"] = kor;

        /// <summary>코드 → 한글명. 미발견 시 wh=10로 폴백, 그래도 없으면 코드 원본 반환.</summary>
        public string GetName(string cdxCd, string ccdCd, string whTyp = "10")
        {
            if (string.IsNullOrEmpty(ccdCd)) return string.Empty;
            string wh = NormWh(whTyp);
            if (_kor.TryGetValue($"{cdxCd}|{ccdCd}|{wh}", out var n)) return n;
            if (_kor.TryGetValue($"{cdxCd}|{ccdCd}|10", out var n2)) return n2;
            return ccdCd;   // 미발견 시 원본 코드
        }
    }
}
