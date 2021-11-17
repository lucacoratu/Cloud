using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Text.RegularExpressions;
using CloudClient.src;

namespace CloudClient
{
    /// <summary>
    /// Interaction logic for RegisterWindow.xaml
    /// </summary>
    public partial class RegisterWindow : Window
    {
        public RegisterWindow()
        {
            InitializeComponent();
        }

        private void RegisterWindow1_Closed(object sender, EventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void txtbox_password_GotFocus(object sender, RoutedEventArgs e)
        {
            this.txtbox_password.Visibility = Visibility.Hidden;
            this.pwdbox_password.Visibility = Visibility.Visible;
            this.pwdbox_password.Focus();
        }

        private void txtbox_confirmpass_GotFocus(object sender, RoutedEventArgs e)
        {
            this.txtbox_confirmpass.Visibility = Visibility.Hidden;
            this.pwdbox_confirmpass.Visibility = Visibility.Visible;
            this.pwdbox_confirmpass.Focus();
        }

        private void txtbox_username_GotFocus(object sender, RoutedEventArgs e)
        {
            this.txtbox_username.Text = "";
            this.txtbox_username.HorizontalContentAlignment = HorizontalAlignment.Left;
        }

        private void txtbox_email_GotFocus(object sender, RoutedEventArgs e)
        {
            this.txtbox_email.Text = "";
            this.txtbox_email.HorizontalContentAlignment = HorizontalAlignment.Left;
        }

        private void VerifyUserInput()
        {
            /*
             * This function verifies if the text in the text boxes, respectively in the password boxes
             * has been inputed correctly by the user.
             * Returns true if the input is correct, false otherwise
             */

            //The text boxes/ password boxes cannot be empty
            if (this.txtbox_username.Text == "")
                throw new Exception("Fields cannot be left empty!");

            if (this.pwdbox_password.Password == "")
                throw new Exception("Fields cannot be left empty!");

            if (this.pwdbox_confirmpass.Password == "")
                throw new Exception("Fields cannot be left empty!");

            if(this.txtbox_email.Text == "")
                throw new Exception("Fields cannot be left empty!");

            //Check the password box and confirm password box contain the same text
            if (this.pwdbox_password.Password != this.pwdbox_confirmpass.Password)
                throw new Exception("Password and confirm password do not match!");

            //Check if the email is in the correct format
            Regex emailRegex = new Regex("^([a-zA-Z])([a-zA-Z0-9]|\\.|_)+@([a-z]|[A-Z])+\\.([a-zA-Z])+");
            Match match = emailRegex.Match(this.txtbox_email.Text);

            if (match.Success == false)
                throw new Exception("Email is not in a correct format!");
        }
        private void btn_register_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                VerifyUserInput();
            }
            catch(Exception exp)
            {
                this.lbl_result.Content = exp.Message;
                return;
            }

            //Send the information to the server and check it's response
            string message_data = this.txtbox_username.Text + " " + this.pwdbox_password.Password + " " + this.txtbox_email.Text;
            src.Connection.MessageHeader header = new src.Connection.MessageHeader();
            header.action = (byte)Action.REGISTER_ACCOUNT;
            header.errorNo = (byte)(0);
            header.dataLength = (int)(message_data.Length);

            src.Connection.Message message = new src.Connection.Message(header, Encoding.ASCII.GetBytes(message_data));

            byte[] server_message = message.GetMessageAsByteArray();
            src.Connection.Socket.SendToServer(server_message, server_message.Length);

            byte[] server_answer = src.Connection.Socket.GetServerAnswer();

            src.Connection.Message response = new src.Connection.Message(server_answer);
            string resp = Encoding.ASCII.GetString(response.GetMessageData());
            MessageBox.Show(resp, "Message");
        }
    }
}
