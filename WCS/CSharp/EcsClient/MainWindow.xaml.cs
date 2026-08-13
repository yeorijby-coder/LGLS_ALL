using System;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using DciLib;

namespace EcsClient
{
    /// <summary>
    /// C++의 EcsClient에 대응하는 원격 모니터링 클라이언트.
    /// ECS 서버의 UDP 멀티캐스트를 수신해 레이아웃을 표시한다.
    /// </summary>
    public partial class MainWindow : Window
    {
        private UdpClient? _udp;
        private CancellationTokenSource _cts = new();
        private readonly DciMaster _master = new();
        private DrawingVisual? _visual;

        public MainWindow()
        {
            InitializeComponent();
            Closing += (s, e) => Disconnect();
        }

        private async void BtnConnect_Click(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(TxtPort.Text, out int port)) return;

            try
            {
                _udp = new UdpClient(port);
                _udp.JoinMulticastGroup(System.Net.IPAddress.Parse(TxtServer.Text));
                _cts = new CancellationTokenSource();

                TxtConnStatus.Text = $"연결됨 - {TxtServer.Text}:{port}";
                await Task.Run(() => ReceiveLoop(_cts.Token));
            }
            catch (Exception ex)
            {
                MessageBox.Show($"연결 오류: {ex.Message}", "오류");
            }
        }

        private void BtnDisconnect_Click(object sender, RoutedEventArgs e) => Disconnect();

        private void Disconnect()
        {
            _cts.Cancel();
            _udp?.Close();
            _udp = null;
            Dispatcher.Invoke(() => TxtConnStatus.Text = "연결 안됨");
        }

        private async Task ReceiveLoop(CancellationToken token)
        {
            while (!token.IsCancellationRequested && _udp != null)
            {
                try
                {
                    var result = await _udp.ReceiveAsync();
                    ProcessPacket(result.Buffer);
                }
                catch { break; }
            }
        }

        private void ProcessPacket(byte[] data)
        {
            if (data.Length < 2) return;
            // 패킷 타입별 처리 (Ecs/Equipment/Cv.cs의 MulticastInfo 패킷 형식과 동일)
            Dispatcher.Invoke(Redraw);
        }

        private void LayoutCanvas_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            _master.Resize(e.NewSize.Width, e.NewSize.Height);
            Redraw();
        }

        private void Redraw()
        {
            LayoutCanvas.Children.Clear();
            _visual = new DrawingVisual();
            using (var dc = _visual.RenderOpen())
            {
                dc.DrawRectangle(new SolidColorBrush(Color.FromRgb(26, 26, 46)), null,
                    new Rect(0, 0, LayoutCanvas.ActualWidth, LayoutCanvas.ActualHeight));
                _master.Draw(dc);
            }
            // VisualHost 추가는 EcsLayoutView와 동일한 패턴 사용
        }
    }
}
