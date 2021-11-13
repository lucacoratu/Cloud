using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
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


namespace CloudClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            src.Connection.Socket.ConnectToServer("127.0.0.1", 54000);
        }

        private void TextBlock_Click(object sender, RoutedEventArgs e)
        {

        }

        private void btn_login_Click(object sender, RoutedEventArgs e)
        {
            byte[] bytes = new byte[this.txtbox_username.Text.Length + this.txtbox_password.Text.Length + 1];
            byte[] usernameBytes = Encoding.ASCII.GetBytes(this.txtbox_username.Text);
            byte[] passwordBytes = Encoding.ASCII.GetBytes(this.txtbox_password.Text);

            for(int i =0; i < usernameBytes.Length; i++)
            {
                bytes[i] = usernameBytes[i]; 
            }
            bytes[usernameBytes.Length] = (byte)32;

            for(int i = usernameBytes.Length + 1; i < usernameBytes.Length + passwordBytes.Length + 1; i++)
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
    }
}
