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
        public List<Dialogue> Dialogues { get; set; }
        public List<Choice> Choices { get; set; }
        private HashSet<int> DialogueID = new HashSet<int>();

        public DialogueWindow()
        {
            InitializeComponent();
            Dialogues = new List<Dialogue>();
            Choices = new List<Choice>();
            lstDialogues.ItemsSource = Dialogues;
        }

        public DialogueWindow(Event ev)
        {
            InitializeComponent();
            Dialogues = new List<Dialogue>();
            Choices = new List<Choice>();
            lstDialogues.ItemsSource = Dialogues;
            Event_ = ev;

            // Json stuffs goin on
            try
            {
                foreach (var o in Event_.args)
                {
                    if (o.Key.Contains("dialogue"))
                    {
                        JObject jObject = o.Value as JObject;
                        Dialogue d = jObject.ToObject<Dialogue>();
                        Dialogues.Add(d);
                        DialogueID.Add(d.id);
                    }
                    else if (o.Key.Contains("choice"))
                    {
                        JObject jObject = o.Value as JObject;
                        Choices.Add(jObject.ToObject<Choice>());
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            lstDialogues.Items.Refresh();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
            Dictionary<string, object> dictionary = new Dictionary<string, object>();
            foreach (var x in Dialogues)
            {
                string temp = JsonConvert.SerializeObject(x, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
                JObject jObject = JObject.Parse(temp);
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
            foreach (var x in Choices)
            {
                string temp = JsonConvert.SerializeObject(x, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore });
                JObject jObject = JObject.Parse(temp);
                dictionary.Add("choice " + counter++, jObject);
            }

            Event_.args = dictionary;

            Close();
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
                Dialogue dialogue = Dialogues.ElementAt(lstDialogues.SelectedIndex);
                int temp = dialogue.id;

                DialogueModify dialogueModify = new DialogueModify(dialogue);
                dialogueModify.ShowDialog();

                if (dialogue.id != temp)
                {
                    DialogueID.Remove(temp);
                    DialogueID.Add(dialogue.id);
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

            Dialogue dialogue = Dialogues.ElementAt(lstDialogues.SelectedIndex);
            List<Choice> list = new List<Choice>();
            foreach (Choice c in Choices)
                if (c.d == dialogue.id)
                    list.Add(c);

            lstChoices.ItemsSource = list;
            lstChoices.Items.Refresh();
        }

        private void lstChoices_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2 && lstChoices.SelectedIndex >= 0 && lstChoices.SelectedIndex < lstChoices.Items.Count)
            {
                Choice choice = Choices.ElementAt(lstChoices.SelectedIndex);

                ChoiceModify choiceModify = new ChoiceModify(choice);
                choiceModify.ShowDialog();

                lstChoices.Items.Refresh();
            }
        }

        private void btnAddDialogue_Click(object sender, RoutedEventArgs e)
        {
            Dialogue dialogue = new Dialogue();
            for (int i = 0; i < 999; i++)
                if (!DialogueID.Contains(i))
                {
                    DialogueID.Add(i);
                    dialogue.id = i;
                    break;
                }
            Dialogues.Add(dialogue);

            lstDialogues.Items.Refresh();
        }

        private void btnAddChoice_Click(object sender, RoutedEventArgs e)
        {
            Choice choice = new Choice();
            choice.d = ((Dialogue)lstDialogues.SelectedItem).id;
            Choices.Add(choice);

            ((Dialogue)lstDialogues.SelectedItem).type = "choice";

            lstChoices.Items.Refresh();
        }

        private void btnRemoveDialogue_Click(object sender, RoutedEventArgs e)
        {
            if (lstDialogues.SelectedIndex >= 0 || lstDialogues.SelectedIndex < lstDialogues.Items.Count)
            {
                DialogueID.Remove(((Dialogue)lstDialogues.SelectedItem).id);
                Dialogues.RemoveAt(lstDialogues.SelectedIndex);
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
                Choices.Remove((Choice)lstChoices.SelectedItem);
                lstChoices.Items.Refresh();
            }
        }
    }
}
