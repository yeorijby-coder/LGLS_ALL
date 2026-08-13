using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Controls;
using Ecs.Job;

namespace Ecs.Views
{
    public partial class JobListView : UserControl
    {
        private EcsDoc? _doc;
        private readonly ObservableCollection<JobRowVm> _rows = new();
        private int _filterStatus = -1;

        public JobListView()
        {
            InitializeComponent();
            JobListCtrl.ItemsSource = _rows;
        }

        public void SetDoc(EcsDoc doc)
        {
            _doc = doc;
            doc.Jobs.JobAdded         += (s, j) => Dispatcher.Invoke(Refresh);
            doc.Jobs.JobStatusChanged += (s, j) => Dispatcher.Invoke(Refresh);
            doc.Jobs.JobRemoved       += (s, j) => Dispatcher.Invoke(Refresh);
            Refresh();
        }

        private void Refresh()
        {
            _rows.Clear();
            if (_doc == null) return;
            foreach (var job in _doc.Jobs.GetAll())
            {
                if (_filterStatus >= 0 && (int)job.Status != _filterStatus) continue;
                _rows.Add(new JobRowVm(job));
            }
        }

        private void CbStatus_Changed(object sender, SelectionChangedEventArgs e)
        {
            var sel = CbStatus.SelectedItem as ComboBoxItem;
            _filterStatus = sel != null ? int.Parse(sel.Tag?.ToString() ?? "-1") : -1;
            Refresh();
        }

        private void BtnCancel_Click(object sender, RoutedEventArgs e)
        {
            if (_doc == null) return;
            if (JobListCtrl.SelectedItem is JobRowVm row)
            {
                _doc.Jobs.Cancel(row.LuggNum);
                Refresh();
            }
        }

        private void BtnRefresh_Click(object sender, RoutedEventArgs e) => Refresh();
    }

    public class JobRowVm
    {
        public int    LuggNum       { get; }
        public string JobTypeStr    { get; }
        public string StatusStr     { get; }
        public string SrcStr        { get; }
        public string DstStr        { get; }
        public int    ScIndex       { get; }
        public string CreateTimeStr { get; }

        public JobRowVm(JobItem job)
        {
            LuggNum       = job.LuggNum;
            JobTypeStr    = job.JobType.ToString();
            StatusStr     = job.Status.ToString();
            SrcStr        = $"ST{job.SrcStation}";
            DstStr        = $"ST{job.DstStation} R{job.DstRow}C{job.DstCol}";
            ScIndex       = job.ScIndex;
            CreateTimeStr = job.CreateTime.ToString("MM-dd HH:mm:ss");
        }
    }
}
