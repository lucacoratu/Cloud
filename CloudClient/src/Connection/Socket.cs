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
                responseData = System.Text.Encoding.ASCII.GetString(data, 0, bytes);

                MessageBox.Show(responseData,"Response", MessageBoxButton.OK);
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
    }
}
