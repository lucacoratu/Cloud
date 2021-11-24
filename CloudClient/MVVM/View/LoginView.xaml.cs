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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace CloudClient.MVVM.View
{
    /// <summary>
    /// Interaction logic for LoginView.xaml
    /// </summary>
    public partial class LoginView : UserControl
    {
        public LoginView()
        {
            InitializeComponent();
        }

        public LoginView(System.Windows.RoutedEventHandler action1, System.Windows.RoutedEventHandler action2)
        {
            InitializeComponent();
            this.LoginButton.Click += action1;
            this.RegisterButton.Click += action2;
        }

        public string getUsernameInput()
        {
            return this.usernameInput.Text;
        }

        public string getPasswordInput()
        {
            return this.passwordInput.Password;
        }
        
    }
}
