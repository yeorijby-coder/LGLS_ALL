using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Ecs.Equipment;

namespace Ecs.Views
{
    /// <summary>
    /// 우측 패널에 표시되는 장비별 상태 요약 카드.
    /// C++의 MainFrm 우측 상태 패널에 대응.
    /// </summary>
    public partial class EquipmentStatusPanel : UserControl
    {
        private EcsDoc? _doc;

        public EquipmentStatusPanel()
        {
            InitializeComponent();
        }

        public void SetDoc(EcsDoc doc)
        {
            _doc = doc;
            BuildPanel();

            doc.CvStatusChanged  += (s, cv)  => Dispatcher.Invoke(RefreshAll);
            doc.ScStatusChanged  += (s, sc)  => Dispatcher.Invoke(RefreshAll);
            doc.RtvStatusChanged += (s, rtv) => Dispatcher.Invoke(RefreshAll);
        }

        private void BuildPanel()
        {
            if (_doc == null) return;
            PanelRoot.Children.Clear();

            AddSection("컨베이어 (CV)");
            foreach (var eq in _doc.CvArray)
                PanelRoot.Children.Add(CreateEquipCard(eq));

            AddSection("스태커 크레인 (SC)");
            foreach (var eq in _doc.ScArray)
                PanelRoot.Children.Add(CreateEquipCard(eq));

            AddSection("RTV");
            foreach (var eq in _doc.RtvArray)
                PanelRoot.Children.Add(CreateEquipCard(eq));

            AddSection("BCR");
            foreach (var eq in _doc.BcrArray)
                PanelRoot.Children.Add(CreateEquipCard(eq));
        }

        private void AddSection(string title)
        {
            var tb = new TextBlock
            {
                Text = title,
                FontWeight = FontWeights.Bold,
                Background = Brushes.DarkSlateGray,
                Foreground = Brushes.White,
                Padding = new Thickness(4, 2, 4, 2),
                Margin = new Thickness(0, 6, 0, 2),
            };
            PanelRoot.Children.Add(tb);
        }

        private UIElement CreateEquipCard(Equipment.Equipment eq)
        {
            Brush bg = eq.State switch
            {
                EquipmentState.Fault   => Brushes.LightCoral,
                EquipmentState.PM      => Brushes.LightYellow,
                EquipmentState.Offline => Brushes.LightGray,
                _                     => Brushes.LightGreen,
            };

            var border = new Border
            {
                BorderBrush     = Brushes.DarkGray,
                BorderThickness = new Thickness(1),
                CornerRadius    = new CornerRadius(3),
                Margin          = new Thickness(2),
                Background      = bg,
                Tag             = eq,
            };

            var sp = new StackPanel { Margin = new Thickness(4, 2, 4, 2) };
            sp.Children.Add(new TextBlock
            {
                Text       = eq.DeviceName,
                FontWeight = FontWeights.Bold,
                FontSize   = 11,
            });
            sp.Children.Add(new TextBlock
            {
                Text     = $"상태: {eq.State}",
                FontSize = 10,
            });

            // SC일 경우 현재 위치 표시
            if (eq is Sc sc)
            {
                sp.Children.Add(new TextBlock
                {
                    Text     = $"위치: R{sc.Info.Row} C{sc.Info.Col} / {sc.Info.ScState}",
                    FontSize = 10,
                });
            }

            border.Child = sp;

            // 더블클릭으로 상세 다이얼로그 열기
            border.MouseLeftButtonDown += (s, e) =>
            {
                if (e.ClickCount == 2)
                    OpenEquipDialog(eq);
            };

            return border;
        }

        private void OpenEquipDialog(Equipment.Equipment eq)
        {
            // 향후 장비별 상세 다이얼로그 구현
            MessageBox.Show($"장비 정보\n이름: {eq.DeviceName}\n상태: {eq.State}\n연결: {(eq.Running ? "실행중" : "중지")}",
                            "장비 상세", MessageBoxButton.OK, MessageBoxImage.Information);
        }

        private void RefreshAll()
        {
            BuildPanel();
        }
    }
}
