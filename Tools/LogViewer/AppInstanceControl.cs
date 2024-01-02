using System.Diagnostics;
using System.IO;
using System.Windows;

namespace LogViewer
{
    public class AppInstanceControl
    {
        public AppInstanceControl()
        {
            CheckIsRunning();
            CreateGlobalConfig();
        }

        /// <summary>
        /// Creates pop up asking should we ignore that
        /// application is already running </summary>
        /// <returns> False if we shouldn't ignore that application is running </returns>
        public bool CreatePopUpIgnore()
        {
            string title = "An instance of application is already running!";
            string message = "An instance of application is already running. Are you sure that you want to open this application?";

            MessageBoxButton buttons = MessageBoxButton.YesNo;
            MessageBoxImage icon = MessageBoxImage.Warning;
            MessageBoxResult result = MessageBox.Show(message,
                title,
                buttons,
                icon,
                MessageBoxResult.Yes);

            if (result == MessageBoxResult.No)
                return false;
            else
                return true;
        }

        public void SetPathGlobally(string path)
        {

        }

        public bool IsRunning { get => _IsRunning; }

        // Private ------------------------------------------------------------

        private void CheckIsRunning()
        {
            if (Process.GetProcessesByName(_AppName).Count() > 1)
                _IsRunning = true;
        }

        private void CreateGlobalConfig()
        {
            _GlobalControlPath = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);

            // Check for and create necessary files and dirs
            _GlobalControlPath += "\\ImIllegalBee\\";
            if (!Directory.Exists(_GlobalControlPath))
                Directory.CreateDirectory(_GlobalControlPath);

            _GlobalControlPath += _AppName;
            if (!Directory.Exists(_GlobalControlPath))
                Directory.CreateDirectory(_GlobalControlPath);

            _GlobalControlPath += "\\Session.Ctrl";
            using (FileStream fs = new FileStream(_GlobalControlPath, FileMode.OpenOrCreate))
                fs.Close();
        }

        private bool _IsRunning = false;
        private const string _AppName = "LogViewer";
        private string _GlobalControlPath = "";
    }
}
