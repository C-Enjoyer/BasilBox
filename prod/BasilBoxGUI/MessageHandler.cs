using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Google.Protobuf;

namespace BasilBoxGUI
{
    public class MessageHandler
    {
        private const Int32 pingId = 42069420;
        private UInt32 nextMessageLength = 0;
        private byte[] inBuf = new byte[0];
        private bool pingFlag = false;
        private const UInt32 messageLengthBytes = 4;

        public void Handle(byte[] bytes)
        {
            inBuf = Combine(inBuf, bytes);

            if (nextMessageLength == 0)
            {
                if(inBuf.Length < messageLengthBytes)
                {
                    return;
                }

                nextMessageLength = BitConverter.ToUInt32(inBuf, 0);
                inBuf = inBuf.Skip((int) messageLengthBytes).ToArray();
            }
            
            if(inBuf.Length < nextMessageLength)
            {
                return;
            }

            byte[] message = new byte[nextMessageLength];
            Array.Copy(inBuf, message, nextMessageLength);
            inBuf = inBuf.Skip((int)nextMessageLength).ToArray();
            nextMessageLength = 0;

            BasilMessage basilMessage = Decode(message);

            switch(basilMessage.MessageCase)
            {
                case BasilMessage.MessageOneofCase.InfoMessage:
                    HandleInfoMessage(basilMessage.InfoMessage);
                break;
                default:
                break;
            }
        }

        private void HandleInfoMessage(InfoMessage infoMessage)
        {
            if(infoMessage.HasPingId && infoMessage.PingId == pingId)
            {
                pingFlag = true;
            }
        }

        public bool GetPingFlag()
        {
            bool flag = pingFlag;
            pingFlag = false;
            return flag;
        }

        public BasilMessage Decode(byte[] bytes)
        {
            return BasilMessage.Parser.ParseFrom(bytes);
        }

        public byte[] Encode(InfoMessage msg)
        {
            BasilMessage basilMessage = new BasilMessage();
            basilMessage.InfoMessage = msg;
            byte[] msgBytes;
            using (MemoryStream stream = new MemoryStream())
            {
                basilMessage.WriteTo(stream);
                msgBytes = stream.ToArray();
            }

            byte[] lenBytes = BitConverter.GetBytes((UInt32) msgBytes.Length);

            byte[] allBytes = Combine(lenBytes, msgBytes);

            return allBytes;
        }

        public byte[] GetPingMessage()
        {
            InfoMessage message = new InfoMessage();
            message.PingId = pingId;

            return Encode(message);
        }

        private byte[] Combine(byte[] ba1, byte[] ba2)
        {
            byte[] allBytes = new byte[ba1.Length + ba2.Length];
            Buffer.BlockCopy(ba1, 0, allBytes, 0, ba1.Length);
            Buffer.BlockCopy(ba2, 0, allBytes, ba1.Length, ba2.Length);
            return allBytes;
        }

        public enum MessageType
        {
            UnknownMessage,
            PingMessage,
            OtherMessage
        }

    }


}
