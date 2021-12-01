using System;
using System.Collections.Generic;
using System.Text;
using CloudClient.src.Connection;
using CloudClient.src.Encryption;
using System.Windows;
using System.Windows.Controls;
using System.IO;

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

            for (int i = 0; i < tokens.Length - 1; i += 2)
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

        public static string DownloadFile(string filename, string localFilename)
        {
            //Clear the local filename if it exists
            if (File.Exists(localFilename))
            {
                File.WriteAllText(localFilename, String.Empty);
            }
            else
            {
                File.Create(localFilename);
            }

            MessageHeader header_download = new MessageHeader();
            header_download.action = (byte)Action.DOWNLOAD_FILE;
            header_download.errorNo = (byte)0;
            header_download.dataLength = filename.Length;

            Message message_download = new Message(header_download, Encoding.ASCII.GetBytes(filename));
            byte[] server_message = message_download.GetMessageAsByteArray();

            Socket.SendToServer(server_message, server_message.Length);

            Message server_answer = Socket.GetServerMessage();
            string resp = Encoding.ASCII.GetString(server_answer.GetMessageData());

            //MessageBox.Show(resp);

            if (resp == "File does not exist!")
            {
                return resp;
            }

            MessageHeader header_start_trans = new MessageHeader();
            header_start_trans.action = (byte)Action.START_TRANSMISSION;
            header_start_trans.errorNo = (byte)0;
            header_start_trans.dataLength = filename.Length;

            Message message_start_trans = new Message(header_start_trans, Encoding.ASCII.GetBytes(filename));
            byte[] server_message_trans = message_start_trans.GetMessageAsByteArray();

            Socket.SendToServer(server_message_trans, server_message.Length);

            Message server_answer2 = Socket.GetServerMessage();
            string resp2 = Encoding.ASCII.GetString(server_answer2.GetMessageData());

            if (server_answer2.GetMessageAction() == (byte)Action.LAST_CHUNK)
            {
                MessageHeader header_ack = new MessageHeader();
                header_ack.action = (byte)Action.ACKNOWLEDGEMENT;
                header_ack.errorNo = (byte)0;
                header_ack.dataLength = filename.Length;
                Message message_ack = new Message(header_ack, Encoding.ASCII.GetBytes(filename));
                byte[] server_message_ack = message_ack.GetMessageAsByteArray();

                //File.AppendAllText(localFilename, resp2);
                using (var stream = new FileStream(localFilename, FileMode.Append))
                {
                    byte[] bytes = server_answer2.GetMessageData();
                    stream.Write(bytes, 0, bytes.Length);
                }

                Socket.SendToServer(server_message_ack, server_message_ack.Length);

                server_answer2 = Socket.GetServerMessage();
                resp2 = Encoding.ASCII.GetString(server_answer2.GetMessageData());

                //MessageBox.Show(resp2);
                return resp2;
            }

            Message server_answer3 = server_answer2;
            string resp3 = resp2;
            do
            {
                //Append the text into the file
                using (var stream = new FileStream(localFilename, FileMode.Append, FileAccess.Write, FileShare.Read))
                {
                    byte[] bytes = server_answer3.GetMessageData();
                    stream.Write(bytes, 0, bytes.Length);
                }

                MessageHeader header_ack = new MessageHeader();
                header_ack.action = (byte)Action.ACKNOWLEDGEMENT;
                header_ack.errorNo = (byte)0;
                header_ack.dataLength = filename.Length;
                Message message_ack = new Message(header_ack, Encoding.ASCII.GetBytes(filename));
                byte[] server_message_ack = message_ack.GetMessageAsByteArray();

                Socket.SendToServer(server_message_ack, server_message_ack.Length);

                server_answer3 = Socket.GetServerMessage();
                resp3 = Encoding.ASCII.GetString(server_answer3.GetMessageData());
            } while (server_answer3.GetMessageAction() != (byte)Action.LAST_CHUNK);

            //Append the thext into the file
            using (var stream = new FileStream(localFilename, FileMode.Append, FileAccess.Write, FileShare.Read))
            {
                byte[] bytes = server_answer3.GetMessageData();
                stream.Write(bytes, 0, bytes.Length);
            }

            //Send the last aknowledgement
            MessageHeader header_ack_final = new MessageHeader();
            header_ack_final.action = (byte)Action.ACKNOWLEDGEMENT;
            header_ack_final.errorNo = (byte)0;
            header_ack_final.dataLength = filename.Length;
            Message message_ack_final = new Message(header_ack_final, Encoding.ASCII.GetBytes(filename));
            byte[] server_message_ack_final = message_ack_final.GetMessageAsByteArray();

            Socket.SendToServer(server_message_ack_final, server_message_ack_final.Length);

            server_answer3 = Socket.GetServerMessage();
            resp3 = Encoding.ASCII.GetString(server_answer3.GetMessageData());

            MessageBox.Show(resp3);
            return resp2;
        }
    }
}
