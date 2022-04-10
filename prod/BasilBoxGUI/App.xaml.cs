using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using static BasilBoxGUI.COM;

namespace BasilBoxGUI
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>

    public partial class App : Application
    {
        private MainWindow mw;
        private COM com = new COM();

        private void Application_Startup(object sender, StartupEventArgs e)
        {
            mw = new MainWindow();
            mw.Show();

            com.ConnectionStateChanged += new ConnectionStateChangedEventHandler(OnConnectionStateChanged);
            com.setAutoConnect(1000);
        }

        public void OnConnectionStateChanged(object sender, EventHandler e)
        {
            Dispatcher.Invoke(() =>
            {
                mw.SetConnectionState(com.connectionState);
            });
        }
    }


}
