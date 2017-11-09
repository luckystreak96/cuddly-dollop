using Microsoft.Win32;
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace dollop_editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Editor editor;
        private string selTile = "";
        private int prevx = -1;
        private int prevy = -1;
        private bool opaqueView = false;
        private bool entityMode = false;
        private int contextMenuX = 0;
        private int contextMenuY = 0;
        private Rectangle selectedTile;

        public MainWindow()
        {
            InitializeComponent();

            editor = new Editor();
            Setup(32, 18);

            SetupTileSelecter();
            ReleaseMouse();

            selectedTile = new Rectangle()
            {
                Width = editor.TileSize,
                Height = editor.TileSize,
                Stroke = new SolidColorBrush() { Color = Colors.Black, Opacity = 1.0 },
                StrokeThickness = 3.0
            };
            selectedTile.SetCurrentValue(Canvas.ZIndexProperty, 20);
            selectedTile.RenderTransform = new TranslateTransform(0, 0);
            cnvTilePicker.Children.Add(selectedTile);
        }

        private void Setup(int x, int y)
        {
            editor.Setup(x, y);

            cnvMap.Children.Clear();
            cnvMap.Width = editor.Width * editor.TileSize;
            cnvMap.Height = editor.Height * editor.TileSize;
            editor.AddTile(2, 2, slrDepth.Value, "pumpkin.png");
            foreach (var t in editor.Tiles)
                cnvMap.Children.Add(t.Value);
        }

        private void CnvMap_MouseDown(object sender, MouseButtonEventArgs e)
        {
            int x = (int)e.GetPosition(cnvMap).X;
            int y = (int)e.GetPosition(cnvMap).Y;

            if (e.LeftButton == MouseButtonState.Pressed)
            {
                if(entityMode && e.ClickCount == 2 ||
                    !entityMode && x > 0 && x < cnvMap.Width && y > 0 && y < cnvMap.Height)
                    SetMapTile(x, y, selTile);
                else // Entitymode && clickcount == 1
                {
                    if(entityMode)
                    {

                    }
                }
            }
            else if (e.RightButton == MouseButtonState.Pressed)
            {
                contextMenuX = x / 32;
                contextMenuY = y / 32;
                FrameworkElement fe = e.Source as FrameworkElement;
                if (entityMode)
                {
                    ContextMenu theMenu = new ContextMenu();
                    MenuItem modify = new MenuItem();
                    modify.Header = "Modify";
                    modify.Click += Modify_Click;
                    MenuItem delete = new MenuItem();
                    delete.Header = "Delete";
                    delete.Click += Delete_Click;
                    theMenu.Items.Add(modify);
                    theMenu.Items.Add(delete);

                    fe.ContextMenu = theMenu;
                }
                else
                    fe.ContextMenu.Items.Clear();
            }
        }

        private void CnvTilePicker_MouseDown(object sender, MouseButtonEventArgs e)
        {
            int x = (int)e.GetPosition(cnvTilePicker).X;
            int y = (int)e.GetPosition(cnvTilePicker).Y;

            selTile = "";
            // -1 at the end because of the empty tile at the beginning for erasing
            int index = (x / 32) + ((y / 32) * ((int)cnvTilePicker.Width / 32)) - 1;

            // DONT CRASH PLZZ
            if (index >= editor.Brushes.Count || index < 0)
                return;

            selTile = editor.Brushes.ElementAt(index).Key;

            selectedTile.RenderTransform = new TranslateTransform((x / 32) * 32, (y / 32) * 32);
        }

        // Sets up the images for the sprite selecter
        private void SetupTileSelecter()
        {
            int offx = 32;
            int offy = 0;
            int width = (int)cnvTilePicker.Width;
            foreach (var x in editor.Brushes)
            {
                Rectangle r = new Rectangle
                {
                    Width = 32,
                    Height = 32,
                    Fill = x.Value,
                    RenderTransform = new TranslateTransform(offx, offy)
                };
                cnvTilePicker.Children.Add(r);
                offx += 32;
                if (offx >= width)
                {
                    offx = 0;
                    offy += 32;
                }
            }

            cnvTilePicker.Height = offy + 32;
        }

        private void SetMapTile(int x, int y, string brush)
        {
            x /= 32;
            y /= 32;

            if (entityMode)
            {
                EntitySelected(x, y, slrDepth.Value);
                return;
            }

            // Dont want to spam add a tile on the same spot
            if (x == prevx && y == prevy)
                return;
            prevx = x;
            prevy = y;
            var t = editor.AddTile(x, y, slrDepth.Value, brush);
            if (cnvMap.Children.Contains(t.Item2))
                cnvMap.Children.Remove(t.Item2);
            if (t.Item1 != null)
                cnvMap.Children.Add(t.Item1);
        }

        private void EntitySelected(int x, int y, double z)
        {
            Point3D point = new Point3D(x, y, z);
            if (editor.Entities.ContainsKey(point))
            {
                // Load up that entity into the new window
                EntityWindow window = new EntityWindow(new Point3D(x, editor.Height - 1 - y, z), editor.Entities, editor.Height, this, editor);
                window.ShowDialog();
                ReSyncOnEditor();
            }
            else
            {
                // Open entity window
                EntityWindow window = new EntityWindow(new Point3D(x, editor.Height - 1 - y, z), editor.Entities, editor.Height, this, editor);
                window.ShowDialog();

                if (window.Entity.id == -1)
                    return;

                Rectangle rectangle = editor.EntityRectangle(window.Entity);

                editor.Entities.Add(point, new Tuple<Entity, Rectangle>(window.Entity, rectangle));
                cnvMap.Children.Add(rectangle);
            }
        }

        private void CnvMap_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                int x = (int)e.GetPosition(cnvMap).X;
                int y = (int)e.GetPosition(cnvMap).Y;


                if (x > 0 && x < cnvMap.Width && y > 0 && y < cnvMap.Height)
                    SetMapTile(x, y, selTile);
            }
        }

        private void ReSyncOnEditor()
        {
            cnvMap.Children.Clear();

            // Draw the tiles
            foreach (var x in editor.Tiles)
                cnvMap.Children.Add(x.Value);

            // Draw the entities
            if (entityMode)
                foreach (var x in editor.Entities)
                    cnvMap.Children.Add(x.Value.Item2);
        }

        private void AddCurrentMapToHistory()
        {
            Dictionary<Point3D, Rectangle> dictionary = new Dictionary<Point3D, Rectangle>(editor.Tiles.Count);
            foreach (var x in editor.Tiles)
                dictionary.Add(x.Key, x.Value);
            if (editor.HistoryProgress >= 0 && editor.HistoryProgress < editor.TileHistory.Count - 1)
                editor.TileHistory.RemoveRange(editor.HistoryProgress + 1, editor.TileHistory.Count - (editor.HistoryProgress + 1));
            editor.TileHistory.Add(dictionary);
            editor.HistoryProgress++;
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyboardDevice.IsKeyDown(Key.LeftCtrl) && e.KeyboardDevice.IsKeyDown(Key.Z))
            {
                if (editor.HistoryProgress > 0 && editor.HistoryProgress < editor.TileHistory.Count)
                {
                    editor.HistoryProgress--;
                    Dictionary<Point3D, Rectangle> dictionary = new Dictionary<Point3D, Rectangle>(editor.TileHistory[editor.HistoryProgress].Count);
                    foreach (var x in editor.TileHistory[editor.HistoryProgress])
                        dictionary.Add(x.Key, x.Value);
                    editor.Tiles = dictionary;
                    ReSyncOnEditor();
                }
            }
            if (e.KeyboardDevice.IsKeyDown(Key.LeftCtrl) && e.KeyboardDevice.IsKeyDown(Key.Y))
            {
                if (editor.HistoryProgress >= 0 && editor.HistoryProgress < editor.TileHistory.Count - 1)
                {
                    editor.HistoryProgress++;
                    Dictionary<Point3D, Rectangle> dictionary = new Dictionary<Point3D, Rectangle>(editor.TileHistory[editor.HistoryProgress].Count);
                    foreach (var x in editor.TileHistory[editor.HistoryProgress])
                        dictionary.Add(x.Key, x.Value);
                    editor.Tiles = dictionary;
                    ReSyncOnEditor();
                }
            }
        }

        private void ReleaseMouse()
        {
            AddCurrentMapToHistory();
        }

        private void CnvMap_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Released)
                ReleaseMouse();
            prevx = -1;
            prevy = -1;
        }

        private void SlrDepth_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            Editor.ChangeOpacity(cnvMap.Children, slrDepth.Value, opaqueView);
        }


        private void ChkOpaqueView_Checked(object sender, RoutedEventArgs e)
        {
            opaqueView = chkOpaqueView.IsChecked ?? false;
            Editor.ChangeOpacity(cnvMap.Children, slrDepth.Value, opaqueView);
        }

        private void MenuNew_Click(object sender, RoutedEventArgs e)
        {
            NewMap newMap = new NewMap();
            newMap.ShowDialog();
            if (newMap.MapWidth != -1 && newMap.Height != -1)
                Setup(newMap.MapWidth, newMap.MapHeight);
        }

        private void MenuSave_Click(object sender, RoutedEventArgs e)
        {
            FileDialog fileDialog = new SaveFileDialog();
            fileDialog.FileName = "001";
            fileDialog.DefaultExt = ".json";
            fileDialog.Filter = "JSON files (.json)|*.json";

            bool? result;
            while ((result = fileDialog.ShowDialog()) == true)
            {
                // Open document 
                string filename = fileDialog.FileName;
                if (int.TryParse(filename.Split('\\').Last().Split('.').First(), out int temp) != true)
                {
                    MessageBox.Show("Name must be a number (ex: 001.json)");
                    continue;
                }
                editor.Serialize(filename);
                break;
            }
        }

        private void MenuOpen_Click(object sender, RoutedEventArgs e)
        {
            FileDialog fileDialog = new OpenFileDialog();
            fileDialog.DefaultExt = ".json";
            fileDialog.Filter = "JSON files (.json)|*.json";

            bool? result = fileDialog.ShowDialog();

            if (result == true)
            {
                // Open document 
                string filename = fileDialog.FileName;

                editor.Load(filename);
                ReSyncOnEditor();
            }

            Editor.ChangeOpacity(cnvMap.Children, slrDepth.Value, opaqueView);
        }

        private void chkEntityMode_Click(object sender, RoutedEventArgs e)
        {
            entityMode = chkEntityMode.IsChecked ?? false;
            ReSyncOnEditor();
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            Point3D p = new Point3D(contextMenuX, contextMenuY, slrDepth.Value);
            if (editor.Entities.ContainsKey(p))
            {
                editor.Entities.Remove(p);
                ReSyncOnEditor();
            }
        }

        private void Modify_Click(object sender, RoutedEventArgs e)
        {
            EntitySelected(contextMenuX, contextMenuY, slrDepth.Value);
        }
    }
}
