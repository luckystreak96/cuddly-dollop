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

        public MainWindow()
        {
            InitializeComponent();
            editor = new Editor();
            Setup(32, 18);

            SetupTileSelecter();
            ReleaseMouse();
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

            if (x > 0 && x < cnvMap.Width && y > 0 && y < cnvMap.Height)
                SetMapTile(x, y, selTile);
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
        }

        private void SetMapTile(int x, int y, string brush)
        {
            x /= 32;
            y /= 32;
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
            foreach (var x in editor.Tiles)
                cnvMap.Children.Add(x.Value);
        }

        private void CnvMap_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            //AddCurrentMapToHistory();
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
            ChangeOpacity();
        }

        private void ChangeOpacity()
        {
            foreach (Rectangle x in cnvMap.Children)
            {
                int zind = (int)x.GetValue(Canvas.ZIndexProperty);
                if (!opaqueView && zind != (int)((10 - slrDepth.Value) * 2))
                {
                    double opacity =  1 - Math.Abs((20 - slrDepth.Value * 2) - zind) / 5;
                    x.Fill.Opacity = opacity;
                }
                else
                    x.Fill.Opacity = 1.0;
            }
        }

        private void ChkOpaqueView_Checked(object sender, RoutedEventArgs e)
        {
            opaqueView = chkOpaqueView.IsChecked ?? false;
            ChangeOpacity();
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
            editor.Serialize();
        }

        private void MenuOpen_Click(object sender, RoutedEventArgs e)
        {
            editor.Load();
            ReSyncOnEditor();
        }
    }
}
