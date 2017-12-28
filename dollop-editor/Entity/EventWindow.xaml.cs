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
                else if (cmbType.SelectedIndex == -1)
                    cmbType.SelectedIndex = 0;
            }

            foreach (EventExecution val in Enum.GetValues(typeof(EventExecution)))
            {
                cmbExecution.Items.Add(val.ToString());
                if (Event != null && Event.execution_type == val.ToString())
                    cmbExecution.SelectedItem = val.ToString();
                else if (cmbExecution.SelectedIndex == -1)
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
                if (Event.args.ContainsKey(txtKey.Text))
                {
                    MessageBox.Show("Key already exists");
                    return;
                }
                AddArg();
            }
            lstArgs.Items.Refresh();
        }

        private void AddArg()
        {
            try
            {
                if (chkJsonObject.IsChecked == true)
                    Event.args.Add(txtKey.Text, JsonConvert.DeserializeObject(txtArg.Text));
                else
                    Event.args.Add(txtKey.Text, TryBasicParse(txtArg.Text));
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
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

        private void cmbArgKey_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (cmbArgKey.SelectedItem == null)
                return;
            txtKey.Text = cmbArgKey.SelectedItem.ToString();
            if (cmbType.SelectedItem.ToString() == "dialogue")
            {
                if (cmbArgKey.SelectedItem.ToString() == "dialogue")
                {
                    string text = string.Join("\n", @"{
                                    ""id"": 0,
                                    ""text"": ""Hey man, what's up? I wrote a poem today."",
                                    ""next"": 1,
                                    ""type"": ""simple choice end""
                                }".Split('\n').Select(s => s.Trim()));
                    txtArg.Text = text;
                }
                else if (cmbArgKey.SelectedItem.ToString() == "choice")
                {
                    string text = string.Join("\n", @"{
                                            ""d"": 0,
                                            ""text"": ""Can you walk down for me?"",
                                            ""next"": 2

                                            ,""queues"": [{
                                                    ""id"": 2,
                                                    ""repeating"": false,
                                                    ""events"": [{
                                                            ""type"": ""move_down"",
                                                            ""execution_type"": ""blocking"",
                                                            ""args"": {
                                                                ""id"": 2,
                                                                ""distance"": 3.0
                                                        }}]}]

                                        }".Split('\n').Select(s => s.Trim()));
                    txtArg.Text = text;
                }
                chkJsonObject.IsChecked = true;
            }

        }

        private void cmbType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if(e.AddedItems.Contains("dialogue"))
            {
                SaveAndClose();
            }
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
                result = int.Parse(text);
                return result;
            }
            catch { }
            try
            {
                result = float.Parse(text);
                return result;
            }
            catch { }

            if (txtArg.Text == "true")
                return true;
            if (txtArg.Text == "false")
                return false;
            return text;
        }

        private void lstArgs_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (lstArgs.SelectedIndex >= 0 && lstArgs.SelectedIndex < lstArgs.Items.Count)
            {
                _currentSelectionKey = ((KeyValuePair<string, object>)lstArgs.SelectedItem).Key;
                txtKey.Text = _currentSelectionKey;

                foreach (var x in cmbArgKey.Items)
                {
                    if (x.ToString() == _currentSelectionKey)
                        cmbArgKey.SelectedItem = x;
                }

                txtArg.Text = ((KeyValuePair<string, object>)lstArgs.SelectedItem).Value.ToString();
                _modifyMode = true;
                SetSaveButtonText();

                chkJsonObject.IsChecked = cmbType.SelectedItem.ToString() == "dialogue";
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
            SaveAndClose();
        }

        private void SaveAndClose()
        {
            Event.type = cmbType.SelectedItem.ToString();
            Event.execution_type = cmbExecution.SelectedItem.ToString();
            DialogResult = true;
            Close();
        }
    }
}
