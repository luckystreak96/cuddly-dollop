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
using System.Windows.Shapes;

namespace dollop_editor
{
    /// <summary>
    /// Interaction logic for ChoiceModify.xaml
    /// </summary>
    public partial class ChoiceModify : Window
    {
        public Choice Choice_ { get; set; }

        public ChoiceModify(Choice choice)
        {
            InitializeComponent();
            Choice_ = choice;
            txtID.Text = choice.d.ToString();
            txtNext.Text = choice.next.ToString();
            txtText.Text = choice.text;
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            Choice_.d = int.Parse(txtID.Text);
            Choice_.next = int.Parse(txtNext.Text);
            Choice_.text = txtText.Text;
            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void btnQueue_Click(object sender, RoutedEventArgs e)
        {
            EntityWindow entityWindow = new EntityWindow(Choice_.queues);
            if (entityWindow.ShowDialog() == true)
                Choice_.queues = entityWindow.Entity_.queues;
        }
    }
}
