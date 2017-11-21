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

            Point3D p = new Point3D(pos.X, pos.Y, pos.Z);
            if (dictionary.ContainsKey(p))
            {
                Entity = dictionary[p].Item1;
                chkEthereal.IsChecked = dictionary[p].Item1.ethereal;
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

            btnAddEvent.IsEnabled = false;
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            int id = -1;
            float x, y, z = -1;
            bool player = false;
            bool ethereal = false;
            int.TryParse(txtID.Text, out id);
            float.TryParse(txtX.Text, out x);
            float.TryParse(txtY.Text, out y);
            float.TryParse(txtZ.Text, out z);
            player = chkPlayer.IsChecked == true;
            ethereal = chkEthereal.IsChecked == true;

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

            Entity.id = id;
            Entity.x = x;
            Entity.y = y;
            Entity.z = z;
            Entity.player = player;
            Entity.ethereal = ethereal;
            Entity.sprite = txtSprite.Text;

            Point3D p = new Point3D(location.X, location.Y, location.Z);
            if (dictionary.ContainsKey(p))
            {
                Point3D current = new Point3D(Math.Floor(Entity.x), Math.Floor(Entity.y), Entity.z);
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
            if (lstQueue.SelectedIndex >= 0 && Entity.queues.Count > lstQueue.SelectedIndex)
            {
                lstEvent.ItemsSource = Entity.queues[lstQueue.SelectedIndex].events;
                btnAddEvent.IsEnabled = true;
            }
        }

        private void btnAddQueue_Click(object sender, RoutedEventArgs e)
        {
            QueueWindow queueWindow = new QueueWindow();
            if (queueWindow.ShowDialog() == true)
                Entity.queues.Add(queueWindow.Queue);

            lstQueue.Items.Refresh();
        }

        private void lstQueue_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (lstQueue.SelectedIndex >= 0 && lstQueue.SelectedIndex < lstQueue.Items.Count)
            {
                if (e.ClickCount == 2)
                {
                    QueueWindow queueWindow = new QueueWindow(lstQueue.SelectedItem as EventQueue);
                    queueWindow.ShowDialog();
                    lstQueue.Items.Refresh();
                }
                btnAddEvent.IsEnabled = true;
            }
        }

        private void lstEvent_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2 && lstEvent.SelectedIndex >= 0 && lstEvent.SelectedIndex < lstEvent.Items.Count && lstQueue.SelectedIndex >= 0 && lstQueue.SelectedIndex < lstQueue.Items.Count)
            {
                Event ev = lstEvent.SelectedItem as Event;
                EventWindow eventWindow = new EventWindow(ev);

                if (eventWindow.ShowDialog() == true)
                {
                    Entity.queues[lstQueue.SelectedIndex].events[lstEvent.SelectedIndex] = eventWindow.Event;
                }
                lstEvent.Items.Refresh();
            }
        }

        private void btnAddEvent_Click(object sender, RoutedEventArgs e)
        {
            if (lstQueue.SelectedIndex >= 0 && lstQueue.SelectedIndex < lstQueue.Items.Count)
            {
                EventWindow eventWindow = new EventWindow();
                if (eventWindow.ShowDialog() == true)
                {
                    Entity.queues[lstQueue.SelectedIndex].events.Add(eventWindow.Event);
                    lstEvent.Items.Refresh();
                }
            }
        }

        private void btnRemoveEvent_Click(object sender, RoutedEventArgs e)
        {
            if (lstEvent.SelectedIndex >= 0 && lstEvent.SelectedIndex < lstEvent.Items.Count)
            {
                Entity.queues[lstEvent.SelectedIndex].events.RemoveAt(lstEvent.SelectedIndex);
                lstEvent.Items.Refresh();
            }
        }

        private void btnRemoveQueue_Click(object sender, RoutedEventArgs e)
        {
            if (lstQueue.SelectedIndex >= 0 && lstQueue.SelectedIndex < lstQueue.Items.Count)
            {
                Entity.queues.RemoveAt(lstQueue.SelectedIndex);
                lstQueue.Items.Refresh();
            }
        }

        private void btnMoveUp_Click(object sender, RoutedEventArgs e)
        {
            if (lstEvent.SelectedIndex > 0)
            {
                int index = lstEvent.SelectedIndex;
                Entity.queues[lstQueue.SelectedIndex].events.Swap(index, index - 1);
                lstEvent.Items.Refresh();
                lstEvent.SelectedIndex = index - 1;
                lstEvent.Focus();
            }
        }

        private void btnMoveDown_Click(object sender, RoutedEventArgs e)
        {
            if (lstEvent.SelectedIndex < lstEvent.Items.Count - 1)
            {
                int index = lstEvent.SelectedIndex;
                Entity.queues[lstQueue.SelectedIndex].events.Swap(index, index + 1);
                lstEvent.Items.Refresh();
                lstEvent.SelectedIndex = index + 1;
                lstEvent.Focus();
            }
        }


    }
}
