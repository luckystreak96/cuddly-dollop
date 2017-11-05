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
        private Dictionary<Point3D, Tuple<Entity, Rectangle>> dictionary;
        private Point3D location;
        private MainWindow mainWindow;
        private Editor Editor;
        private int mapHeight;

        public EntityWindow(Point3D pos, Dictionary<Point3D, Tuple<Entity, Rectangle>> dict, int height, MainWindow window, Editor editor)
        {
            Editor = editor;
            mapHeight = height;
            mainWindow = window;
            dictionary = dict;
            location = pos;
            Entity = new Entity();
            Entity.id = -1;

            InitializeComponent();

            for (int i = 1; i < 999999; i++)
            {
                bool exists = false;
                foreach (var x in dict)
                    if (x.Value.Item1.id == i)
                    {
                        exists = true;
                        break;
                    }

                if (!exists)
                {
                    txtID.Text = i.ToString();
                    break;
                }
            }

            Point3D p = new Point3D(pos.X, mapHeight - 1 - pos.Y, pos.Z);
            if (dictionary.ContainsKey(p))
            {
                Entity = dictionary[p].Item1;
                txtX.Text = dictionary[p].Item1.x.ToString();
                txtY.Text = dictionary[p].Item1.y.ToString();
                txtZ.Text = dictionary[p].Item1.z.ToString();
                txtID.Text = dictionary[p].Item1.id.ToString();
                txtSprite.Text = dictionary[p].Item1.sprite.ToString();
                chkPlayer.IsChecked = dictionary[p].Item1.player;
            }
            else
            {
                txtX.Text = pos.X.ToString();
                txtY.Text = pos.Y.ToString();
                txtZ.Text = pos.Z.ToString();
            }

            // At the end because otherwise it follows the queues of an empty entity if it existed (see line 62)
            lstQueue.ItemsSource = Entity.queues;
                if (Entity.queues.Count > 0)
                    lstQueue.SelectedIndex = 0;
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
                player = player,
                sprite = txtSprite.Text
            };

            Point3D p = new Point3D(location.X, mapHeight - 1 - location.Y, location.Z);
            if (dictionary.ContainsKey(p))
            {
                Point3D current = new Point3D(Math.Floor(Entity.x), Editor.InvertHeight((float)Math.Floor(Entity.y)), Entity.z);
                Tuple<Entity, Rectangle> tu = new Tuple<Entity, Rectangle>(Entity, Editor.EntityRectangle(Entity));
                if (current != p)
                {
                    dictionary.Remove(p);
                    dictionary.Add(current, tu);
                }
                else
                    dictionary[p] = tu;

            }

            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }

        private void lstQueue_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            lstEvent.ItemsSource = Entity.queues[lstQueue.SelectedIndex].events;
        }
    }
}
