using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;

namespace Ecs.Views
{
    /// <summary>
    /// C++ CvSkinDlg/ScSkinDlg/RtvSkinDlg 공통 기반.
    /// C++ .rc(DIALOGEX)의 절대 좌표(DLU)를 그대로 재현하기 위해 Canvas + DLU→px 변환 사용.
    /// MS Shell Dlg 8pt 기준 1 DLU ≈ (1.5px, 1.625px). 확대(ZOOM) 버튼으로 차량패널을 펼침.
    /// </summary>
    public abstract class SkinDialogBase : Window
    {
        protected readonly EcsDoc _doc;
        protected readonly int    _no;
        protected readonly string _whTyp = "10";

        protected const double DX = 1.5;    // px per horizontal DLU
        protected const double DY = 1.625;  // px per vertical DLU

        private DispatcherTimer? _timer;
        private readonly Dictionary<string, TextBlock> _fields = new(StringComparer.OrdinalIgnoreCase);
        private readonly Dictionary<string, Border>    _leds   = new(StringComparer.OrdinalIgnoreCase);

        protected readonly Canvas Main = new();
        private   readonly StackPanel _extra = new()
            { Margin = new Thickness(8, 6, 8, 6), Visibility = Visibility.Collapsed };
        private   bool _extraBuilt;
        private   Button? _zoomBtn;

        protected static readonly Brush LedOn   = new SolidColorBrush(Color.FromRgb(0x00, 0xC8, 0x00));
        protected static readonly Brush LedOff  = new SolidColorBrush(Color.FromRgb(0x5A, 0x5A, 0x5A));
        protected static readonly Brush FieldBg = Brushes.White;
        private   static readonly Brush GroupBd = new SolidColorBrush(Color.FromRgb(0xA0, 0xA0, 0xA0));
        private   static readonly Brush LblFg   = Brushes.Black;

        protected SkinDialogBase(EcsDoc doc, int no, string title)
        {
            _doc = doc; _no = no;
            Title = title;
            WindowStartupLocation = WindowStartupLocation.CenterOwner;
            SizeToContent = SizeToContent.WidthAndHeight;
            ResizeMode = ResizeMode.CanMinimize;
            Background = new SolidColorBrush(Color.FromRgb(0xF0, 0xF0, 0xF0));
            FontFamily = new FontFamily("맑은 고딕");
            FontSize = 11;

            Main.Background = new SolidColorBrush(Color.FromRgb(0xF0, 0xF0, 0xF0));
            var host = new StackPanel { Orientation = Orientation.Horizontal };
            host.Children.Add(Main);
            host.Children.Add(_extra);
            Content = host;

            try
            {
                string ico = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "app.ico");
                if (System.IO.File.Exists(ico))
                    Icon = System.Windows.Media.Imaging.BitmapFrame.Create(new Uri(ico));
            }
            catch { }
        }

        /// <summary>클라이언트 영역(DLU) 지정 — .rc DIALOGEX의 cx,cy.</summary>
        protected void SetClient(int cxDlu, int cyDlu)
        {
            Main.Width  = cxDlu * DX;
            Main.Height = cyDlu * DY;
        }

        // ── DLU 배치 헬퍼 ───────────────────────────────────────
        private static void Place(FrameworkElement el, int x, int y, int w, int h)
        {
            Canvas.SetLeft(el, x * DX);
            Canvas.SetTop(el, y * DY);
            el.Width  = w * DX;
            el.Height = h * DY;
        }

        /// <summary>GROUPBOX</summary>
        protected void Grp(string title, int x, int y, int w, int h, Canvas? c = null)
        {
            var g = new GroupBox
            {
                Header = title,
                BorderBrush = GroupBd,
                Foreground = new SolidColorBrush(Color.FromRgb(0x20, 0x40, 0x80)),
                FontSize = 11
            };
            Place(g, x, y, w, h);
            (c ?? Main).Children.Add(g);
        }

        /// <summary>RTEXT/CTEXT/LTEXT 라벨. align: 'r'/'c'/'l'</summary>
        protected void Lbl(string text, int x, int y, int w, int h, char align = 'r', Canvas? c = null)
        {
            var tb = new TextBlock
            {
                Text = text,
                Foreground = LblFg,
                TextWrapping = TextWrapping.NoWrap,
                VerticalAlignment = VerticalAlignment.Center,
                HorizontalAlignment = align == 'r' ? HorizontalAlignment.Right
                                    : align == 'c' ? HorizontalAlignment.Center : HorizontalAlignment.Left
            };
            var grid = new Grid();
            grid.Children.Add(tb);
            Place(grid, x, y, w, h);
            (c ?? Main).Children.Add(grid);
        }

        /// <summary>EDITTEXT(읽기필드). key로 값 갱신. status=true면 노란 배경.</summary>
        protected void Fld(string key, int x, int y, int w, int h, Canvas? c = null, bool center = false)
        {
            var val = new TextBlock
            {
                Text = "-",
                Foreground = Brushes.Black,
                VerticalAlignment = VerticalAlignment.Center,
                Margin = new Thickness(3, 0, 3, 0),
                TextTrimming = TextTrimming.CharacterEllipsis,
                TextAlignment = center ? TextAlignment.Center : TextAlignment.Left
            };
            var bd = new Border
            {
                Background = FieldBg,
                BorderBrush = GroupBd,
                BorderThickness = new Thickness(1),
                Child = val
            };
            Place(bd, x, y, w, h);
            (c ?? Main).Children.Add(bd);
            _fields[key] = val;
        }

        /// <summary>PUSHBUTTON</summary>
        protected Button Btn(string text, int x, int y, int w, int h, Action onClick, Canvas? c = null)
        {
            var b = new Button { Content = text, FontSize = 11 };
            Place(b, x, y, w, h);
            b.Click += (s, e) => { try { onClick(); } catch (Exception ex) { MessageBox.Show(ex.Message); } };
            (c ?? Main).Children.Add(b);
            return b;
        }

        /// <summary>체크박스</summary>
        protected CheckBox Chk(string text, int x, int y, int w, int h, Canvas? c = null)
        {
            var cb = new CheckBox { Content = text, VerticalContentAlignment = VerticalAlignment.Center, FontSize = 11 };
            Place(cb, x, y, w, h);
            (c ?? Main).Children.Add(cb);
            return cb;
        }

        // 상태표시 버튼(값 1이면 초록 배경) — C++ 스킨의 상태 컬러 버튼 대응
        private readonly Dictionary<string, Control> _stateCtrls = new(StringComparer.OrdinalIgnoreCase);
        protected Button StateBtn(string text, int x, int y, int w, int h, string key, Canvas? c = null)
        {
            var b = new Button { Content = text, FontSize = 11 };
            Place(b, x, y, w, h);
            (c ?? Main).Children.Add(b);
            _stateCtrls[key] = b;
            return b;
        }
        protected void SetState(string key, bool on)
        {
            if (_stateCtrls.TryGetValue(key, out var c))
                c.Background = on ? LedOn : SystemColors.ControlBrush;
        }

        /// <summary>확대(ZOOM) 버튼 등록 — 클릭 시 차량패널 펼침/접힘.</summary>
        protected void SetZoomButton(Button b) { _zoomBtn = b; }

        private void ToggleExpand()
        {
            bool show = _extra.Visibility != Visibility.Visible;
            _extra.Visibility = show ? Visibility.Visible : Visibility.Collapsed;
            if (_zoomBtn != null) _zoomBtn.Content = show ? "축소" : "확대";
        }

        // ── 차량패널(확대) : LED 그룹 ───────────────────────────
        protected Grid ExtraGroup(string header)
        {
            _extraBuilt = true;
            var grid = new Grid { Margin = new Thickness(6) };
            var box = new GroupBox
            {
                Header = header, BorderBrush = GroupBd, Margin = new Thickness(0, 0, 0, 8),
                Content = grid, Foreground = new SolidColorBrush(Color.FromRgb(0x20, 0x40, 0x80)), FontWeight = FontWeights.Bold
            };
            _extra.Children.Add(box);
            return grid;
        }

        protected void FillLeds(Grid grid, (string label, string key)[] items, int cols = 2)
        {
            for (int i = 0; i < cols; i++)
            {
                grid.ColumnDefinitions.Add(new ColumnDefinition { Width = GridLength.Auto });
                grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(110) });
            }
            int rows = (items.Length + cols - 1) / cols;
            for (int r = 0; r < rows; r++) grid.RowDefinitions.Add(new RowDefinition());
            for (int idx = 0; idx < items.Length; idx++)
            {
                int col = idx / rows, row = idx % rows;
                var led = new Border
                {
                    Width = 13, Height = 13, Background = LedOff, BorderBrush = GroupBd, BorderThickness = new Thickness(1),
                    Margin = new Thickness(6, 3, 4, 3), VerticalAlignment = VerticalAlignment.Center, CornerRadius = new CornerRadius(2)
                };
                Grid.SetRow(led, row); Grid.SetColumn(led, col * 2); grid.Children.Add(led);
                _leds[items[idx].key] = led;
                var lbl = new TextBlock { Text = items[idx].label, VerticalAlignment = VerticalAlignment.Center, Margin = new Thickness(2, 3, 8, 3) };
                Grid.SetRow(lbl, row); Grid.SetColumn(lbl, col * 2 + 1); grid.Children.Add(lbl);
            }
        }

        protected void ExtraField(Grid grid, string label, string key, int row)
        {
            // 확대 패널 하단의 값 필드용(위치/팔레트 등)
        }

        // ── 폴링 ────────────────────────────────────────────────
        protected void StartPolling()
        {
            if (!_extraBuilt) _extra.Visibility = Visibility.Collapsed;
            if (_zoomBtn != null) _zoomBtn.Click += (s, e) => ToggleExpand();
            Refresh();
            _timer = new DispatcherTimer { Interval = TimeSpan.FromSeconds(1) };
            _timer.Tick += (s, e) => { try { Refresh(); } catch { } };
            _timer.Start();
            Closed += (s, e) => _timer?.Stop();
        }

        protected abstract void Refresh();

        // ── 값/LED/포맷 ─────────────────────────────────────────
        protected void Set(string key, string text)
        {
            if (_fields.TryGetValue(key, out var tb)) tb.Text = string.IsNullOrEmpty(text) ? "-" : text;
        }
        protected void SetLed(string key, bool on)
        {
            if (_leds.TryGetValue(key, out var b)) b.Background = on ? LedOn : LedOff;
        }
        protected static string G(Dictionary<string, string>? r, string col)
            => r != null && r.TryGetValue(col, out var v) ? v : "";
        protected static bool On(Dictionary<string, string>? r, string col) => G(r, col) == "1";
        protected static string Dash(string v) => (string.IsNullOrEmpty(v) || v == "0" || v == "0000") ? "-" : v;
        protected static string OnOff(string v, string on, string off) => v == "1" ? on : off;
        protected static string Pick(string v, string def, params (string code, string name)[] map)
        {
            foreach (var m in map) if (m.code == v) return m.name;
            return def;
        }
        protected static string Trio(string a, string b, string c)
        {
            a = string.IsNullOrEmpty(a) ? "0" : a; b = string.IsNullOrEmpty(b) ? "0" : b; c = string.IsNullOrEmpty(c) ? "0" : c;
            return $"{a}-{b}-{c}";
        }
        protected bool Confirm(string msg)
            => MessageBox.Show(this, msg, Title, MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.Yes;
    }
}
