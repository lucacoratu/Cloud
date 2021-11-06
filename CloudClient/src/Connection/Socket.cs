using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Windows;

namespace CloudClient.src.Connection
{
    static class Socket
    {
        //Member variables
        static TcpClient client;
        static NetworkStream stream;
        static Byte[] serverAnswer;

        static private string ByteArrayToHexString(byte[] Bytes)
        {
            StringBuilder Result = new StringBuilder(Bytes.Length * 2);
            string HexAlphabet = "0123456789ABCDEF";

            foreach (byte B in Bytes)
            {
                Result.Append(HexAlphabet[(int)(B >> 4)]);
                Result.Append(HexAlphabet[(int)(B & 0xF)]);
            }

            return Result.ToString();
        }
        static public void ConnectToServer(string serverIpAddress, Int32 port)
        {
            try
            {
                // Create a TcpClient.
                // Note, for this client to work you need to have a TcpServer
                // connected to the same address as specified by the server, port
                // combination.
                client = new TcpClient(serverIpAddress, port);
                stream = client.GetStream();

                Byte[] data = new Byte[256];

                // String to store the response ASCII representation.
                String responseData = String.Empty;

                // Read the first batch of the TcpServer response bytes.
                Int32 bytes = stream.Read(data, 0, data.Length);
                serverAnswer = data;

                //In this message the server sends its public key
                src.Connection.Message message = new Message(data);

                MessageBox.Show(ByteArrayToHexString(message.GetMessageData()).Substring(0, 30), "Response", MessageBoxButton.OK);
            }
            catch (ArgumentNullException e)
            {
                MessageBox.Show("Cannot connect to the server");
            }
            catch (SocketException e)
            {
                MessageBox.Show("Cannot connect to the server");
            }
        }

        static public Byte[] GetServerAnswer()
        {
            return serverAnswer;
        }

        static public void SendToServer(Byte[] data, int dataLength)
        {
            //Sends the byte array to the server
            try
            {
                stream.Write(data, 0, dataLength);
                int bytesRead = stream.Read(serverAnswer, 0, 256);
            }
            catch(ArgumentNullException e){
                MessageBox.Show(e.Message, "Error::Socket.SendToServer", MessageBoxButton.OK, MessageBoxImage.Error);
            }

        }
    }
}
