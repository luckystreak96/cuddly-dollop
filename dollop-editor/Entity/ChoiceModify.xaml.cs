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

        Dictionary<string, Dictionary<string, string>> Strings;

        public ChoiceModify(Choice choice)
        {
            InitializeComponent();
            Choice_ = choice;
            txtID.Text = choice.d.ToString();
            txtNext.Text = choice.next.ToString();
            cmbText.Text = choice.text;
            //txtText.Text = choice.text;

            SetLocalizationCmb();
            UpdateQueueButton();
        }

        private void UpdateQueueButton()
        {
            int count = Choice_ != null && Choice_.queues != null ? Choice_.queues.Count : 0;
            btnQueue.Content = "Queues (" + count + ")";
        }

        private void SetLocalizationCmb()
        {
            Strings = LocalizationLoader.LoadLocalization();
            cmbText.ItemsSource = new List<String>(Strings.Keys);
            cmbText.Items.Refresh();

            cmbLanguage.ItemsSource = LocalizationLoader.Languages;
            cmbLanguage.SelectedValue = "english";
            cmbLanguage.Items.Refresh();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            Choice_.d = int.Parse(txtID.Text);
            Choice_.next = int.Parse(txtNext.Text);
            Choice_.text = cmbText.Text;// txtText.Text;
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

            UpdateQueueButton();
        }

        private void cmbText_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            SetPreviewText(cmbText.Text, cmbLanguage.Text);
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

        private void cmbText_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string val = e.AddedItems.Count > 0 ? e.AddedItems[0].ToString() : "";
            SetPreviewText(val, cmbLanguage.Text);
        }

        private void cmbLanguage_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            SetPreviewText(cmbText.Text, cmbLanguage.Text);
        }

        private void cmbLanguage_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string val = e.AddedItems.Count > 0 ? e.AddedItems[0].ToString() : "";
            SetPreviewText(cmbText.Text, val);
        }

        private void btnEditLocalizations_Click(object sender, RoutedEventArgs e)
        {
            Localization window = new Localization();
            window.cmbText.Text = cmbText.Text;
            window.ShowDialog();

            string cmbLocValue = cmbText.Text;
            string txtPrevValue = txtPreview.Text;
            string cmbLang = cmbLanguage.Text;

            Strings = LocalizationLoader.LoadLocalization();
            SetLocalizationCmb();

            if (Strings.ContainsKey(cmbLocValue))
            {
                cmbText.Text = cmbLocValue;
                cmbLanguage.Text = cmbLang;

                if (Strings[cmbLocValue].ContainsKey(cmbLang))
                    txtPreview.Text = Strings[cmbLocValue][cmbLang];
            }
        }
    }
}
