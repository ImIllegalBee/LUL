using System.Configuration;
using System.Data;
using System.Windows;

namespace LogViewer
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public AppInstanceControl InstanceControl { get => _InstanceCtrl; }

        // Private ------------------------------------------------------------

        private void StartApp(object sender, StartupEventArgs e)
        {
            if (!_InstanceCtrl.IsRunning)
            {
                _MainWindow = new MainWindow();
                _MainWindow.Show();

                return;
            }

            // Set new path globally
            // Program was started from terminal,
            // passed '/Path' arg.
            // We assume that, because there is already an instance of program
            // running and it was started from terminal, user doesn't want
            // to start a new instance of app.
            // We just shutdown
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

        private void ExitApp(object sender, ExitEventArgs e)
        {
            if (_MainWindow != null)
            {
                _MainWindow.Close();
            }
        }

        private AppInstanceControl _InstanceCtrl = new AppInstanceControl();
        private MainWindow? _MainWindow = null;
    }
}
