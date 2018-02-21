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
        public List<string> _Strings;

        public WindowStatCurve()
        {
            _Stats = new Dictionary<string, CurveStyle>();
            _Strings = new List<string>();
            InitializeComponent();
            cmbCurve.ItemsSource = _Strings;
            SetCmbCurves();
        }

        private void SetCmbCurves()
        {
            _Strings.Clear();
            foreach(var x in _Stats)
                _Strings.Add(x.Key);

            cmbCurve.Items.Refresh();
        }

        private void cmbCurve_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            cmbOperator.Text = _Stats[cmbCurve.Text].op.ToString();
            txtValue.Text = _Stats[cmbCurve.Text].value.ToString();
            cmbStat.Text = _Stats[cmbCurve.Text].stat.ToString();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            if (txtName.Text == "")
            {
                MessageBox.Show("Curve must have a name");
                return;
            }

            try
            {
                CurveStyle style = new CurveStyle() { op = 'i', value = 32, stat = "hp" };
                if (_Stats.ContainsKey(cmbCurve.Text))
                    _Stats.Remove(cmbCurve.Text);

                _Stats.Add(txtName.Text, style);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }

            SetCmbCurves();
        }

        private void btnNew_Click(object sender, RoutedEventArgs e)
        {
            Random random = new Random();
            CurveStyle style = new CurveStyle() { op = 'i', value = 32 };
            string name = "";
            do
            {
                name = "Curve" + random.Next().ToString();
            } while (_Stats.ContainsKey(name));

            _Stats.Add(name, style);

            cmbCurve.Items.Refresh();
            cmbCurve.SelectedValue = name;
        }

        private void btnDelete_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                if (_Stats.ContainsKey(cmbCurve.Text))
                    _Stats.Remove(cmbCurve.Text);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}
