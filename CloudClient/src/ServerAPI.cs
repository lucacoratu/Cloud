using System;
using System.Collections.Generic;
using System.Text;
using CloudClient.src.Connection;
using CloudClient.src.Encryption;
using System.Windows;
using System.Windows.Controls;

namespace CloudClient.src
{
    public struct FileDetails
    {
        public string filename;
        public string type;
    }
    static class ServerAPI
    {
        public static string LoginIntoAccount(string username, string password)
        {
            byte[] bytes = new byte[username.Length + password.Length + 1];
            byte[] usernameBytes = Encoding.ASCII.GetBytes(username);
            byte[] passwordBytes = Encoding.ASCII.GetBytes(password);

            for (int i = 0; i < usernameBytes.Length; i++)
            {
                bytes[i] = usernameBytes[i];
            }
            bytes[usernameBytes.Length] = (byte)32;

            for (int i = usernameBytes.Length + 1; i < usernameBytes.Length + passwordBytes.Length + 1; i++)
            {
                bytes[i] = passwordBytes[i - usernameBytes.Length - 1];
            }

            src.Connection.MessageHeader header;
            header.action = (byte)Action.LOGIN_INTO_ACCOUNT;
            header.errorNo = (byte)0;
            header.dataLength = bytes.Length;

            src.Connection.Message message = new src.Connection.Message(header, bytes);

            Byte[] serverMesssage = message.GetMessageAsByteArray();
            src.Connection.Socket.SendToServer(serverMesssage, serverMesssage.Length);

            src.Connection.Message response = new src.Connection.Message(src.Connection.Socket.GetServerAnswer());
            string answer = Encoding.ASCII.GetString(response.GetMessageData());

            return answer;
        }

        public static string RegisterAccount(string username, string password, string email)
        {
            //Send the information to the server and check it's response
            string message_data = username + " " + password + " " + email;
            src.Connection.MessageHeader header = new src.Connection.MessageHeader();
            header.action = (byte)Action.REGISTER_ACCOUNT;
            header.errorNo = (byte)(0);
            header.dataLength = (int)(message_data.Length);


            src.Connection.Message message = new src.Connection.Message(header, Encoding.ASCII.GetBytes(message_data));

            byte[] server_message = message.GetMessageAsByteArray();
            src.Connection.Socket.SendToServer(server_message, server_message.Length);

            byte[] server_answer = src.Connection.Socket.GetServerAnswer();

            src.Connection.Message response = src.Connection.Socket.GetServerMessage();
            string resp = Encoding.ASCII.GetString(response.GetMessageData());

            return resp;
        }

        public static List<FileDetails> ViewDirectoryContents(string directory)
        {
            /*
             * Requests to the server the contents of a directory
             */
            List<FileDetails> list = new List<FileDetails>();

            MessageHeader header = new MessageHeader();
            header.action = (byte)Action.VIEW_DIRECTORY_CONTENT;
            header.errorNo = (byte)0;
            header.dataLength = (int)(directory.Length);

            Message message = new Message(header, Encoding.ASCII.GetBytes(directory));
            byte[] server_message = message.GetMessageAsByteArray();
            Socket.SendToServer(server_message, server_message.Length);

            Message server_answer = Socket.GetServerMessage();
            string resp = Encoding.ASCII.GetString(server_answer.GetMessageData());

            MessageBox.Show(resp);

            if (resp == "There are 0 files in this directory!")
                return list;

            //Break the string into list of file details
            var tokens = resp.Split(' ');

            for(int i = 0; i < tokens.Length - 1; i += 2)
            {
                FileDetails details = new FileDetails();
                details.filename = tokens[i];
                details.type = tokens[i + 1];
                list.Add(details);
            }

            return list;
        }

        public static string CreateNewFile(string filename)
        {
            /*
             * Sends the server a request to create a new file in the current active directory 
             */
            MessageHeader header = new MessageHeader();
            header.action = (byte)Action.CREATE_NEW_FILE;
            header.errorNo = (byte)0;
            header.dataLength = (int)(filename.Length);

            Message message = new Message(header, Encoding.ASCII.GetBytes(filename));
            byte[] server_message = message.GetMessageAsByteArray();
            Socket.SendToServer(server_message, server_message.Length);

            Message server_answer = Socket.GetServerMessage();
            string resp = Encoding.ASCII.GetString(server_answer.GetMessageData());

            MessageBox.Show(resp);

            return resp;
        }

        public static string CreateNewDirectory(string directory)
        {
            /*
             * Sends the server a request to create a new directory in the current directory 
             */
            MessageHeader header = new MessageHeader();
            header.action = (byte)Action.CREATE_NEW_DIRECTORY;
            header.errorNo = (byte)0;
            header.dataLength = (int)(directory.Length);

            Message message = new Message(header, Encoding.ASCII.GetBytes(directory));
            byte[] server_message = message.GetMessageAsByteArray();
            Socket.SendToServer(server_message, server_message.Length);

            Message server_answer = Socket.GetServerMessage();
            string resp = Encoding.ASCII.GetString(server_answer.GetMessageData());

            MessageBox.Show(resp);

            return resp;
        }

        public static string ChangeDirectory(string directory)
        {
            /*
             * Send a request to the server to change the current directory to the directory 
             */
            MessageHeader header = new MessageHeader();
            header.action = (byte)Action.CHANGE_DIRECTORY;
            header.errorNo = (byte)0;
            header.dataLength = (int)(directory.Length);

            Message message = new Message(header, Encoding.ASCII.GetBytes(directory));
            byte[] server_message = message.GetMessageAsByteArray();
            Socket.SendToServer(server_message, server_message.Length);

            Message server_answer = Socket.GetServerMessage();
            string resp = Encoding.ASCII.GetString(server_answer.GetMessageData());

            MessageBox.Show(resp);

            return resp;
        }
    }
}
