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
    /// Interaction logic for EventWindow.xaml
    /// </summary>
    public partial class EventWindow : Window
    {
        public EventWindow()
        {
            InitializeComponent();
            SetupComboBoxes();
        }

        public EventWindow(Event ev)
        {
            InitializeComponent();
            SetupComboBoxes();
        }

        private void SetupComboBoxes()
        {
            foreach (EventType val in Enum.GetValues(typeof(EventType)))
                cmbType.Items.Add(val.ToString());
            foreach (EventExecution val in Enum.GetValues(typeof(EventExecution)))
                cmbExecution.Items.Add(val.ToString());
        }
    }
}
