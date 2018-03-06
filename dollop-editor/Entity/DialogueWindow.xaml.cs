using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace dollop_editor
{
    /// <summary>
    /// Interaction logic for DialogueWindow.xaml
    /// </summary>
    public partial class DialogueWindow : Window
    {
        public Event Event_ { get; set; }
        public string CurrentLang { get; set; }
        public Dictionary<string, List<Dialogue>> Dialogues { get; set; }
        public Dictionary<string, List<Choice>> Choices { get; set; }
        private Dictionary<string, HashSet<int>> DialogueID = new Dictionary<string, HashSet<int>>();
        private enum Languages { english, french }
        private List<string> langList;

        public DialogueWindow()
        {
            InitializeComponent();
            SetupDictionaries();
            lstDialogues.ItemsSource = Dialogues["english"];
            cmbLanguage.SelectedValue = "english";
        }

        private void SetupDictionaries()
        {
            CurrentLang = "english";
            Dialogues = new Dictionary<string, List<Dialogue>>();
            Choices = new Dictionary<string, List<Choice>>();

            langList = Enum.GetNames(typeof(Languages)).ToList();
            cmbLanguage.ItemsSource = langList;

            foreach(string s in langList)
            {
                DialogueID.Add(s, new HashSet<int>());
                Dialogues.Add(s, new List<Dialogue>());
                Choices.Add(s, new List<Choice>());
            }
        }

        public DialogueWindow(Event ev)
        {
            InitializeComponent();
            SetupDictionaries();
            lstDialogues.ItemsSource = Dialogues["english"];
            cmbLanguage.SelectedValue = "english";
            Event_ = ev;

            if (Event_.args.Count == 0)
                return;

            // Json stuffs goin on - parse
            try
            {
                bool found = false;
                foreach (string language in cmbLanguage.Items)
                {
                    if (Event_.args.First().Key == language)
                    {
                        found = true;

                        // Make sure the lists exist
                        if (!Dialogues.ContainsKey(language))
                        {
                            Dialogues.Add(language, new List<Dialogue>());
                            DialogueID.Add(language, new HashSet<int>());
                            Choices.Add(language, new List<Choice>());
                        }
                        break;
                    }
                }
                // the languages are defined
                if (Event_.args.Count > 0)
                {
                    if (found)
                    {
                        // Load the languages
                        foreach (var o in Event_.args)
                        {
                            JObject jObject = o.Value as JObject;
                            Dictionary<string, object> dict = jObject.ToObject<Dictionary<string, object>>();
                            // handle language load
                            foreach (var s in dict)
                            {
                                if (s.Key.Contains("dialogue"))
                                {
                                    jObject = s.Value as JObject;
                                    Dialogue d = jObject.ToObject<Dialogue>();
                                    Dialogues[o.Key].Add(d);
                                    DialogueID[o.Key].Add(d.id);
                                }
                                else if (s.Key.Contains("choice"))
                                {
                                    jObject = s.Value as JObject;
                                    Choices[o.Key].Add(jObject.ToObject<Choice>());
                                }
                            }
                        }
                    }
                    else
                    {
                        // handle non-language load
                        foreach (var o in Event_.args)
                        {
                            if (o.Key.Contains("dialogue"))
                            {
                                JObject jObject = o.Value as JObject;
                                Dialogue d = jObject.ToObject<Dialogue>();
                                Dialogues["english"].Add(d);
                                DialogueID["english"].Add(d.id);
                            }
                            else if (o.Key.Contains("choice"))
                            {
                                JObject jObject = o.Value as JObject;
                                Choices["english"].Add(jObject.ToObject<Choice>());
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            lstDialogues.Items.Refresh();
        }

        // Save and exit
        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                DialogResult = true;
                Dictionary<string, object> languages = new Dictionary<string, object>();

                foreach (var langDics in Dialogues)
                {
                    string temp;
                    JObject jObject;

                    // Dont save an empty dialogue
                    if (Dialogues[langDics.Key].Count == 0)
                        continue;
                    Dictionary<string, object> dictionary = new Dictionary<string, object>();
                    foreach (var x in Dialogues[langDics.Key])
                    {
                        temp = JsonConvert.SerializeObject(x, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
                        jObject = JObject.Parse(temp);
                        try
                        {
                            dictionary.Add("dialogue " + x.id, jObject);
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show(ex.Message);
                            return;
                        }
                    }

                    int counter = 0;
                    foreach (var x in Choices[langDics.Key])
                    {
                        temp = JsonConvert.SerializeObject(x, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
                        jObject = JObject.Parse(temp);
                        dictionary.Add("choice " + counter++, jObject);
                    }

                    // Convert the big dict to json
                    temp = JsonConvert.SerializeObject(dictionary, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
                    jObject = JObject.Parse(temp);
                    try
                    {
                        languages.Add(langDics.Key, jObject);
                    }
                    catch (Exception ex)
                    {
                        MessageBox.Show(ex.Message);
                        return;
                    }

                    //languages.Add(langDics.Key, dictionary);
                }

                Event_.args = languages;

                Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error saving : " + ex.Message);
            }
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }

        private void lstDialogues_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2 && lstDialogues.SelectedIndex >= 0 && lstDialogues.SelectedIndex < lstDialogues.Items.Count)
            {
                Dialogue dialogue = Dialogues[cmbLanguage.Text].ElementAt(lstDialogues.SelectedIndex);
                int temp = dialogue.id;

                DialogueModify dialogueModify = new DialogueModify(dialogue);
                dialogueModify.ShowDialog();

                if (dialogue.id != temp)
                {
                    DialogueID[cmbLanguage.Text].Remove(temp);
                    DialogueID[cmbLanguage.Text].Add(dialogue.id);
                }

                lstDialogues.Items.Refresh();
            }
        }

        private void lstDialogues_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (lstDialogues.SelectedIndex < 0 || lstDialogues.SelectedIndex >= lstDialogues.Items.Count)
            {
                btnAddChoice.IsEnabled = false;
                btnRemoveChoice.IsEnabled = false;
                return;
            }

            btnAddChoice.IsEnabled = true;

            Dialogue dialogue = Dialogues[cmbLanguage.Text].ElementAt(lstDialogues.SelectedIndex);
            List<Choice> list = new List<Choice>();
            foreach (Choice c in Choices[cmbLanguage.Text])
                if (c.d == dialogue.id)
                    list.Add(c);

            lstChoices.ItemsSource = list;
            lstChoices.Items.Refresh();
        }

        private void lstChoices_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2 && lstChoices.SelectedIndex >= 0 && lstChoices.SelectedIndex < lstChoices.Items.Count)
            {
                Choice choice = Choices[cmbLanguage.Text].ElementAt(lstChoices.SelectedIndex);

                ChoiceModify choiceModify = new ChoiceModify(choice);
                choiceModify.ShowDialog();

                lstChoices.Items.Refresh();
            }
        }

        private void btnAddDialogue_Click(object sender, RoutedEventArgs e)
        {
            Dialogue dialogue = new Dialogue();
            for (int i = 0; i < 999; i++)
                if (!DialogueID[cmbLanguage.Text].Contains(i))
                {
                    DialogueID[cmbLanguage.Text].Add(i);
                    dialogue.id = i;
                    break;
                }
            Dialogues[cmbLanguage.Text].Add(dialogue);

            lstDialogues.Items.Refresh();
        }

        private void btnAddChoice_Click(object sender, RoutedEventArgs e)
        {
            lstChoices.ItemsSource = Choices[cmbLanguage.Text];
            lstChoices.Items.Refresh();

            if (lstDialogues.SelectedItem == null)
                return;

            Choice choice = new Choice();
            choice.d = ((Dialogue)lstDialogues.SelectedItem).id;
            Choices[cmbLanguage.Text].Add(choice);

            ((Dialogue)lstDialogues.SelectedItem).type = "choice";

            lstChoices.Items.Refresh();
        }

        private void btnRemoveDialogue_Click(object sender, RoutedEventArgs e)
        {
            if (lstDialogues.SelectedIndex >= 0 || lstDialogues.SelectedIndex < lstDialogues.Items.Count)
            {
                DialogueID[cmbLanguage.Text].Remove(((Dialogue)lstDialogues.SelectedItem).id);
                Dialogues[cmbLanguage.Text].RemoveAt(lstDialogues.SelectedIndex);
                lstDialogues.Items.Refresh();
            }
        }

        private void lstChoices_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (lstChoices.SelectedIndex >= 0 && lstChoices.SelectedIndex < lstChoices.Items.Count)
            {
                btnRemoveChoice.IsEnabled = true;
            }
        }

        private void btnRemoveChoice_Click(object sender, RoutedEventArgs e)
        {
            if (lstChoices.SelectedIndex >= 0 && lstChoices.SelectedIndex < lstChoices.Items.Count)
            {
                Choices[cmbLanguage.Text].Remove((Choice)lstChoices.SelectedItem);
                lstChoices.Items.Refresh();
            }
        }

        private void cmbLanguage_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string lang = e.AddedItems[0].ToString();

            if (!Dialogues.ContainsKey(lang))
                Dialogues.Add(lang, new List<Dialogue>());
            if (!DialogueID.ContainsKey(lang))
                DialogueID.Add(lang, new HashSet<int>());
            if (!Choices.ContainsKey(lang))
                Choices.Add(lang, new List<Choice>());

            lstDialogues.ItemsSource = Dialogues[lang];
            lstDialogues.Items.Refresh();

            lstChoices.ItemsSource = Choices[lang];
            lstChoices.Items.Refresh();
        }
    }
}
