﻿using Newtonsoft.Json;
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

        public Localization()
        {
            InitializeComponent();
            Strings = LocalizationLoader.LoadLocalization();
            // Load string from file

            UpdateCmb();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            // Save the stuff to the json file
            try
            {
                File.WriteAllText(LocalizationLoader.path, JsonConvert.SerializeObject(Strings, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore }).Replace("\r", "").Replace("\\r", ""));
                Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Couldn't write to file due to error: " + ex.Message);
            }
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void btnUpdate_Click(object sender, RoutedEventArgs e)
        {
            // Modify the selected string
            try
            {
                if (!Strings.ContainsKey(txtKey.Text))
                {
                    Strings.Remove((string)cmbText.SelectedItem);
                    Strings.Add(txtKey.Text, CreateList());
                }
                else
                    Strings[txtKey.Text] = CreateList();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Couldn't update localization due to error: " + ex.Message);
            }

            UpdateCmb();
        }

        private void btnNew_Click(object sender, RoutedEventArgs e)
        {
            AddNewLoc();
        }

        private void AddNewLoc()
        {
            // Add a new string
            try
            {
                if (!Strings.ContainsKey(txtKey.Text))
                    Strings.Add(txtKey.Text, CreateList());
                else
                    Strings[txtKey.Text] = CreateList();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Couldn't create new localization due to error: " + ex.Message);
            }

            UpdateCmb();
        }

        private void UpdateCmb()
        {
            cmbText.ItemsSource = new List<String>(Strings.Keys);
            cmbText.Items.Refresh();
        }

        private Dictionary<string, string> CreateList()
        {
            return new Dictionary<string, string>() { { "french", txtFrench.Text }, { "english", txtEnglish.Text } };
        }

        private void cmbText_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                string key = (string)cmbText.SelectedItem;
                if (key == null || !Strings.ContainsKey(key))
                    return;

                ClearAllTextBoxes();

                // Write the localizations if they exist
                if (Strings[key].ContainsKey("english"))
                    txtEnglish.Text = Strings[key]["english"];

                if (Strings[key].ContainsKey("french"))
                    txtFrench.Text = Strings[key]["french"];

                txtKey.Text = key;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Could not load localization due to error:" + ex.Message);
            }
        }

        private void ClearAllTextBoxes()
        {
            txtEnglish.Text = "";
            txtFrench.Text = "";
            txtKey.Text = "";
        }

        private void btnRename_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (cmbText.Text == "")
                    return;

                if (!Strings.ContainsKey(cmbText.Text))
                    return;

                Strings.Remove(cmbText.Text);
                AddNewLoc();
                cmbText.Text = txtKey.Text;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnDeleteKey_Click(object sender, RoutedEventArgs e)
        {
            if (cmbText.Text == "")
                return;

            if (Strings.ContainsKey(cmbText.Text))
            {
                if (chkConfirm.IsChecked == false || MessageBox.Show("Are you sure you want to delete \"" + cmbText.Text + "\"?", "Delete " + cmbText.Text + "?", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
                    Strings.Remove(cmbText.Text);
            }

            txtEnglish.Text = "";
            txtFrench.Text = "";
            txtKey.Text = "";
            cmbText.Text = "";
            UpdateCmb();
        }
    }
}
