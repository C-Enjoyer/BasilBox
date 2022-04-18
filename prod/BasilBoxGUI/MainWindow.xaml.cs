using System.Collections.Generic;
using System.Data;
using System.Windows;
using System.Windows.Media;

namespace BasilBoxGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        private Dictionary<COM.ConnectionState, string> connectionStateDict = new Dictionary<COM.ConnectionState, string>
        {
            {COM.ConnectionState.NotOpened, "Not connected"},
            {COM.ConnectionState.Opened, "Opened"},
            {COM.ConnectionState.Connected, "Connected"}
        };
        private Dictionary<COM.ConnectionState, SolidColorBrush> connectionStateCol = new Dictionary<COM.ConnectionState, SolidColorBrush>
        {
            {COM.ConnectionState.NotOpened, Brushes.Red},
            {COM.ConnectionState.Opened, Brushes.Orange},
            {COM.ConnectionState.Connected, Brushes.Green}
        };

        public MainWindow()
        {
            InitializeComponent();
        }

        public void SetConnectionState(COM.ConnectionState connectionState)
        {
            txtConnectionState.Text = connectionStateDict[connectionState];
            txtConnectionState.Background = connectionStateCol[connectionState];
        }

    }
}
