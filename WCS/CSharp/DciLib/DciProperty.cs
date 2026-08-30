using System;
using System.Collections.Generic;
using System.Windows.Media;

namespace DciLib
{
    /// <summary>
    /// C++의 CDciProperty에 대응. 컨트롤의 시각적 속성을 담는다.
    /// </summary>
    public class DciProperty
    {
        public string Name { get; set; } = string.Empty;
        public string Value { get; set; } = string.Empty;

        public DciProperty() { }
        public DciProperty(string name, string value) { Name = name; Value = value; }
    }

    /// <summary>
    /// C++의 COLORREF를 WPF Color로 변환하는 헬퍼.
    /// EcsLayout.xml의 bgcolor/fgcolor 속성은 "RRGGBB" 16진 문자열로 저장된다.
    /// </summary>
    public static class DciColor
    {
        public static Color FromHexString(string hex, Color def = default)
        {
            if (string.IsNullOrEmpty(hex)) return def == default ? Colors.Black : def;
            hex = hex.TrimStart('#');
            if (hex.Length == 6)
            {
                try
                {
                    // C++ CConvert::ToColor는 hex를 COLORREF(0x00BBGGRR)로 파싱한다.
                    // 즉 XML "RRGGBB" 문자열은 실제로 B/G/R 순(첫바이트=B, 끝바이트=R).
                    // C++와 동일하게 R/B를 스왑해서 읽는다.
                    byte b = Convert.ToByte(hex.Substring(0, 2), 16);
                    byte g = Convert.ToByte(hex.Substring(2, 2), 16);
                    byte r = Convert.ToByte(hex.Substring(4, 2), 16);
                    return Color.FromRgb(r, g, b);
                }
                catch { }
            }
            return def == default ? Colors.Black : def;
        }

        public static SolidColorBrush ToBrush(string hex, Color def = default)
            => new SolidColorBrush(FromHexString(hex, def));
    }
}
