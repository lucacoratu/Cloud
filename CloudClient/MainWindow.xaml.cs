using CloudClient.MVVM.View;
using System;
using System.Collections.Generic;
using System.Linq;
using System;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Text.RegularExpressions;
using System.Text;

namespace CloudClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>

    public partial class MainWindow : Window

    { 
        private LoginView loginView;
        private RegisterView registerView;
        public MainWindow()
        {
            InitializeComponent();
            src.Connection.Socket.ConnectToServer("127.0.0.1", 54000);
            loginView = new LoginView(LoginButton_Click, SwitchToRegisterView);
            registerView = null;
            this.mainGrid.Children.Add(loginView);
        }
        private void BorderMouse_Down(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                DragMove();
            }
        }

        private void MinimizeButton_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.MainWindow.WindowState = WindowState.Minimized;
        }

        private void MaximizeButton_Click(object sender, RoutedEventArgs e)
        {
            if (Application.Current.MainWindow.WindowState != WindowState.Maximized)
            {
                Application.Current.MainWindow.WindowState = WindowState.Maximized;
            }
            else
            {
                Application.Current.MainWindow.WindowState = WindowState.Normal;
            }
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void SwitchToRegisterView(object sender, RoutedEventArgs e)
        {
            mainGrid.Children.Clear();
            registerView = new RegisterView(SwitchToLoginView, RegisterButton_Click );
            mainGrid.Children.Add(registerView);
        }

        private void SwitchToLoginView(object sender, RoutedEventArgs e)
        {
            mainGrid.Children.Clear();
            loginView = new LoginView(LoginButton_Click, SwitchToRegisterView);
            mainGrid.Children.Add(loginView);
        }

        private void SwitchToMainView(object sender, RoutedEventArgs e)
        {
            mainGrid.Children.Clear();
        }

        private void LoginButton_Click(object sender, RoutedEventArgs e)
        {
            byte[] bytes = new byte[this.loginView.getUsernameInput().Length + this.loginView.getPasswordInput().Length + 1];
            byte[] usernameBytes = Encoding.ASCII.GetBytes(this.loginView.getUsernameInput());
            byte[] passwordBytes = Encoding.ASCII.GetBytes(this.loginView.getPasswordInput());

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

            MessageBox.Show(answer, "Message");
        }
        private void VerifyUserInput()
        {
            /*
             * This function verifies if the text in the text boxes, respectively in the password boxes
             * has been inputed correctly by the user.
             * Returns true if the input is correct, false otherwise
             */

            //The text boxes/ password boxes cannot be empty
            if (this.registerView.getUsernameInput() == "")
                throw new Exception("Fields cannot be left empty!");

            if (this.registerView.getPasswordInput() == "")
                throw new Exception("Fields cannot be left empty!");

            if (this.registerView.getSecondPasswordInput() == "")
                throw new Exception("Fields cannot be left empty!");

            if (this.registerView.getEmailInput() == "")
                throw new Exception("Fields cannot be left empty!");

            //Check the password box and confirm password box contain the same text
            if (this.registerView.getPasswordInput() != this.registerView.getSecondPasswordInput())
                throw new Exception("Password and confirm password do not match!");

            //Check if the email is in the correct format
            Regex emailRegex = new Regex("^([a-zA-Z])([a-zA-Z0-9]|\\.|)+@([a-z]|[A-Z])+\\.([a-zA-Z])+");
            Match match = emailRegex.Match(this.registerView.getEmailInput());

            if (match.Success == false)
                throw new Exception("Email is not in a correct format!");
        }
        private void RegisterButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                VerifyUserInput();
            }
            catch (Exception exp)
            {
               // this.lbl_result.Content = exp.Message;
                return;
            }

            //Send the information to the server and check it's response
            string message_data = this.registerView.getUsernameInput() + " " + this.registerView.getPasswordInput() + " " + this.registerView.getEmailInput();
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
            MessageBox.Show(resp, "Message");
        }
    }
}
