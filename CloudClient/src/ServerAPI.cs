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

            //MessageBox.Show(resp);

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

           // MessageBox.Show(resp);

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

            //MessageBox.Show(resp);

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

            //MessageBox.Show(resp);

            return resp;
        }

        public static string ChangeDirectoryFullPath(string directoryPath)
        {
            /*
             * Send a request to the server to change the current directory to the directory path
             * Directory path is relative to the home directory
             */
            MessageHeader header = new MessageHeader();
            header.action = (byte)Action.CHANGE_DIRECTORY_FULL_PATH;
            header.errorNo = (byte)0;
            header.dataLength = (int)(directoryPath.Length);

            Message message = new Message(header, Encoding.ASCII.GetBytes(directoryPath));
            byte[] server_message = message.GetMessageAsByteArray();
            Socket.SendToServer(server_message, server_message.Length);

            Message server_answer = Socket.GetServerMessage();
            string resp = Encoding.ASCII.GetString(server_answer.GetMessageData());

            //MessageBox.Show(resp);

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
                using (FileStream fs = File.Create(localFilename))
                {
                    //you can use the filstream here to put stuff in the file if you want to
                }
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

        public static string UploadFile(string serverFilename, string localFilename)
        {
            MessageHeader header_upload = new MessageHeader();
            header_upload.action = (byte)Action.UPLOAD_FILE;
            header_upload.errorNo = (byte)0;
            header_upload.dataLength = serverFilename.Length;

            Message message_upload = new Message(header_upload, Encoding.UTF8.GetBytes(serverFilename));
            byte[] message_upload_bytes = message_upload.GetMessageAsByteArray();

            int data_size = 32762 - serverFilename.Length - 1;

            Socket.SendToServer(message_upload_bytes, message_upload_bytes.Length);

            Message server_upload_response = Socket.GetServerMessage();
            if(server_upload_response.GetMessageAction() == (byte)Action.START_UPLOAD)
            {
                //Start the upload
                FileInfo info = new FileInfo(localFilename);
                long file_length = info.Length;
                long number_chunks = file_length / data_size;

                long current_chunk = 0;

                if(number_chunks == 0)
                {
                    byte[] data = File.ReadAllBytes(localFilename);

                    MessageHeader header_last_chunk = new MessageHeader();
                    header_last_chunk.action = (byte)Action.LAST_CHUNK;
                    header_last_chunk.errorNo = (byte)0;
                    header_last_chunk.dataLength = (int)(file_length + serverFilename.Length + 1);

                    byte[] filename_space = new byte[serverFilename.Length + 1];
                    byte[] filename = Encoding.ASCII.GetBytes(serverFilename);
                    for (int i = 0; i < filename.Length; i++)
                    {
                        filename_space[i] = filename[i];
                    }

                    filename_space[serverFilename.Length] = (byte)(32);
                    byte[] server_data = new byte[data.Length + filename.Length + 1];

                    for (int i = 0; i <= filename.Length; i++)
                    {
                        server_data[i] = filename_space[i];
                    }

                    for (int i = 0; i < file_length; i++)
                    {
                        server_data[filename.Length + 1 + i] = data[i];
                    }

                    Message last_chunk_message = new Message(header_last_chunk, server_data);
                    byte[] server_bytes = last_chunk_message.GetMessageAsByteArray();
                    Socket.SendToServer(server_bytes, server_bytes.Length - 1);

                    Message server_last_chunk_response = Socket.GetServerMessage();
                    if(server_last_chunk_response.GetMessageAction() == (byte)Action.ACKNOWLEDGEMENT)
                    {
                        string resp = "Upload completed";
                        MessageBox.Show(resp);
                        return resp;
                    }
                }
                else
                {
                    do
                    {
                        byte[] data = new byte[data_size];
                        long current_offset = current_chunk * data_size;
                        using (BinaryReader reader = new BinaryReader(new FileStream(localFilename, FileMode.Open)))
                        {
                            reader.BaseStream.Seek(current_offset, SeekOrigin.Begin);
                            int totalRead = 0;
                            do
                            {
                                int bytesRead = reader.Read(data, totalRead, data_size - totalRead);
                                if (bytesRead <= 0)
                                    break;
                                totalRead += bytesRead;
                            } while (totalRead < data_size);
                        }

                        byte[] filename_space = new byte[serverFilename.Length + 2];
                        byte[] filename = Encoding.ASCII.GetBytes(serverFilename);
                        for(int i =0; i < filename.Length; i++)
                        {
                            filename_space[i] = filename[i];
                        }

                        filename_space[serverFilename.Length] = (byte)(32);
                        byte[] server_data = new byte[data.Length + filename.Length + 1];

                        for (int i = 0; i <= filename.Length; i++)
                        {
                            server_data[i] = filename_space[i];
                        }

                        for (int i = 0; i < data.Length; i++)
                        {
                            server_data[filename.Length + 1 + i] = data[i];       
                        }

                        MessageHeader header_chunk = new MessageHeader();
                        header_chunk.action = (byte)Action.RECEIVE_CHUNK;
                        header_chunk.errorNo = (byte)0;
                        header_chunk.dataLength = server_data.Length;

                        Message chunk_message = new Message(header_chunk, server_data);
                        byte[] server_bytes = chunk_message.GetMessageAsByteArray();
                        Socket.SendToServer(server_bytes, server_bytes.Length - 1);

                        Message server_answer = Socket.GetServerMessage();
                        if (server_answer.GetMessageAction() != (byte)Action.ACKNOWLEDGEMENT)
                            break;

                        current_chunk++;
                    } while (current_chunk < number_chunks);

                    //Sent the last chunk
                    long last_chunk_offset = number_chunks * data_size;
                    byte[] data_last = new byte[data_size];
                    int totalRead_last = 0;
                    using (BinaryReader reader = new BinaryReader(new FileStream(localFilename, FileMode.Open)))
                    {
                        reader.BaseStream.Seek(last_chunk_offset, SeekOrigin.Begin);
                        do
                        {
                            int bytesRead = reader.Read(data_last, 0, data_size);
                            if (bytesRead <= 0)
                                break;
                            totalRead_last += bytesRead;
                        } while (totalRead_last < data_size);
                    }

                    byte[] filename_last = Encoding.UTF8.GetBytes(serverFilename + ' ');
                    byte[] server_data_last = new byte[totalRead_last + filename_last.Length];

                    for (int i = 0; i < filename_last.Length; i++)
                    {
                        server_data_last[i] = filename_last[i];
                    }

                    for (int i = 0; i < totalRead_last; i++)
                    {
                        server_data_last[filename_last.Length + i] = data_last[i];
                    }

                    MessageHeader header_last_chunk = new MessageHeader();
                    header_last_chunk.action = (byte)Action.LAST_CHUNK;
                    header_last_chunk.errorNo = (byte)0;
                    header_last_chunk.dataLength = (int)(totalRead_last + serverFilename.Length + 1);

                    Message chunk_last_message = new Message(header_last_chunk, server_data_last);
                    byte[] server_bytes_last = chunk_last_message.GetMessageAsByteArray();
                    Socket.SendToServer(server_bytes_last, server_bytes_last.Length);

                    Message server_answer_last = Socket.GetServerMessage();
                    if(server_answer_last.GetMessageAction() == (byte)Action.ACKNOWLEDGEMENT)
                    {
                        string resp2 = "Upload completed!";
                        MessageBox.Show(resp2);
                        return resp2;
                    }
                }
            }

            return null;
        }
    }
}
