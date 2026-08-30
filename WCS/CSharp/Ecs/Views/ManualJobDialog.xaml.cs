using System;
using System.Text;
using System.Windows;
using Ecs.Job;

namespace Ecs.Views
{
    /// <summary>
    /// 수동 작업 생성 다이얼로그. C++ CManualJob (IDD_MANAUL_JOB)에 대응.
    /// 창고/작업유형/작업번호/수량/출발·목적 로케이션/우선순위/바코드를 입력해
    /// JobItem을 생성하고 EcsDoc.AddJob으로 투입한다.
    /// </summary>
    public partial class ManualJobDialog : Window
    {
        private readonly EcsDoc _doc;

        public ManualJobDialog(EcsDoc doc)
        {
            InitializeComponent();
            _doc = doc;
            _doc.Jobs.JobAdded   += (s, j) => Dispatcher.Invoke(RefreshJobs);
            _doc.Jobs.JobRemoved += (s, j) => Dispatcher.Invoke(RefreshJobs);
            Loaded += (s, e) => RefreshJobs();
        }

        private static JobType MapJobType(int idx) => idx switch
        {
            0 => JobType.Store,
            1 => JobType.Retrieve,
            2 => JobType.RackToRack,
            3 => JobType.HsToHs,
            _ => JobType.Store,
        };

        private void BtnAdd_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(TxtLugg.Text, out int lugg) || lugg <= 0)
            {
                MessageBox.Show("작업번호를 올바르게 입력하세요.", "수동 작업"); return;
            }
            if (!int.TryParse(TxtCnt.Text, out int cnt) || cnt <= 0) cnt = 1;

            int.TryParse((CbWh.Text ?? "10"), out int wh);
            int.TryParse(TxtPriority.Text, out int pri);
            int.TryParse(TxtSrcSta.Text, out int srcSta);
            int.TryParse(TxtSrcRow.Text, out int srcRow);
            int.TryParse(TxtSrcCol.Text, out int srcCol);
            int.TryParse(TxtDstSta.Text, out int dstSta);
            int.TryParse(TxtDstRow.Text, out int dstRow);
            int.TryParse(TxtDstCol.Text, out int dstCol);
            var jt = MapJobType(CbJobType.SelectedIndex);

            int added = 0, failed = 0;
            for (int i = 0; i < cnt; i++)
            {
                var job = new JobItem
                {
                    LuggNum      = lugg + i,
                    JobType      = jt,
                    Priority     = pri,
                    SrcWarehouse = wh,  SrcStation = srcSta, SrcRow = srcRow, SrcCol = srcCol,
                    DstWarehouse = wh,  DstStation = dstSta, DstRow = dstRow, DstCol = dstCol,
                };
                job.Tray.Barcode = TxtBarcode.Text ?? string.Empty;

                if (_doc.AddJob(job)) added++;
                else failed++;
            }

            // 다음 입력을 위해 작업번호 자동 증가
            TxtLugg.Text = (lugg + cnt).ToString();
            if (failed > 0)
                MessageBox.Show($"{added}건 추가, {failed}건 실패(중복 작업번호).", "수동 작업");
        }

        private void BtnClear_Click(object sender, RoutedEventArgs e)
        {
            TxtLugg.Text = "1001"; TxtCnt.Text = "1"; TxtPriority.Text = "0"; TxtBarcode.Text = "";
            TxtSrcSta.Text = "1"; TxtSrcRow.Text = "0"; TxtSrcCol.Text = "0";
            TxtDstSta.Text = "1"; TxtDstRow.Text = "0"; TxtDstCol.Text = "0";
            CbWh.SelectedIndex = 0; CbJobType.SelectedIndex = 0;
        }

        private void RefreshJobs()
        {
            var all = _doc.Jobs.GetAll();
            string db = _doc.Db?.IsConnected == true ? "DB:연결(JOB_MST INSERT)" : "DB:미연결(인메모리)";
            TxtCount.Text = $"작업 수: {all.Count}   |   {db}";
            var sb = new StringBuilder();
            foreach (var j in all)
            {
                string typ = string.IsNullOrEmpty(j.JobTypeName) ? j.JobType.ToString() : j.JobTypeName;
                string sta = string.IsNullOrEmpty(j.StatusName)  ? j.Status.ToString()  : j.StatusName;
                sb.AppendLine($"• [{j.LuggNum}] {typ} / {sta} / 출발 St{j.SrcStation} R{j.SrcRow}C{j.SrcCol} → 목적 St{j.DstStation} R{j.DstRow}C{j.DstCol}");
            }
            TxtJobs.Text = sb.Length > 0 ? sb.ToString() : "(대기 작업 없음)";
        }

        private void BtnClose_Click(object sender, RoutedEventArgs e) => Close();
    }
}
