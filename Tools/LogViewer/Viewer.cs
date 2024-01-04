using System.Diagnostics;
using System.IO;
using System.Windows.Media;

namespace LogViewer
{
    public struct LogLine
    {
        public LogLine(string l)
        {
            _line = l;
        }

        public LogLine(Brush b, string l)
        {
            _brush = b;
            _line = l;
        }

        static public bool IsEmpty(LogLine ll) => string.IsNullOrEmpty(ll.Line);

        static public LogLine Empty => new LogLine(string.Empty);

        public Brush Brush { get => _brush; set => _brush = value; }

        public string Line { get => _line; }

        // Private ------------------------------------------------------------

        private Brush _brush = Brushes.White;

        private string _line;
    }

    public class Viewer : IDisposable
    {
        public Viewer(MainWindow parentWindow)
        {
            _ParentWindow = parentWindow;

            StartViewerThread();
        }

        public void Dispose()
        {
            KillViewerThread();
        }

        // Private ------------------------------------------------------------
        private void StartViewerThread()
        {
            if (_ViewerThread != null)
            {
                Debug.WriteLine("ViewerThread was already initialized!");
                return;
            }

            _ViewerThreadStarted = true;
            _ViewerThread = new Thread(MainViewerLoop);
            _ViewerThread.Start();
        }

        private void MainViewerLoop()
        {
            Debug.WriteLine("ViewerThread has been initialized");

            string NewPath = "";

            while (_ViewerThreadStarted)
            {
                Thread.Sleep(_ThreadSleep);

                NewPath = CheckAndUpdatePath();
                List<LogLine> FileOutput = CheckFileOutput();

                if (!string.IsNullOrEmpty(NewPath))
                {
                    _PathToFile = NewPath;
                    _LinesRead = 0;

                    try
                    {
                        _ParentWindow.Dispatcher.InvokeAsync(() =>
                        {
                            _ParentWindow.SetPathTextFields(NewPath);
                            NewPath = "";
                        });
                    }
                    catch
                    {
                        // Ignore everything
                    }
                }

                if (FileOutput.Count > 0)
                {
                    try
                    {
                        _ParentWindow.Dispatcher.InvokeAsync(() =>
                        {
                            _ParentWindow.AddChunkOfLines(FileOutput);
                            FileOutput.Clear();
                        });
                    }
                    catch
                    {
                        // Ignore everything
                    }
                }
            }
        }

        private void KillViewerThread()
        {
            if (_ViewerThread == null)
                return;

            _ViewerThreadStarted = false;
            _ViewerThread.Join();

            _ViewerThread = null;

            Debug.WriteLine("ViewerThread has been killed");
        }

        private string CheckAndUpdatePath()
        {
            string result = string.Empty;
            bool fixFmt = false;

            try
            {
                using (StreamReader sr = new StreamReader(( (App)App.Current ).InstanceControl.GlobalControlPath))
                {
                    uint changedPathCounter = 1;
                    try
                    {
                        changedPathCounter = Convert.ToUInt32(sr.ReadLine());
                    }
                    catch
                    {
                        changedPathCounter = 1;
                        fixFmt = true;
                    }

                    if (changedPathCounter == _ChangedPathCount)
                    {
                        sr.Close();
                        return result;
                    }

                    result = sr.ReadLine();
                    _ChangedPathCount = changedPathCounter;
                    sr.Close();
                }
            }
            catch
            { return result; }

            if (fixFmt)
            {
                ( (App)App.Current ).InstanceControl.SetPathGlobally(result + "");
            }

            return result;
        }

        private List<LogLine> CheckFileOutput()
        {
            List<LogLine> ll = new List<LogLine>();

            try
            {
                using (StreamReader sr = new StreamReader(_PathToFile))
                {
                    sr.ReadToEnd();
                    if (sr.BaseStream.Position == _LinesRead)
                    {
                        sr.Close();
                        return ll;
                    }

                    sr.BaseStream.Seek(_LinesRead, SeekOrigin.Begin);

                    while (!sr.EndOfStream)
                    {
                        ll.Add(new LogLine(sr.ReadLine()));
                    }

                    _LinesRead = sr.BaseStream.Position;
                    sr.Close();
                }

                for (int i = 0; i < ll.Count; i++)
                {
                    if (ll[i].Line.ToLower().Contains("info"))
                        ll[i] = new LogLine(Brushes.LightGreen, ll[i].Line);
                    if (ll[i].Line.ToLower().Contains("warning"))
                        ll[i] = new LogLine(Brushes.Yellow, ll[i].Line);
                    if (ll[i].Line.ToLower().Contains("error"))
                        ll[i] = new LogLine(Brushes.Red, ll[i].Line);
                }

                _ThreadSleep = _DefaultThreadSleep;
            }
            catch
            {
                _ThreadSleep = _LazyThreadSleep;
            }

            return ll;
        }

        private Thread? _ViewerThread = null;
        private bool _ViewerThreadStarted = false;
        int _ThreadSleep = _DefaultThreadSleep;
        const int _DefaultThreadSleep = 50;
        const int _LazyThreadSleep = 500;

        uint _ChangedPathCount = 0;
        string _PathToFile = "";
        long _LinesRead = 0;

        private readonly MainWindow _ParentWindow;
    }
}
