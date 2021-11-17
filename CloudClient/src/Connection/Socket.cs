using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Windows;
using CloudClient.src.Connection;
using CloudClient.src.Encryption;
using System.Security.Cryptography;
using CloudClient.src;

namespace CloudClient.src.Connection
{
    static class Socket
    {
        //Member variables
        static TcpClient client;
        static NetworkStream stream;
        static Byte[] serverAnswer;
        static Byte[] secret;
        static Message serverMessage;

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

                //Generate the key pair for the client
                byte[] clientPublicKey = new byte[16];
                byte[] clientPrivateKey = new byte[16];
                DHExchange.generate_key_pair(clientPublicKey, clientPrivateKey);
                //MessageBox.Show(ByteArrayToHexString(message.GetMessageData()).Substring(0,30), "Response", MessageBoxButton.OK);
                byte[] serverPublicKey = message.GetMessageData();

                //Generate the secret for encrypted connection
                secret = DHExchange.generate_key_secret(clientPrivateKey, serverPublicKey);

                //Send the public key back to the server so it can generate the secret too
                MessageHeader header = new MessageHeader();
                header.action = (byte)Action.RECEIVE_PUBLIC_KEY;
                header.errorNo = (byte)0;
                header.dataLength = 16;
                src.Connection.Message publicKeyMessage = new src.Connection.Message(header, clientPublicKey);

                stream.Write(publicKeyMessage.GetMessageAsByteArray());

                bytes = stream.Read(data, 0, data.Length);
                serverAnswer = data;

                src.Connection.Message message1 = new Message(data);

                string mes1 = Encoding.ASCII.GetString(message1.GetMessageData());
                //MessageBox.Show(mes1 + "\nsecret: " + ByteArrayToHexString(secret).Substring(0,30));

                //TO DO...Test the connection with the server (If the server responds with unknown command then regenerate the keys)
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

        static public Message GetServerMessage()
        {
            return serverMessage;
        }

        static public void SendToServer(Byte[] data, int dataLength)
        {
            //Sends the byte array to the server
            try
            {
                byte[] encrypted;
                byte[] iv = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

                // Encrypt the string to an array of bytes.
                encrypted = Encryption.EncryptionAPI.EncryptStringToBytes(System.Text.Encoding.UTF8.GetString(data, 0, dataLength), secret, iv);

                byte[] answer = new byte[256];
                Array.Clear(answer, 0, 256); 
                stream.Write(encrypted, 0, encrypted.Length);
                int bytesRead = stream.Read(answer, 0, 256);

                byte[] cypherText = new byte[bytesRead];
                for(int i =0; i<bytesRead; i++)
                {
                    cypherText[i] = answer[i];
                }

                string decryptedString = Encryption.EncryptionAPI.DecryptStringFromBytes(cypherText, secret, iv);
                serverAnswer = Encoding.UTF8.GetBytes(decryptedString);

                //Create a message from the server answer
                serverMessage = new src.Connection.Message(serverAnswer);
            }
            catch(ArgumentNullException e){
                MessageBox.Show(e.Message, "Error::Socket.SendToServer", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }
    }
}
