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
using System.Text.RegularExpressions;

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
            Event = new Event();
            InitializeComponent();
            SetupComboBoxes();
            SetupArgs();
            SetSaveButtonText();
        }

        public EventWindow(Event ev)
        {
            Event = ev;
            InitializeComponent();
            SetupComboBoxes();
            SetupArgs();
            SetSaveButtonText();
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

            cmbArgKey.Items.Clear();
            foreach (var val in Enum.GetValues(EventCompletionHelper.eventArgDictionary[cmbType.SelectedIndex]))
            {
                cmbArgKey.Items.Add(val.ToString());
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

        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            if (Event.args.ContainsKey(txtKey.Text))
            {
                MessageBox.Show("Key already exists");
                return;
            }
            if (_modifyMode)
            {
                Event.args.Remove(_currentSelectionKey);
                AddArg();
                txtKey.Text = "";
                txtArg.Text = "";
                _modifyMode = false;
                SetSaveButtonText();
            }
            else
            {
                AddArg();
            }
            lstArgs.Items.Refresh();
        }

        private void AddArg()
        {
            if (chkJsonObject.IsChecked == true)
                Event.args.Add(txtKey.Text, JsonConvert.DeserializeObject(txtArg.Text));
            else
                Event.args.Add(txtKey.Text, TryBasicParse(txtArg.Text));
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

        private void cmbArgKey_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cmbArgKey.SelectedItem == null)
                return;
            txtKey.Text = cmbArgKey.SelectedItem.ToString();
            if (EventCompletionHelper.eventArgComplexExamples.ContainsKey(cmbType.SelectedItem.ToString()))
            {
                string trimmedByLine = string.Join(
                             "\n",
                             EventCompletionHelper.eventArgComplexExamples[cmbType.SelectedItem.ToString()].Split('\n').Select(s => s.Trim()));
                txtArg.Text = trimmedByLine;
            }
        }

        private void cmbType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            cmbArgKey.Items.Clear();
            if (EventCompletionHelper.eventArgDictionary.ContainsKey(cmbType.SelectedIndex))
                foreach (var val in Enum.GetValues(EventCompletionHelper.eventArgDictionary[cmbType.SelectedIndex]))
                {
                    cmbArgKey.Items.Add(val.ToString());
                }
        }

        private object TryBasicParse(string text)
        {
            object result;
            try
            {
                result = float.Parse(text);
                return result;
            }
            catch { }
            try
            {
                result = int.Parse(text);
                return result;
            }
            catch { }

            if (txtArg.Text == "true")
                return true;
            if (txtArg.Text == "false")
                return true;
            return text;
        }

        private void lstArgs_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (lstArgs.SelectedIndex >= 0 && lstArgs.SelectedIndex < lstArgs.Items.Count)
            {
                _currentSelectionKey = ((KeyValuePair<string, object>)lstArgs.SelectedItem).Key;
                txtKey.Text = _currentSelectionKey;
                txtArg.Text = ((KeyValuePair<string, object>)lstArgs.SelectedItem).Value.ToString();
                _modifyMode = true;
                SetSaveButtonText();
            }
        }

        private void btnRemoveArg_Click(object sender, RoutedEventArgs e)
        {
            if (lstArgs.SelectedIndex >= 0 && lstArgs.SelectedIndex < lstArgs.Items.Count)
            {
                string key = ((KeyValuePair<string, object>)lstArgs.SelectedItem).Key;
                Event.args.Remove(key);
                lstArgs.Items.Refresh();
            }
        }

        private void btnCancelEvent_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }

        private void btnSaveEvent_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Close();
        }
    }
}
