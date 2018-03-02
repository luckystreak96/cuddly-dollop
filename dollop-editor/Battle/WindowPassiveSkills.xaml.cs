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
    }
}
