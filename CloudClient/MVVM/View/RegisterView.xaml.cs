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
    /// Interaction logic for RegisterView.xaml
    /// </summary>
    public partial class RegisterView : UserControl
    {

        public RegisterView()
        {
            InitializeComponent();
        }

        public RegisterView(System.Windows.RoutedEventHandler action1, System.Windows.RoutedEventHandler action2)
        {
            InitializeComponent();
            this.BackToLoginButton.Click += action1;
            this.SendRequestButton.Click += action2;
        }

        public string getUsernameInput()
        {
            return this.UsernameInput.Text;
        }

        public string getEmailInput()
        {
            return this.EmailInput.Text;
        }

        public string getPasswordInput()
        {
            return this.PasswordInput.Password;
        }

        public string getSecondPasswordInput()
        {
            return this.SecondPasswordInput.Password;
        }


    }
}
