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
        public Entity Entity_ { get; set; }
        private Dictionary<Point3D, Tuple<Entity, Rectangle>> dictionary;
        private Point3D location;
        private MainWindow mainWindow;
        private Editor Editor;
        private int mapHeight;
        private bool isEntity = true;

        public EntityWindow(List<EventQueue> list)
        {
            InitializeComponent();
            isEntity = false;
            Entity_ = new Entity();
            Entity_.queues = list;
            if (Entity_.queues == null)
                Entity_.queues = new List<EventQueue>();
            txtID.IsEnabled = false;
            txtX.IsEnabled = false;
            txtY.IsEnabled = false;
            txtZ.IsEnabled = false;
            cmbSprites.IsEnabled = false;
            chkEthereal.IsEnabled = false;
            chkFullSize.IsEnabled = false;
            chkPlayer.IsEnabled = false;

            // At the end because otherwise it follows the queues of an empty entity if it existed (see line 62)
            lstQueue.ItemsSource = Entity_.queues;
            if (Entity_.queues.Count > 0)
                lstQueue.SelectedIndex = 0;
        }

        public EntityWindow(Point3D pos, Dictionary<Point3D, Tuple<Entity, Rectangle>> dict, int height, MainWindow window, Editor editor)
        {
            Editor = editor;
            mapHeight = height;
            mainWindow = window;
            dictionary = dict;
            location = pos;
            Entity_ = new Entity();
            Entity_.id = -1;

            InitializeComponent();

            cmbSprites.ItemsSource = new List<String>(Editor.Brushes.Keys.Where(x => x.Contains("entity")));
            cmbSprites.Items.Refresh();

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
                Entity_ = new Entity(dictionary[p].Item1);
                Entity_.SneakyId(dictionary[p].Item1.id);
                chkEthereal.IsChecked = dictionary[p].Item1.ethereal;
                chkFullSize.IsChecked = dictionary[p].Item1.full_size;
                txtX.Text = dictionary[p].Item1.x.ToString();
                txtY.Text = dictionary[p].Item1.y.ToString();
                txtZ.Text = dictionary[p].Item1.z.ToString();
                txtID.Text = dictionary[p].Item1.id.ToString();
                cmbSprites.Text = dictionary[p].Item1.sprite.ToString();
                chkPlayer.IsChecked = dictionary[p].Item1.player;
            }
            else
            {
                txtX.Text = pos.X.ToString();
                txtY.Text = pos.Y.ToString();
                txtZ.Text = pos.Z.ToString();
            }

            // At the end because otherwise it follows the queues of an empty entity if it existed (see line 62)
            lstQueue.ItemsSource = Entity_.queues;
            if (Entity_.queues.Count > 0)
                lstQueue.SelectedIndex = 0;

            btnAddEvent.IsEnabled = false;
        }

        private void btnSave_Click(object sender, RoutedEventArgs e)
        {
            if (isEntity)
            {

                int id = -1;
                float x, y, z = -1;
                bool player = false;
                bool ethereal, full_size = false;
                int.TryParse(txtID.Text, out id);
                float.TryParse(txtX.Text, out x);
                float.TryParse(txtY.Text, out y);
                float.TryParse(txtZ.Text, out z);
                player = chkPlayer.IsChecked == true;
                ethereal = chkEthereal.IsChecked == true;
                full_size = chkFullSize.IsChecked == true;

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

                Entity_.id = id;
                Entity_.x = x;
                Entity_.y = y;
                Entity_.z = z;
                Entity_.player = player;
                Entity_.ethereal = ethereal;
                Entity_.full_size = full_size;
                Entity_.sprite = cmbSprites.Text;

                Point3D p = new Point3D(location.X, location.Y, location.Z);
                if (dictionary.ContainsKey(p))
                {
                    Point3D current = new Point3D(Math.Floor(Entity_.x), Math.Floor(Entity_.y), Entity_.z);
                    Tuple<Entity, Rectangle> tu = new Tuple<Entity, Rectangle>(Entity_, Editor.EntityRectangle(Entity_));
                    if (current != p)
                    {
                        dictionary.Remove(p);
                        dictionary.Add(current, tu);
                    }
                    else
                        dictionary[p] = tu;

                }
            mainWindow.mustSave = true;
            }
            else
            {
                if (Entity_.queues.Count == 0)
                    Entity_.queues = null;
            }

            DialogResult = true;
            Close();
        }

        private void btnCancel_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = false;
            Close();
        }

        private void lstQueue_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (lstQueue.SelectedIndex >= 0 && Entity_.queues.Count > lstQueue.SelectedIndex)
            {
                lstEvent.ItemsSource = Entity_.queues[lstQueue.SelectedIndex].events;
                btnAddEvent.IsEnabled = true;
            }
        }

        private void btnAddQueue_Click(object sender, RoutedEventArgs e)
        {
            QueueWindow queueWindow = new QueueWindow();
            if (queueWindow.ShowDialog() == true)
                Entity_.queues.Add(queueWindow.Queue);

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
                if (ev.type == "dialogue")
                {
                    OpenDialogueWindow(ev);
                }
                else
                {
                    EventWindow eventWindow = new EventWindow(ev);

                    if (eventWindow.ShowDialog() == true)
                    {
                        Entity_.queues[lstQueue.SelectedIndex].events[lstEvent.SelectedIndex] = eventWindow.Event;
                    }
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
                    Event ev = eventWindow.Event;
                    Entity_.queues[lstQueue.SelectedIndex].events.Add(ev);
                    lstEvent.Items.Refresh();
                    lstEvent.SelectedItem = ev;
                    if(ev.type == "dialogue")
                    {
                        OpenDialogueWindow(ev);
                    }
                }
            }
        }

        private void btnRemoveEvent_Click(object sender, RoutedEventArgs e)
        {
            if (lstEvent.SelectedIndex >= 0 && lstEvent.SelectedIndex < lstEvent.Items.Count)
            {
                Entity_.queues[lstQueue.SelectedIndex].events.RemoveAt(lstEvent.SelectedIndex);
                lstEvent.Items.Refresh();
            }
        }

        private void btnRemoveQueue_Click(object sender, RoutedEventArgs e)
        {
            if (lstQueue.SelectedIndex >= 0 && lstQueue.SelectedIndex < lstQueue.Items.Count)
            {
                Entity_.queues.RemoveAt(lstQueue.SelectedIndex);
                lstQueue.Items.Refresh();
                lstEvent.ItemsSource = null;
            }
        }

        private void btnMoveUp_Click(object sender, RoutedEventArgs e)
        {
            if (lstEvent.SelectedIndex > 0)
            {
                int index = lstEvent.SelectedIndex;
                Entity_.queues[lstQueue.SelectedIndex].events.Swap(index, index - 1);
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
                Entity_.queues[lstQueue.SelectedIndex].events.Swap(index, index + 1);
                lstEvent.Items.Refresh();
                lstEvent.SelectedIndex = index + 1;
                lstEvent.Focus();
            }
        }

        private void OpenDialogueWindow(Event ev)
        {
            DialogueWindow window = new DialogueWindow(ev);

            if (window.ShowDialog() == true)
            {
                Entity_.queues[lstQueue.SelectedIndex].events[lstEvent.SelectedIndex] = window.Event_;
                lstEvent.Items.Refresh();
            }
        }
    }
}
