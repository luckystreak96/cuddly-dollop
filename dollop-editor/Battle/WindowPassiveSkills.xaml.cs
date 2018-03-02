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
    public partial class WindowPassiveSkills : Window
    {
        List<string> priorities_enum = new List<string>();
        List<string> types_enum = new List<string>();
        List<string> specifiers_enum = new List<string>();

        public List<PassiveSkill> passives = new List<PassiveSkill>();

        public WindowPassiveSkills()
        {
            InitializeComponent();

            foreach (var x in Enum.GetNames(typeof(PassivePriority)))
                priorities_enum.Add(x);

            foreach (var x in Enum.GetNames(typeof(PassiveType)))
                types_enum.Add(x);

            foreach (var x in Enum.GetNames(typeof(PassiveSpecifier)))
                specifiers_enum.Add(x);

            cmbPriority.ItemsSource = priorities_enum;
            cmbType.ItemsSource = types_enum;
            cmbSpecifier.ItemsSource = specifiers_enum;
        }

        public void Setup()
        {
            cmbPassives.ItemsSource = passives;
            cmbPassives.Items.Refresh();
        }

        private void SaveCurrentPassive()
        {
            try
            {
                PassiveSkill passiveSkill = new PassiveSkill();
                passiveSkill.name = txtName.Text;
                passiveSkill.priority = cmbPriority.Text;
                passiveSkill.specifier = cmbSpecifier.Text;
                passiveSkill.type = cmbType.Text;

                if (txtFloat.Text == "")
                    passiveSkill._float = 0;
                else
                    passiveSkill._float = float.Parse(txtFloat.Text);

                if (txtInt.Text == "")
                    passiveSkill._int = 0;
                else
                    passiveSkill._int = int.Parse(txtInt.Text);
                passiveSkill._string = txtString.Text;

                passiveSkill.id = int.Parse(txtId.Text);

                PassiveSkill ps = passives.Where(x => x.id == int.Parse(txtId.Text)).FirstOrDefault();
                if (ps != null && ps.id == int.Parse(txtId.Text))
                {
                    passives.Remove(ps);
                    passives.Add(passiveSkill);
                }
                else
                    passives.Add(passiveSkill);

                cmbPassives.Items.Refresh();
                cmbPassives.SelectedItem = passiveSkill;
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
            SaveCurrentPassive();
            DialogResult = true;
            Close();
        }

        private void btnSavePassive_Click(object sender, RoutedEventArgs e)
        {
            SaveCurrentPassive();
        }

        private void btnNew_Click(object sender, RoutedEventArgs e)
        {
            cmbPassives.SelectedIndex = -1;
            for (int i = 0; i < 9999; i++)
            {
                if (passives.Where(x => x.id == i).ToList().Count > 0)
                    continue;

                txtId.Text = i.ToString();
                break;
            }
        }

        private void cmbPassives_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            PassiveSkill passiveSkill = (PassiveSkill)cmbPassives.SelectedItem;
            if (passiveSkill != null)
            {
                txtFloat.Text = passiveSkill._float.ToString();
                txtInt.Text = passiveSkill._int.ToString();
                txtString.Text = passiveSkill._string;
                cmbPriority.Text = passiveSkill.priority;
                cmbType.Text = passiveSkill.type;
                cmbSpecifier.Text = passiveSkill.specifier;
                txtName.Text = passiveSkill.name;
                txtId.Text = passiveSkill.id.ToString();
            }
            else
            {
                txtFloat.Text = "";
                txtInt.Text = "";
                txtString.Text = "";
                cmbPriority.Text = "";
                cmbType.Text = "";
                cmbSpecifier.Text = "";
                txtName.Text = "";
                txtId.Text = "";
            }
        }
    }
}
