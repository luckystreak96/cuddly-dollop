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
    /// Interaction logic for DialogueModify.xaml
    /// </summary>
    public partial class DialogueModify : Window
    {
        Dictionary<string, Dictionary<string, string>> Strings;

        public Dialogue Dialogue_ { get; set; }
        public enum DialogueTypes { simple, choice, end }

        private DialogueModify()
        {
            InitializeComponent();
            UpdateQueueButton();
        }

        private void SetCmb()
        {
            foreach (DialogueTypes val in Enum.GetValues(typeof(DialogueTypes)))
                cmbType.Items.Add(val.ToString());

            try
            {
                cmbType.SelectedValue = Dialogue_.type;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            SetLocalizationCmb();
        }

        private void SetLocalizationCmb()
        {
            Strings = LocalizationLoader.LoadLocalization();
            cmbLoc.ItemsSource = new List<String>(Strings.Keys);
            cmbLoc.Items.Refresh();

            cmbLanguage.ItemsSource = LocalizationLoader.Languages;
            cmbLanguage.SelectedValue = "english";
            cmbLanguage.Items.Refresh();
        }

        public DialogueModify(Dialogue dialogue)
        {
            InitializeComponent();
            Dialogue_ = dialogue;
            SetCmb();
            txtID.Text = dialogue.id.ToString();
            txtNext.Text = dialogue.next.ToString();
            txtText.Text = dialogue.text;
            UpdateQueueButton();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            Dialogue_.id = int.Parse(txtID.Text);
            Dialogue_.next = int.Parse(txtNext.Text);
            Dialogue_.text = txtText.Text;
            Dialogue_.type = cmbType.Text;
            Close();
        }

        private void btnQueue_Click(object sender, RoutedEventArgs e)
        {
            EntityWindow entityWindow = new EntityWindow(Dialogue_.queues);
            if (entityWindow.ShowDialog() == true)
                Dialogue_.queues = entityWindow.Entity_.queues;

            UpdateQueueButton();
        }

        private void cmbLoc_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string val = e.AddedItems.Count > 0 ? (string)e.AddedItems[0] : (string)cmbLoc.SelectedValue;
            txtText.Text = val;
            SetPreviewText(val, cmbLanguage.Text);
        }

        // cmbLanguage
        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string val = e.AddedItems.Count > 0 ? e.AddedItems[0].ToString() : "";
            SetPreviewText(cmbLoc.Text, val);
        }

        private void SetPreviewText(string key, string language)
        {
            try
            {
                txtPreview.Text = "";
                if (Strings.ContainsKey(key))
                    if (Strings[key].ContainsKey(language))
                        txtPreview.Text = Strings[key][language];
            }
            catch (Exception ex)
            {
                txtPreview.Text = ex.Message;
            }
        }

        private void cmbLoc_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            SetPreviewText(txtText.Text, cmbLanguage.Text);
        }

        private void btnEditLocalizations_Click(object sender, RoutedEventArgs e)
        {
            Localization window = new Localization();
            window.cmbText.Text = cmbLoc.Text;
            window.ShowDialog();

            string cmbLocValue = cmbLoc.Text;
            string txtPrevValue = txtPreview.Text;
            string cmbLang = cmbLanguage.Text;

            Strings = LocalizationLoader.LoadLocalization();
            SetLocalizationCmb();

            if (Strings.ContainsKey(cmbLocValue))
            {
                cmbLoc.Text = cmbLocValue;
                txtText.Text = cmbLocValue;

                cmbLanguage.Text = cmbLang;

                if(Strings[cmbLocValue].ContainsKey(cmbLang))
                    txtPreview.Text = Strings[cmbLocValue][cmbLang];
            }
        }

        private void UpdateQueueButton()
        {
            int count = Dialogue_ != null && Dialogue_.queues != null ? Dialogue_.queues.Count : 0;
            btnQueue.Content = "Queues (" + count + ")";
        }
    }
}
