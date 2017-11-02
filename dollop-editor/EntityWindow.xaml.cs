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
using System.Windows.Media.Media3D;
using System.Windows.Shapes;

namespace dollop_editor
{
    /// <summary>
    /// Interaction logic for Entity.xaml
    /// </summary>
    public partial class EntityWindow : Window
    {
        public Entity Entity { get; set; }

        public EntityWindow(Point3D pos, Dictionary<Point3D, Tuple<Entity, Rectangle>> dict)
        {
            Entity = new Entity();
            Entity.id = -1;

            InitializeComponent();


            for (int i = 1; i < 999999; i++)
            {
                foreach (var x in dict)
                    if (x.Value.Item1.id == i)
                        continue;

                txtID.Text = i.ToString();
                break;
            }

            txtX.Text = pos.X.ToString();
            txtY.Text = pos.Y.ToString();
            txtZ.Text = pos.Z.ToString();
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            int id = -1;
            float x, y, z = -1;
            bool player = false;
            int.TryParse(txtID.Text, out id);
            float.TryParse(txtX.Text, out x);
            float.TryParse(txtY.Text, out y);
            float.TryParse(txtZ.Text, out z);
            player = chkPlayer.IsChecked == true;

            if (x == -1 || y == -1 || z == -1)
            {
                MessageBox.Show("X, Y or Z are not valid.");
                return;
            }

            if (id == -1)
            {
                MessageBox.Show("ID is not valid.");
                return;
            }

            Entity = new Entity()
            {
                id = id,
                x = x,
                y = y,
                z = z,
                player = player
            };

            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
