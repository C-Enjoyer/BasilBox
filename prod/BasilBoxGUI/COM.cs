using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.Windows;
using System.Threading;

namespace BasilBoxGUI
{



    public class COM
    {
        private SafeSerialPort serialPort;
        public ConnectionState connectionState { get; set; } = ConnectionState.NotOpened;
        private string serialPortName = "COM69";
        private int serialPortBaudrate = 115200;
        private int serialWriteTimeout = 250;
        private int serialReadTimeout = 250;
        private string pingWrite = "f";
        private string pingRead = "You are a fucking idiot!";
        private bool pingSuccess = false;

        private Timer autoConnectTimer;
        private bool autoConnectTimerRunning = false;
        private int autoConnectTimerMs = Timeout.Infinite;
        public COM()
        {
            serialPort = new SafeSerialPort(serialPortName, serialPortBaudrate, Parity.None, 8, StopBits.One);
            serialPort.Handshake = Handshake.None;
            serialPort.DataReceived += new SerialDataReceivedEventHandler(received);

            serialPort.WriteTimeout = serialWriteTimeout;
            serialPort.ReadTimeout = serialReadTimeout;

            autoConnectTimer = new Timer(autoConnectTimerEvent, null, Timeout.Infinite, Timeout.Infinite);
        }

        public delegate void ConnectionStateChangedEventHandler(object sender, EventHandler e);
        public event ConnectionStateChangedEventHandler ConnectionStateChanged;
        protected virtual void OnConnectionStateChanged()
        {
            ConnectionStateChanged(this, null);
        }
        public void setAutoConnect(int ms)
        {
            bool wasRunning = autoConnectTimerRunning;
            autoConnectTimerRunning = true;
            autoConnectTimerMs = ms;
            if(!wasRunning)
            {
                autoConnectTimer.Change(autoConnectTimerMs, Timeout.Infinite);
            }
        }
        public void stopAutoConnect()
        {
            autoConnectTimerRunning = false;
            autoConnectTimerMs = Timeout.Infinite;
        }
        private void autoConnectTimerEvent(Object state)
        {
            if(!autoConnectTimerRunning)
            {
                return;
            }

            checkState();
            autoConnectTimer.Change(autoConnectTimerMs, Timeout.Infinite);
        }
        private ConnectionState checkOpen()
        {
            if(serialPort.IsOpen)
            {
                return connectionState;
            }

            if(!serialPort.Open())
            {
                return ConnectionState.NotOpened;
            }

            return ConnectionState.Opened;
        }
        private bool ping()
        {
            pingSuccess = false;
            write(pingWrite);
            Thread.Sleep(serialWriteTimeout + serialReadTimeout);

            return pingSuccess;
        }
        public void writeLine(string line)
        {
            if (checkOpen() == ConnectionState.NotOpened)
            {
                return;
            }
            serialPort.Write(line + "\r\n");
        }
        public void write(string data)
        {
            if (checkOpen() == ConnectionState.NotOpened)
            {
                return;
            }
            serialPort.Write(data);
        }
        public ConnectionState checkState()
        {
            ConnectionState OldConnectionState = connectionState;

            connectionState = checkOpen();

            if (connectionState == ConnectionState.Opened)
            {
                connectionState = ping() ? ConnectionState.Connected : ConnectionState.Opened;
            }

            if(OldConnectionState != connectionState)
            {
                OnConnectionStateChanged();
            }

            return connectionState;
        }
        public void received(object sender, SerialDataReceivedEventArgs e)
        {
            Thread.Sleep(serialReadTimeout);
            string line = serialPort.ReadLine();
            
            if(line == pingRead)
            {
                pingSuccess = true;
            }
        }
        public enum ConnectionState
        {
            NotOpened,
            Opened,
            Connected
        }
    }
}
