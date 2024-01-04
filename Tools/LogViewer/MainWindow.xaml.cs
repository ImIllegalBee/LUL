using System.Diagnostics;
using System.Windows;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;

namespace LogViewer
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public void SetPathTextFields(string value)
        {
            // PathToFileTextBox.Text = value;
            PathTextDisplay.Text = value;
        }

        public void AddChunkOfLines(List<LogLine> lines) 
        {
            for (int i = 0; i < lines.Count; i++)
            {
                AddLine(lines[i]);
            }

            if (MainOutViewScrl.ScrollableHeight == MainOutViewScrl.VerticalOffset)
            {
                MainOutViewScrl.ScrollToBottom();
            }
        }

        // Private ------------------------------------------------------------

        private void AddLine(LogLine ll)
        {
            if (MainOutView.Inlines.Count >= _MaxLines)
            {
                MainOutViewCount.Inlines.Remove(MainOutViewCount.Inlines.FirstInline);
                MainOutView.Inlines.Remove(MainOutView.Inlines.FirstInline);
            }

            MainOutViewCount.Inlines.Add(new Run(( _Lines++ ) + "\n") { Foreground = Brushes.White });
            MainOutView.Inlines.Add(new Run(ll.Line + "\n") { Foreground = ll.Brush });
        }

        private void OnButtonSetPath(object sender, RoutedEventArgs e)
        {
            Debug.WriteLine("Set path button clicked");

            ( (App)App.Current ).InstanceControl.SetPathGlobally(PathToFileTextBox.Text);
            PathTextDisplay.Text = PathToFileTextBox.Text;
        }

        private void PathToFileTextBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            WasTextInput();
        }

        private void PathToFileTextBox_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            WasTextInput();
        }

        private void WasTextInput()
        {
            if (!_WasTextInput)
            {
                PathToFileTextBox.Text = "";
                PathToFileTextBox.Foreground = Brushes.Black;
                _WasTextInput = true;
            }
        }

        private bool _WasTextInput = false;

        private const int _MaxLines = 512;
        private int _Lines = 1;
    }
}