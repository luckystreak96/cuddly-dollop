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
    /// Interaction logic for DialogueModify.xaml
    /// </summary>
    public partial class DialogueModify : Window
    {
        public Dialogue Dialogue_ { get; set; }
        public enum DialogueTypes { simple, choice, end }

        private DialogueModify()
        {
            InitializeComponent();
        }

        private void SetCmb()
        {
            foreach (DialogueTypes val in Enum.GetValues(typeof(DialogueTypes)))
                cmbType.Items.Add(val.ToString());

            try
            {
                cmbType.SelectedValue = Dialogue_.type;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        public DialogueModify(Dialogue dialogue)
        {
            InitializeComponent();
            Dialogue_ = dialogue;
            SetCmb();
            txtID.Text = dialogue.id.ToString();
            txtNext.Text = dialogue.next.ToString();
            txtText.Text = dialogue.text;
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            Dialogue_.id = int.Parse(txtID.Text);
            Dialogue_.next = int.Parse(txtNext.Text);
            Dialogue_.text = txtText.Text;
            Dialogue_.type = cmbType.Text;
            Close();
        }

        private void btnQueue_Click(object sender, RoutedEventArgs e)
        {
            EntityWindow entityWindow = new EntityWindow(Dialogue_.queues);
            if (entityWindow.ShowDialog() == true)
                Dialogue_.queues = entityWindow.Entity_.queues;
        }
    }
}
