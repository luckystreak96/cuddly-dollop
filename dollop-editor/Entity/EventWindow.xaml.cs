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
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace dollop_editor
{
    /// <summary>
    /// Interaction logic for EventWindow.xaml
    /// </summary>
    public partial class EventWindow : Window
    {
        public Event Event { get; set; }

        private bool _modifyMode = false;
        private string _currentSelectionKey = "";

        public EventWindow()
        {
            Event = new Event(1);
            InitializeComponent();
            SetupComboBoxes();
            SetupArgs();
        }

        public EventWindow(Event ev)
        {
            Event = ev;
            InitializeComponent();
            SetupComboBoxes();
            SetupArgs();
        }

        private void SetupComboBoxes()
        {
            foreach (EventType val in Enum.GetValues(typeof(EventType)))
            {
                cmbType.Items.Add(val.ToString());
                if (Event != null && Event.type == val.ToString())
                    cmbType.SelectedItem = val.ToString();
                else
                    cmbType.SelectedIndex = 0;
            }

            foreach (EventExecution val in Enum.GetValues(typeof(EventExecution)))
            {
                cmbExecution.Items.Add(val.ToString());
                if (Event != null && Event.execution_type == val.ToString())
                    cmbExecution.SelectedItem = val.ToString();
                else
                    cmbExecution.SelectedIndex = 0;
            }
        }

        private void SetupArgs()
        {
            if (Event == null)
                return;

            lstArgs.ItemsSource = Event.args;
        }

        private void lstArgs_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (lstArgs.SelectedIndex >= 0 && lstArgs.SelectedIndex < lstArgs.Items.Count)
            {
                _currentSelectionKey = ((KeyValuePair<string, object>)lstArgs.SelectedItem).Key;
                txtKey.Text = _currentSelectionKey;
                txtArg.Text = JsonConvert.SerializeObject(lstArgs.SelectedItem);
                _modifyMode = true;
                SetSaveButtonText();
            }
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            if(_modifyMode)
            {
                Event.args.Remove(_currentSelectionKey);
                Event.args.Add(txtKey.Text, JsonConvert.DeserializeObject(txtArg.Text));
            }
            else
            {
                Event.args.Add(txtKey.Text, JsonConvert.DeserializeObject(txtArg.Text));
            }
        }

        private void btnAddArg_Click(object sender, RoutedEventArgs e)
        {
            _modifyMode = false;
            SetSaveButtonText();
        }

        private void SetSaveButtonText()
        {
            if (_modifyMode)
                btnSave.Content = "Modify Arg";
            else
                btnSave.Content = "Create New Arg";
        }
    }
}
