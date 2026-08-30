using System;
using System.Windows;
using System.Windows.Controls;
using Ecs.Equipment;

namespace Ecs.Views
{
    /// <summary>
    /// 수동 크레인(SC) 조작 다이얼로그. C++ CManualSc 대응.
    /// ScArray의 SC를 선택해 입고/출고/홈/취소 명령을 직접 발행한다.
    /// </summary>
    public partial class ManualScDialog : Window
    {
        private readonly EcsDoc _doc;

        public ManualScDialog(EcsDoc doc)
        {
            InitializeComponent();
            _doc = doc;
            for (int i = 0; i < _doc.ScArray.Count; i++)
            {
                var sc = _doc.ScArray[i];
                CbSc.Items.Add($"SC#{sc.Number} ({sc.DeviceName})");
            }
            if (CbSc.Items.Count > 0) CbSc.SelectedIndex = 0;
        }

        private Sc? Selected =>
            CbSc.SelectedIndex >= 0 && CbSc.SelectedIndex < _doc.ScArray.Count
                ? _doc.ScArray[CbSc.SelectedIndex] as Sc : null;

        private void CbSc_Changed(object sender, SelectionChangedEventArgs e)
        {
            var sc = Selected;
            TxtScState.Text = sc != null ? $"  상태: {sc.State}  연결:{(sc.IsConnected ? "O" : "X")}" : "  -";
        }

        private int I(string s) => int.TryParse(s, out int v) ? v : 0;

        private async void BtnStore_Click(object sender, RoutedEventArgs e)
        {
            var sc = Selected; if (sc == null) return;
            try { await sc.Store(I(TxtLugg.Text), I(TxtRow.Text), I(TxtCol.Text), I(TxtHs.Text));
                  TxtResult.Text = $"입고 명령 발행: SC#{sc.Number} 화물{TxtLugg.Text} R{TxtRow.Text}C{TxtCol.Text}"; }
            catch (Exception ex) { TxtResult.Text = "입고 명령 실패: " + ex.Message; }
        }

        private async void BtnRetrieve_Click(object sender, RoutedEventArgs e)
        {
            var sc = Selected; if (sc == null) return;
            try { await sc.Retrieve(I(TxtLugg.Text), I(TxtRow.Text), I(TxtCol.Text), I(TxtHs.Text));
                  TxtResult.Text = $"출고 명령 발행: SC#{sc.Number} R{TxtRow.Text}C{TxtCol.Text}"; }
            catch (Exception ex) { TxtResult.Text = "출고 명령 실패: " + ex.Message; }
        }

        private async void BtnHome_Click(object sender, RoutedEventArgs e)
        {
            var sc = Selected; if (sc == null) return;
            try { await sc.Home(); TxtResult.Text = $"홈 명령 발행: SC#{sc.Number}"; }
            catch (Exception ex) { TxtResult.Text = "홈 명령 실패: " + ex.Message; }
        }

        private async void BtnCancel_Click(object sender, RoutedEventArgs e)
        {
            var sc = Selected; if (sc == null) return;
            try { await sc.Cancel(); TxtResult.Text = $"취소 명령 발행: SC#{sc.Number}"; }
            catch (Exception ex) { TxtResult.Text = "취소 명령 실패: " + ex.Message; }
        }

        private void BtnClose_Click(object sender, RoutedEventArgs e) => Close();
    }
}
