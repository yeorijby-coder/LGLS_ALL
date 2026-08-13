using System;
using System.Windows;

namespace FTPServer
{
    public partial class MainWindow : Window
    {
        private EcsFtpServer? _server;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void BtnStart_Click(object sender, RoutedEventArgs e)
        {
            if (_server?.Running == true) return;

            if (!int.TryParse(TxtPort.Text, out int port) || port < 1 || port > 65535)
            {
                MessageBox.Show("올바른 포트를 입력하세요.");
                return;
            }

            _server = new EcsFtpServer
            {
                Port     = port,
                RootPath = TxtRoot.Text,
            };
            _server.Log += (s, msg) => Dispatcher.Invoke(() =>
            {
                LogBox.Items.Add($"[{DateTime.Now:HH:mm:ss}] {msg}");
                if (LogBox.Items.Count > 0)
                    LogBox.ScrollIntoView(LogBox.Items[LogBox.Items.Count - 1]);
            });

            _server.Start();
            TxtStatus.Text = $"실행 중 (포트 {port})";
            AddLog($"FTP 서버 시작: 포트 {port}, 루트 {TxtRoot.Text}");
        }

        private void BtnStop_Click(object sender, RoutedEventArgs e)
        {
            _server?.Stop();
            _server = null;
            TxtStatus.Text = "중지됨";
            AddLog("FTP 서버 중지");
        }

        private void BtnClearLog_Click(object sender, RoutedEventArgs e)
        {
            LogBox.Items.Clear();
        }

        private void AddLog(string msg)
        {
            LogBox.Items.Add($"[{DateTime.Now:HH:mm:ss}] {msg}");
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            _server?.Stop();
        }
    }
}
