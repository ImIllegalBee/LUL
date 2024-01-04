using System.Windows;

namespace LogViewer
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public AppInstanceControl InstanceControl { get => _InstanceCtrl; }

        public Viewer? LogViewer { get => _Viewer; }

        // Private ------------------------------------------------------------

        private void StartApp(object sender, StartupEventArgs e)
        {
            if (_InstanceCtrl.IsRunning)
            {
                // Set new path globally
                // Program was started from terminal,
                // passed '/Path' arg.
                // We assume that, because there is already an instance of program
                // running and it was started from terminal, user doesn't want
                // to start a new instance of app.
                // We just update path globally and shutdown
                for (int i = 0; i < e.Args.Length; i++)
                {
                    if (e.Args[i] == "/Path" &&
                        i + 1 < e.Args.Length)
                    {
                        _InstanceCtrl.SetPathGlobally(e.Args[i + 1]);

                        this.Shutdown();
                        return;
                    }
                }

                if (!_InstanceCtrl.CreatePopUpIgnore())
                {
                    this.Shutdown();
                    return;
                }
            }

            for (int i = 0; i < e.Args.Length; i++)
            {
                if (e.Args[i] == "/Path" &&
                    i + 1 < e.Args.Length)
                {
                    _InstanceCtrl.SetPathGlobally(e.Args[i + 1]);
                }
            }

            _MainWindow = new MainWindow();
            _MainWindow.Show();

            _Viewer = new Viewer(_MainWindow);
        }

        private void ExitApp(object sender, ExitEventArgs e)
        {
            if (_Viewer != null)
                _Viewer.Dispose();

            if (_MainWindow != null)
                _MainWindow.Close();
        }

        private AppInstanceControl _InstanceCtrl = new AppInstanceControl();
        private Viewer? _Viewer = null;
        private MainWindow? _MainWindow = null;
    }
}
