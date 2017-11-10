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
    /// Interaction logic for AddQueue.xaml
    /// </summary>
    public partial class QueueWindow : Window
    {
        public EventQueue Queue { get; set; }

        public QueueWindow()
        {
            InitializeComponent();
            Queue = new EventQueue();

            FillComboBox();

            txtID.Text = Queue.id.ToString();
            txtFlag.Text = 1.ToString();
            txtName.Text = "queue";
        }

        private void FillComboBox()
        {
            foreach (var val in Enum.GetValues(typeof(EventActivation)))
                cmbActivation.Items.Add(val.ToString());

            cmbActivation.Items.Refresh();
        }

        public QueueWindow(EventQueue eventQueue)
        {
            InitializeComponent();
            Queue = eventQueue;

            FillComboBox();
            foreach (var x in cmbActivation.Items)
                if (Queue.activation == x.ToString())
                    cmbActivation.SelectedItem = x;

            txtID.Text = Queue.id.ToString();
            txtFlag.Text = Queue.flag.ToString();
            txtName.Text = Queue.name;
            chkRepeat.IsChecked = Queue.repeating;
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            Save();
        }

        private void Save()
        {
            bool success = true;
            try
            {
                Queue.id = int.Parse(txtID.Text);
                Queue.flag = int.Parse(txtFlag.Text);
                Queue.name = txtName.Text;
                Queue.repeating = (bool)chkRepeat.IsChecked;
                Queue.activation = cmbActivation.SelectedItem.ToString();
            }
            catch (Exception ex)
            {
                success = false;
                MessageBox.Show(ex.Message);
            }

            if(success)
            {
                DialogResult = true;
                Close();
            }
        }

        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                Save();
        }
    }
}
