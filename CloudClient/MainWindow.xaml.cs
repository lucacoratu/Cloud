using CloudClient.MVVM.View;
using System;
using System.Collections.Generic;
using System.Linq;
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
using CloudClient.src;

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
            string answer = ServerAPI.LoginIntoAccount(this.loginView.usernameInput.Text, this.loginView.passwordInput.Password);
            MessageBox.Show(answer);
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

            string answer = ServerAPI.RegisterAccount(this.registerView.UsernameInput.Text, this.registerView.PasswordInput.Password, this.registerView.EmailInput.Text);
            MessageBox.Show(answer);
        }
    }
}
