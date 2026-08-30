using System;
using System.Collections.Generic;
using System.Data;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace Ecs.Views
{
    /// <summary>메뉴 조회창 공통 기반: 상단 필터/버튼 + DataGrid. C++ CViewXxxDlg 스타일.</summary>
    public abstract class MenuGridDialogBase : Window
    {
        protected readonly EcsDoc _doc;
        protected readonly DataGrid Grid = new()
        {
            AutoGenerateColumns = true,
            IsReadOnly = true,
            CanUserAddRows = false,
            SelectionMode = DataGridSelectionMode.Single,
            SelectionUnit = DataGridSelectionUnit.FullRow,
            HeadersVisibility = DataGridHeadersVisibility.Column,
            GridLinesVisibility = DataGridGridLinesVisibility.All,
            AlternatingRowBackground = new SolidColorBrush(Color.FromRgb(0xF4, 0xF7, 0xFB)),
            FontSize = 12
        };
        protected readonly WrapPanel TopBar = new() { Margin = new Thickness(8, 8, 8, 4) };
        protected readonly TextBlock Status = new() { Margin = new Thickness(8, 4, 8, 6), Foreground = Brushes.DimGray };
        protected readonly Dictionary<string, string> Headers = new(StringComparer.OrdinalIgnoreCase);

        protected MenuGridDialogBase(EcsDoc doc, string title, double w = 780, double h = 520)
        {
            _doc = doc;
            Title = title;
            Width = w; Height = h;
            WindowStartupLocation = WindowStartupLocation.CenterOwner;
            Background = new SolidColorBrush(Color.FromRgb(0xF0, 0xF0, 0xF0));
            FontFamily = new FontFamily("맑은 고딕");
            LoadIcon(this);

            Grid.AutoGeneratingColumn += (s, e) =>
            {
                if (Headers.TryGetValue(e.PropertyName, out var kor)) e.Column.Header = kor;
                else e.Cancel = false;
            };

            var dock = new DockPanel();
            DockPanel.SetDock(TopBar, Dock.Top);
            DockPanel.SetDock(Status, Dock.Bottom);
            dock.Children.Add(TopBar);
            dock.Children.Add(Status);
            Grid.Margin = new Thickness(8, 0, 8, 0);
            dock.Children.Add(Grid);
            Content = dock;
        }

        internal static void LoadIcon(Window w)
        {
            try
            {
                string ico = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "app.ico");
                if (System.IO.File.Exists(ico))
                    w.Icon = System.Windows.Media.Imaging.BitmapFrame.Create(new Uri(ico));
            }
            catch { }
        }

        protected TextBox AddFilter(string label, double width = 110)
        {
            TopBar.Children.Add(new TextBlock
            { Text = label, VerticalAlignment = VerticalAlignment.Center, Margin = new Thickness(6, 0, 4, 0) });
            var tb = new TextBox { Width = width, Height = 24, VerticalContentAlignment = VerticalAlignment.Center };
            TopBar.Children.Add(tb);
            return tb;
        }

        protected ComboBox AddCombo(string label, string[] items, double width = 110)
        {
            TopBar.Children.Add(new TextBlock
            { Text = label, VerticalAlignment = VerticalAlignment.Center, Margin = new Thickness(6, 0, 4, 0) });
            var cb = new ComboBox { Width = width, Height = 24 };
            foreach (var it in items) cb.Items.Add(it);
            cb.SelectedIndex = 0;
            TopBar.Children.Add(cb);
            return cb;
        }

        protected Button AddButton(string text, Action onClick, double width = 78)
        {
            var b = new Button { Content = text, Width = width, Height = 26, Margin = new Thickness(6, 0, 0, 0) };
            b.Click += (s, e) => { try { onClick(); } catch (Exception ex) { MessageBox.Show(ex.Message); } };
            TopBar.Children.Add(b);
            return b;
        }

        protected void Load(DataTable dt)
        {
            Grid.ItemsSource = dt.DefaultView;
            Status.Text = $"{dt.Rows.Count} 건";
        }

        protected DataRowView? Selected => Grid.SelectedItem as DataRowView;
        protected bool Confirm(string msg)
            => MessageBox.Show(this, msg, Title, MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.Yes;
    }

    // ── 1) 통신 연결 정의 (C++ CEqpSuspendDlg) : eqp_mst ─────────────
    public class EqpSuspendDialog : MenuGridDialogBase
    {
        public EqpSuspendDialog(EcsDoc doc) : base(doc, "통신 연결 정의")
        {
            Headers["wh_typ"] = "창고"; Headers["eqp_typ"] = "설비유형"; Headers["plc_no"] = "PLC";
            Headers["plc_ip"] = "IP"; Headers["plc_port_from"] = "포트(From)"; Headers["plc_port_to"] = "포트(To)";
            Headers["use_yn"] = "사용"; Headers["connected_yn"] = "연결"; Headers["retry_yn"] = "재시도";
            Headers["sock_timeout"] = "타임아웃"; Headers["remarks"] = "비고";

            AddButton("검색", Query);
            AddButton("사용", () => SetUse("Y"));
            AddButton("사용안함", () => SetUse("N"));
            AddButton("닫기", Close);
            Query();
        }

        private void Query()
            => Load(_doc.Db?.SelectTable(
                "SELECT wh_typ, eqp_typ, plc_no, plc_ip, plc_port_from, plc_port_to, " +
                "use_yn, connected_yn, retry_yn, sock_timeout, remarks FROM eqp_mst " +
                "ORDER BY wh_typ, eqp_typ, plc_no") ?? new DataTable());

        private void SetUse(string yn)
        {
            var r = Selected;
            if (r == null) { MessageBox.Show(this, "행을 선택하세요."); return; }
            if (!Confirm($"PLC {r["plc_no"]} 사용여부를 '{yn}'(으)로 변경할까요?")) return;
            bool ok = _doc.Db != null && _doc.Db.ExecNonQuery(
                "UPDATE eqp_mst SET use_yn=@yn, upd_dt=GETDATE() WHERE wh_typ=@w AND eqp_typ=@e AND plc_no=@p",
                ("@yn", yn), ("@w", r["wh_typ"]), ("@e", r["eqp_typ"]), ("@p", r["plc_no"]));
            if (!ok) MessageBox.Show(this, "변경 실패: " + (_doc.Db?.LastError ?? ""));
            Query();
        }
    }

    // ── 2) 로그 삭제 설정 (C++ CConfigLogDelete) : del_his_setting ──
    public class ConfigLogDeleteDialog : MenuGridDialogBase
    {
        public ConfigLogDeleteDialog(EcsDoc doc) : base(doc, "로그 삭제 설정", 620, 460)
        {
            Headers["table_name"] = "테이블"; Headers["cycle"] = "보관주기"; Headers["unit"] = "단위";
            Headers["wh_typ"] = "창고"; Headers["key_column"] = "기준컬럼";

            var edt = AddFilter("보관주기", 70);
            AddButton("갱신", () =>
            {
                var r = Selected;
                if (r == null) { MessageBox.Show(this, "테이블 행을 선택하세요."); return; }
                if (!int.TryParse(edt.Text, out int cyc)) { MessageBox.Show(this, "보관주기를 숫자로 입력하세요."); return; }
                if (!Confirm($"[{r["table_name"]}] 보관주기를 {cyc}로 변경할까요?")) return;
                bool ok = _doc.Db != null && _doc.Db.ExecNonQuery(
                    "UPDATE del_his_setting SET cycle=@c WHERE table_name=@t",
                    ("@c", cyc.ToString()), ("@t", r["table_name"]));
                if (!ok) MessageBox.Show(this, "변경 실패: " + (_doc.Db?.LastError ?? ""));
                Query();
            });
            AddButton("닫기", Close);
            Grid.SelectionChanged += (s, e) => { if (Selected != null) edt.Text = Selected["cycle"]?.ToString(); };
            Query();
        }

        private void Query()
            => Load(_doc.Db?.SelectTable(
                "SELECT table_name, cycle, unit, wh_typ, key_column FROM del_his_setting ORDER BY table_name")
                ?? new DataTable());
    }

    // ── 3) 사용자 (C++ CUserUserDlg) : user_mst ────────────────────
    public class UserDialog : MenuGridDialogBase
    {
        public UserDialog(EcsDoc doc) : base(doc, "사용자", 700, 460)
        {
            Headers["user_id"] = "사용자ID"; Headers["user_nm"] = "이름"; Headers["grp_cd"] = "그룹";
            Headers["remarks"] = "비고"; Headers["upd_dt"] = "수정일시";

            var f = AddFilter("검색(ID/이름)", 130);
            AddButton("검색", () => Query(f.Text));
            AddButton("삭제", () =>
            {
                var r = Selected;
                if (r == null) { MessageBox.Show(this, "사용자를 선택하세요."); return; }
                if (!Confirm($"사용자 [{r["user_id"]}]를 삭제할까요?")) return;
                bool ok = _doc.Db != null && _doc.Db.ExecNonQuery(
                    "DELETE FROM user_mst WHERE user_id=@id", ("@id", r["user_id"]));
                if (!ok) MessageBox.Show(this, "삭제 실패: " + (_doc.Db?.LastError ?? ""));
                Query(f.Text);
            });
            AddButton("닫기", Close);
            Query("");
        }

        private void Query(string kw)
        {
            string sql = "SELECT user_id, user_nm, grp_cd, remarks, upd_dt FROM user_mst";
            if (!string.IsNullOrWhiteSpace(kw))
                sql += " WHERE user_id LIKE @k OR user_nm LIKE @k";
            sql += " ORDER BY user_id";
            Load(string.IsNullOrWhiteSpace(kw)
                ? _doc.Db?.SelectTable(sql) ?? new DataTable()
                : _doc.Db?.SelectTable(sql, ("@k", "%" + kw + "%")) ?? new DataTable());
        }
    }

    // ── 4) 공 PLT 작업 (C++ CViewHostEmptyPltDlg) : host_empty_plt ──
    public class HostEmptyPltDialog : MenuGridDialogBase
    {
        public HostEmptyPltDialog(EcsDoc doc) : base(doc, "공 PLT 작업", 760, 480)
        {
            Headers["wh_typ"] = "창고"; Headers["kind"] = "종류"; Headers["stn"] = "STN";
            Headers["status"] = "상태"; Headers["lugg_no"] = "화물번호"; Headers["ins_dt"] = "등록일시";

            var f = AddFilter("화물번호", 110);
            AddButton("검색", () => Query(f.Text));
            AddButton("삭제", () =>
            {
                var r = Selected;
                if (r == null) { MessageBox.Show(this, "행을 선택하세요."); return; }
                if (!Confirm($"화물번호 [{r["lugg_no"]}] 공PLT 항목을 삭제할까요?")) return;
                bool ok = _doc.Db != null && _doc.Db.ExecNonQuery(
                    "DELETE FROM host_empty_plt WHERE wh_typ=@w AND lugg_no=@l",
                    ("@w", r["wh_typ"]), ("@l", r["lugg_no"]));
                if (!ok) MessageBox.Show(this, "삭제 실패: " + (_doc.Db?.LastError ?? ""));
                Query(f.Text);
            });
            AddButton("닫기", Close);
            Query("");
        }

        private void Query(string lugg)
        {
            string sql = "SELECT wh_typ, kind, stn, status, lugg_no, ins_dt FROM host_empty_plt";
            if (!string.IsNullOrWhiteSpace(lugg)) sql += " WHERE lugg_no LIKE @l";
            sql += " ORDER BY ins_dt DESC";
            Load(string.IsNullOrWhiteSpace(lugg)
                ? _doc.Db?.SelectTable(sql) ?? new DataTable()
                : _doc.Db?.SelectTable(sql, ("@l", "%" + lugg + "%")) ?? new DataTable());
        }
    }

    // ── 5) 범례 (C++ CConfigStatus) : 색상 안내(정적) ───────────────
    public class LegendDialog : Window
    {
        private static readonly (string label, Color color)[] Items =
        {
            ("정상 / 대기",        Color.FromRgb(0x66, 0xCC, 0x66)),
            ("입고 / 반입 중",     Color.FromRgb(0xF0, 0x90, 0x30)),
            ("출고 / 반출 중",     Color.FromRgb(0x30, 0x80, 0xF0)),
            ("이동 / 작업 중",     Color.FromRgb(0xF0, 0xE0, 0x40)),
            ("준비 완료",          Color.FromRgb(0x00, 0xC8, 0x00)),
            ("보류(Suspend)",      Color.FromRgb(0x90, 0x90, 0x90)),
            ("에러(Error)",        Color.FromRgb(0xE0, 0x30, 0x30)),
            ("수동(Manual)",       Color.FromRgb(0xFF, 0xFF, 0x96)),
            ("통신 끊김",          Color.FromRgb(0x80, 0x20, 0x20)),
        };

        public LegendDialog(EcsDoc doc)
        {
            Title = "범례";
            SizeToContent = SizeToContent.WidthAndHeight;
            ResizeMode = ResizeMode.NoResize;
            WindowStartupLocation = WindowStartupLocation.CenterOwner;
            Background = new SolidColorBrush(Color.FromRgb(0xF0, 0xF0, 0xF0));
            FontFamily = new FontFamily("맑은 고딕");
            FontSize = 13;
            MenuGridDialogBase.LoadIcon(this);

            var grid = new Grid { Margin = new Thickness(18) };
            grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(40) });
            grid.ColumnDefinitions.Add(new ColumnDefinition { Width = new GridLength(180) });
            for (int i = 0; i < Items.Length; i++) grid.RowDefinitions.Add(new RowDefinition { Height = new GridLength(30) });

            for (int i = 0; i < Items.Length; i++)
            {
                var sw = new Border
                {
                    Background = new SolidColorBrush(Items[i].color),
                    BorderBrush = Brushes.Gray, BorderThickness = new Thickness(1),
                    Width = 26, Height = 18, Margin = new Thickness(0, 0, 8, 0),
                    VerticalAlignment = VerticalAlignment.Center
                };
                Grid.SetRow(sw, i); Grid.SetColumn(sw, 0);
                grid.Children.Add(sw);
                var tb = new TextBlock { Text = Items[i].label, VerticalAlignment = VerticalAlignment.Center };
                Grid.SetRow(tb, i); Grid.SetColumn(tb, 1);
                grid.Children.Add(tb);
            }
            Content = grid;
        }
    }
}
