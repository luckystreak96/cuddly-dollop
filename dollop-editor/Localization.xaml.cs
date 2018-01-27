using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
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
    /// Interaction logic for Localization.xaml
    /// </summary>
    public partial class Localization : Window
    {
        Dictionary<string, Dictionary<string, string>> Strings;
        string path = "../../../Test/res/data/strings/localization.json";

        public Localization()
        {
            InitializeComponent();
            string data;
            if (File.Exists(path))
                data = File.ReadAllText(path);
            else
                data = "";
            Strings = JsonConvert.DeserializeObject<Dictionary<string, Dictionary<string, string>>>(data);
            if (Strings == null)
                Strings = new Dictionary<string, Dictionary<string, string>>();
            // Load string from file

            UpdateCmb();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            // Save the stuff to the json file
            File.WriteAllText(path, JsonConvert.SerializeObject(Strings, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore }).Replace("\r", "").Replace("\\r", ""));
            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void btnUpdate_Click(object sender, RoutedEventArgs e)
        {
            // Modify the selected string
            if (!Strings.ContainsKey(txtEnglish.Text))
            {
                Strings.Remove((string)cmbText.SelectedItem);
                Strings.Add(txtEnglish.Text, CreateList());
            }
            else
                Strings[txtEnglish.Text] = CreateList();

            UpdateCmb();
        }

        private void btnNew_Click(object sender, RoutedEventArgs e)
        {
            // Add a new string
            if (!Strings.ContainsKey(txtEnglish.Text))
                Strings.Add(txtEnglish.Text, CreateList());
            else
                Strings[txtEnglish.Text] = CreateList();

            UpdateCmb();
        }

        private void UpdateCmb()
        {
            cmbText.ItemsSource = new List<String>(Strings.Keys);
            cmbText.Items.Refresh();
        }

        private Dictionary<string, string> CreateList()
        {
            return new Dictionary<string, string>() { { "french", txtFrench.Text } };
        }

        private void cmbText_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string key = (string)cmbText.SelectedItem;
            if (key == null || !Strings.ContainsKey(key))
                return;
            txtEnglish.Text = key;
            txtFrench.Text = Strings[key]["french"];
        }
    }
}
