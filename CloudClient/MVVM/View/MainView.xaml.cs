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
    /// Interaction logic for MainView.xaml
    /// </summary>
    public partial class MainView : UserControl
    {
        private string currentDirectory;
        private string fullPath;
        public MainView()
        {
            InitializeComponent();
            currentDirectory = "Home";
            fullPath = "";
        }

        //private void homeDirectory_Expanded(object sender, RoutedEventArgs e)
        //{
        //    var content = src.ServerAPI.ViewDirectoryContents("current");
        //    List<Expander> directories = new List<Expander>();
        //    List<Label> labels = new List<Label>();

        //    foreach(var item in content)
        //    {
        //        if(item.type == "file")
        //        {
        //            Label file = new Label();
        //            file.Content = item.filename;
        //            labels.Add(file);
        //        }
        //        else
        //        {
        //            Expander directory = new Expander();
        //            directory.Header = item.filename;
        //            directory.Expanded += DirectoryExpanded;
        //            directories.Add(directory);
        //        }
        //    }

        //    StackPanel stackPanel = new StackPanel();
        //    stackPanel.Margin = new Thickness(10, 0, 0, 0);
            
        //    foreach(var directory in directories)
        //        stackPanel.Children.Add(directory);

        //    foreach(var label in labels)    
        //        stackPanel.Children.Add(label);

        //    homeDirectory.Content = stackPanel;
        //}

        private void DirectoryExpanded(object sender, RoutedEventArgs e)
        {
            List<src.FileDetails> content = null;
            var variable = sender as Expander;
           

            if (variable.Header as string == "Home")
            {
                content = src.ServerAPI.ViewDirectoryContents(currentDirectory);
            }
            else
            {
                src.ServerAPI.ChangeDirectory(variable.Header as string);
                content = src.ServerAPI.ViewDirectoryContents("current");
                currentDirectory += "_" + variable.Header as string;
                fullPath = "";
                fullPath = fullPath.Insert(0, variable.Header as string);
            }
            if (variable.Name != currentDirectory)
            {
                fullPath = fullPath.Insert(0, variable.Header as string);
                return;
            }

            List<Expander> directories = new List<Expander>();
            List<Label> labels = new List<Label>();

            foreach (var item in content)
            {
                if (item.type == "file")
                {
                    Label file = new Label();
                    file.Content = item.filename;
                    labels.Add(file);
                }
                else
                {
                    Expander directory = new Expander();
                    directory.Name = currentDirectory + "_" + item.filename;
                    directory.Header = item.filename;
                    directory.Expanded += DirectoryExpanded;
                    //directory.Collapsed += DirectoryCollapsed;
                    directories.Add(directory);
                    
                }
            }

            StackPanel stackPanel = new StackPanel();
            stackPanel.Margin = new Thickness(10, 0, 0, 0);

            foreach (var directory in directories)
                stackPanel.Children.Add(directory);

            foreach (var label in labels)
                stackPanel.Children.Add(label);

            var Variable = sender as Expander;
            Variable.Content = stackPanel;
        }

        //private void DirectoryCollapsed(object sender, RoutedEventArgs e)
        //{
            //Expander expander = sender as Expander;
            //StackPanel stackPanel = expander.Content as StackPanel;
            //var children = stackPanel.Children;
            //foreach (var child in children)
            //{
            //    if (child.GetType() == typeof(Expander))
            //    {
            //        Expander cast = child as Expander;
            //        if (cast.IsExpanded)
            //        {
            //            DirectoryCollapsed(cast, e);
            //            src.ServerAPI.ChangeDirectory("..");
            //            string[] tokens = currentDirectory.Split('_');
            //            currentDirectory = "";
            //            for(int i = 0; i < tokens.Length - 1; i++)
            //            {
            //                currentDirectory += tokens[i];
            //                if (i != tokens.Length - 2)
            //                    currentDirectory += '_';
            //            }
            //            cast.Content = null;
            //        }
            //    }
            //}
        //}
    }
}