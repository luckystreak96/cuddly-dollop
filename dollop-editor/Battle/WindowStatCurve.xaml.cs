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
    /// Interaction logic for WindowStatCurve.xaml
    /// </summary>
    public partial class WindowStatCurve : Window
    {
        public Dictionary<string, CurveStyle> _Stats;
        public List<string> _Operators;
        public Dictionary<string, Dictionary<string, CurveStyle>> _Curves;
        public List<string> _Strings;
        public List<string> _StatNames;

        public WindowStatCurve()
        {
            _Operators = new List<string>();
            _Operators.AddRange(Enum.GetNames(typeof(Operators)));

            _Curves = new Dictionary<string, Dictionary<string, CurveStyle>>();

            // Get all stat names
            _StatNames = new List<string>();
            _StatNames.AddRange(Enum.GetNames(typeof(Stats)));

            // Create stats container
            _Stats = new Dictionary<string, CurveStyle>();

            _Strings = new List<string>();

            InitializeComponent();

            cmbOperator.ItemsSource = _Operators;
            cmbOperator.Items.Refresh();
            cmbCurve.ItemsSource = _Strings;
            SetCmbCurves();
            SetCmbStats();
        }

        public void Setup()
        {
            SetCmbCurves();
            if (_Strings.Count > 0)
                cmbCurve.SelectedIndex = 0;
        }

        private void SetCmbStats()
        {
            cmbStat.ItemsSource = _StatNames;
            cmbStat.Items.Refresh();
        }

        private void SetCmbCurves()
        {
            _Strings.Clear();
            foreach (var x in _Curves)
                _Strings.Add(x.Key);

            cmbCurve.Items.Refresh();
        }

        private void SaveStat()
        {
            try
            {
                if (txtValue.Text == "")
                {
                    MessageBox.Show("Value must have a value!");
                    return;
                }
                else if (cmbOperator.Text == "")
                {
                    MessageBox.Show("Operator must have an operator!");
                    return;
                }

                CurveStyle style = new CurveStyle();
                style.op = cmbOperator.Text;
                style.value = float.Parse(txtValue.Text);

                if (_Stats.ContainsKey(cmbStat.Text))
                    _Stats.Remove(cmbStat.Text);

                _Stats.Add(cmbStat.Text, style);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.StackTrace + "\n" + ex.Message);
            }
        }

        private void SaveCurve()
        {
            try
            {
                if (txtName.Text == "")
                {
                    MessageBox.Show("Curve must have a name!");
                    return;
                }

                // Remove the current one...
                if (_Curves.ContainsKey(cmbCurve.Text))
                    _Curves.Remove(cmbCurve.Text);
                // ...and replace it with the new data
                _Curves.Add(txtName.Text, new Dictionary<string, CurveStyle>(_Stats));

                SetCmbCurves();
                cmbCurve.SelectedValue = txtName.Text;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.StackTrace + "\n" + ex.Message);
            }
        }

        private void SetValueAndOperator(string value = "")
        {
            // Allow setting the desired value
            if (value == "")
                value = cmbStat.Text;

            if (_Stats.ContainsKey(value))
            {
                cmbOperator.Text = _Stats[value].op.ToString();
                txtValue.Text = _Stats[value].value.ToString();
            }
            else
            {
                cmbOperator.Text = "";
                txtValue.Text = "";
            }
        }

        private void SetStatsVariable(string value = "6346354")
        {
            if (value == "6346354")
                value = cmbCurve.Text;

            if (_Curves.ContainsKey(value))
            {
                _Stats = new Dictionary<string, CurveStyle>();
                foreach (var stat in _Curves[value])
                    _Stats.Add(stat.Key, stat.Value);
            }
        }

        private void cmbCurve_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                txtName.Text = e.AddedItems[0].ToString();
                SetStatsVariable(e.AddedItems[0].ToString());
                SetValueAndOperator();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.StackTrace + "\n" + ex.Message);
            }
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            SaveCurve();
        }

        private void btnNew_Click(object sender, RoutedEventArgs e)
        {
            Random random = new Random();
            string name = "";
            do
            {
                name = "Curve" + random.Next().ToString();
            } while (_Stats.ContainsKey(name));

            _Curves.Add(name, new Dictionary<string, CurveStyle>());

            SetCmbCurves();
            cmbCurve.SelectedValue = name;
            txtName.Text = name;
            SetStatsVariable();
            SetValueAndOperator();
            SaveCurve();
        }

        private void cmbStat_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                //SetStatsVariable();
                SetValueAndOperator(e.AddedItems[0].ToString());
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.StackTrace + "\n" + ex.Message);
            }
        }

        private void txtName_LostFocus(object sender, RoutedEventArgs e)
        {
            SaveCurve();
        }

        private void txtValue_LostFocus(object sender, RoutedEventArgs e)
        {
            SaveStat();
        }

        private void btnDeleteStat_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_Stats.ContainsKey(cmbStat.Text))
                    _Stats.Remove(cmbStat.Text);

                SaveStat();

                SetStatsVariable();
                SetValueAndOperator();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.StackTrace + "\n" + ex.Message);
            }
        }

        private void btnDeleteCurve_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_Curves.ContainsKey(cmbCurve.Text))
                    _Curves.Remove(cmbCurve.Text);

                SetCmbCurves();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: " + ex.StackTrace + "\n" + ex.Message);
            }
        }
    }
}
