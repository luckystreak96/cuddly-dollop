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

namespace dollop_editor.Battle
{
    /// <summary>
    /// Interaction logic for WindowPassiveSkills.xaml
    /// </summary>
    public partial class WindowActors : Window
    {
        List<string> skills_enum = new List<string>();

        public BattleData battleData { get; set; }

        public WindowActors()
        {
            InitializeComponent();

            foreach (var x in Enum.GetNames(typeof(Skills)))
                skills_enum.Add(x);

            cmbSkills.ItemsSource = skills_enum;
            cmbSkills.Items.Refresh();
        }

        public void Setup()
        {
            cmbActors.ItemsSource = battleData.fighters;
            cmbActors.Items.Refresh();
            cmbCurve.ItemsSource = battleData.curves.Keys;
            cmbCurve.Items.Refresh();
            cmbPassives.ItemsSource = battleData.passives;
            cmbPassives.Items.Refresh();
        }

        private void SaveCurrentActor()
        {
            try
            {
                if (txtName.Text == "")
                    throw new Exception("Please give a name before saving.");
                if (txtLevel.Text == "")
                    throw new Exception("Level is required.");

                Fighter actor;
                if (txtId.Text == "")
                    actor = new Fighter();
                else
                    actor = battleData.fighters.First(x => x.id == int.Parse(txtId.Text));

                if (txtHealth.Text == "")
                    actor.health = 99999;
                else
                    actor.health = int.Parse(txtHealth.Text);

                if (txtExp.Text == "")
                    actor.exp = 0;
                else
                    actor.exp = int.Parse(txtExp.Text);

                actor.level = int.Parse(txtLevel.Text);

                if (txtSkillPoints.Text == "")
                    actor.skillpoints = actor.level * 4;
                else
                    actor.skillpoints = int.Parse(txtSkillPoints.Text);

                actor.dead = (bool)chkDead.IsChecked;
                actor.name = txtName.Text;
                actor.sprite = txtSprite.Text;

                actor.curve = cmbCurve.Text;

                actor.skills = new List<string>();
                foreach (string x in lstSkills.Items)
                    actor.skills.Add(x);

                actor.passives = new List<int>();
                foreach (PassiveSkill x in lstPassives.Items)
                    actor.passives.Add(x.id);

                if (txtId.Text == "")
                {
                    int max = 0;
                    if(battleData.fighters.Count > 0)
                        max = battleData.fighters.Max(x => x.id);
                    max++;
                    actor.id = max;
                    txtId.Text = max.ToString();

                    battleData.fighters.Add(actor);
                    cmbActors.Items.Refresh();
                    cmbActors.SelectedItem = actor;
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            if (MessageBox.Show("Are you sure you want to exit without saving?", "Exit Without Save", MessageBoxButton.YesNo) == MessageBoxResult.Yes)
            {
                DialogResult = false;
                Close();
            }
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            SaveCurrentActor();
            DialogResult = true;
            Close();
        }

        private void btnNew_Click(object sender, RoutedEventArgs e)
        {
            Fighter actor = new Fighter();
            battleData.fighters.Add(actor);
            cmbActors.Items.Refresh();
            cmbActors.SelectedItem = actor;

            cmbPassives.SelectedIndex = -1;
            cmbSkills.SelectedIndex = -1;
            cmbCurve.SelectedIndex = -1;

            txtExp.Text = "";
            txtHealth.Text = "";
            txtId.Text = "";
            txtLevel.Text = "";
            txtName.Text = "";
            txtSkillPoints.Text = "";
            txtSprite.Text = "";
            chkDead.IsChecked = false;

            int max = battleData.fighters.Max(x => x.id);
            max++;
            actor.id = max;
            txtId.Text = max.ToString();
        }

        private void cmbSkills_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count < 1)
                return;
            if (e.AddedItems[0] == null)
                return;
            string skill = e.AddedItems[0].ToString();
            lstSkills.Items.Add(skill);
        }

        private void cmbActors_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count < 1)
                return;
            Fighter actor = (Fighter)e.AddedItems[0];
            if (actor == null)
                return;

            cmbPassives.SelectedIndex = -1;
            cmbSkills.SelectedIndex = -1;
            string curve = battleData.curves.Keys.FirstOrDefault(x => x == actor.curve);
            if (curve == null)
                curve = "";
            cmbCurve.SelectedItem = curve;

            txtExp.Text = actor.exp.ToString();
            txtHealth.Text = actor.health.ToString();
            txtId.Text = actor.id.ToString();
            txtLevel.Text = actor.level.ToString();
            txtName.Text = actor.name;
            txtSkillPoints.Text = actor.skillpoints.ToString();
            txtSprite.Text = actor.sprite;
            chkDead.IsChecked = actor.dead;

            lstPassives.Items.Clear();
            lstSkills.Items.Clear();
        }

        private void cmbPassives_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (e.AddedItems.Count < 1)
                return;
            PassiveSkill passive = (PassiveSkill)e.AddedItems[0];
            lstPassives.Items.Add(passive);
        }

        private void btnSaveActor_Click(object sender, RoutedEventArgs e)
        {
            SaveCurrentActor();
        }

        private void btnRemoveSkill_Click(object sender, RoutedEventArgs e)
        {
            int index = lstSkills.SelectedIndex;
            if (index > -1)
            {
                lstSkills.Items.RemoveAt(index);
                lstSkills.Items.Refresh();
            }
        }

        private void btnRemovePassive_Click(object sender, RoutedEventArgs e)
        {
            int index = lstPassives.SelectedIndex;
            if (index > -1)
            {
                lstPassives.Items.RemoveAt(index);
                lstPassives.Items.Refresh();
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (cmbActors.SelectedIndex > -1 && MessageBox.Show("Delete this actor?", "Fighter delete", MessageBoxButton.OKCancel) == MessageBoxResult.OK)
            {
                try
                {
                    battleData.fighters.Remove((Fighter)cmbActors.SelectedItem);
                    cmbActors.Items.Refresh();
                    cmbActors.SelectedIndex = -1;
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }
    }
}
