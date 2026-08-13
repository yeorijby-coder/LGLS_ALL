using System.Windows;

namespace Ecs
{
    public partial class App : Application
    {
        public static EcsDoc Doc { get; private set; } = new EcsDoc();

        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);
            Doc.Initialize();
        }

        protected override void OnExit(ExitEventArgs e)
        {
            Doc.Shutdown();
            base.OnExit(e);
        }
    }
}
