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
using System.IO;

namespace dollop_editor
{

    public enum CanvasState { NormalMode, EntityMode, TileAttributeMode }

    public partial class MainWindow : Window
    {
        private SolidColorBrush unselectedMode = new SolidColorBrush(Colors.LightGray);
        private SolidColorBrush selectedMode = new SolidColorBrush(Colors.OrangeRed);
        private Editor editor;
        private Entity entityCopy;
        private string selTile = "";
        private int prevx = -1;
        private int prevy = -1;
        private int clickprevx = -1;
        private int clickprevy = -1;
        private bool opaqueView = false;
        private CanvasState cnvState = CanvasState.NormalMode;
        private int contextMenuX = 0;
        private int contextMenuY = 0;
        private Rectangle selectedPickerTile;
        private Rectangle selectedMapTile;
        private string prevPath = "";
        public bool mustSave = false;
        private bool fill = false;

        public MainWindow()
        {
            System.Windows.FrameworkCompatibilityPreferences.KeepTextBoxDisplaySynchronizedWithTextProperty = false;
            InitializeComponent();

            cnvMap.RenderTransform = new ScaleTransform(0, -1);
            cnvMap.RenderTransform = new TranslateTransform(0, 16);

            editor = new Editor();
            Setup(32, 18, true);

            SetupTileSelecter();
            ReleaseMouse();
            SetCnvMode(CanvasState.NormalMode);

            selectedPickerTile = new Rectangle()
            {
                Width = editor.TileSize,
                Height = editor.TileSize,
                Stroke = new SolidColorBrush() { Color = Colors.Black, Opacity = 1.0 },
                StrokeThickness = 3.0
            };
            selectedPickerTile.SetCurrentValue(Canvas.ZIndexProperty, 20);
            selectedPickerTile.RenderTransform = new TranslateTransform(0, 0);
            selectedMapTile = new Rectangle()
            {
                Width = editor.TileSize,
                Height = editor.TileSize,
                Stroke = new SolidColorBrush() { Color = Colors.LawnGreen, Opacity = 1.0 },
                StrokeThickness = 3.0
            };
            selectedMapTile.SetCurrentValue(Canvas.ZIndexProperty, 20);
            selectedMapTile.RenderTransform = new TranslateTransform(0, 0);
            cnvTilePicker.Children.Add(selectedPickerTile);

            if (File.Exists("..\\..\\..\\Test\\res\\data\\1.json"))
                LoadMap("..\\..\\..\\Test\\res\\data\\1.json");
            //ReSyncOnEditor();
        }

        private void Setup(int x, int y, bool newMap = false)
        {
            editor.Setup(x, y, newMap);

            cnvMap.Children.Clear();
            cnvMap.Width = editor.Width * editor.TileSize;
            cnvMap.Height = editor.Height * editor.TileSize;
            ReSyncOnEditor();
            //editor.AddTile(2, 2, slrDepth.Value, "pumpkin.png");
            //foreach (var t in editor.Tiles)
            //    cnvMap.Children.Add(t.Value);
        }

        private void UpdateSelectedMapTile()
        {
            selectedMapTile.SetCurrentValue(Canvas.ZIndexProperty, 20);
            selectedMapTile.RenderTransform = new TranslateTransform(clickprevx * editor.TileSize, clickprevy * editor.TileSize);
            if (!opaqueView)
                selectedMapTile.Stroke.Opacity = 1;
            if (!cnvMap.Children.Contains(selectedMapTile))
                cnvMap.Children.Add(selectedMapTile);
        }

        private void UpdateSelectedMapTile(int x, int y)
        {
            selectedMapTile.SetCurrentValue(Canvas.ZIndexProperty, 20);
            selectedMapTile.RenderTransform = new TranslateTransform((x / editor.TileSize) * editor.TileSize, (y / editor.TileSize) * editor.TileSize);
            if (!opaqueView)
                selectedMapTile.Stroke.Opacity = 1;
            if (!cnvMap.Children.Contains(selectedMapTile))
                cnvMap.Children.Add(selectedMapTile);
        }

        private void Fill(string tile, int x, int y, double z, bool firstTime, string otherTile)
        {
            if (x < 0 || y < 0)
                return;

            int x_ = x / editor.TileSize;
            int y_ = y / editor.TileSize;

            if (x_ >= editor.Width || y_ >= editor.Height)
                return;

            Point3D point3D = new Point3D(x_, editor.InvertHeight(y_), z);

            string under;
            if (!editor.Tiles.ContainsKey(point3D))
                under = "";
            else
                under = ((BitmapImage)(editor.Tiles[point3D].Fill.GetValue(ImageBrush.ImageSourceProperty))).UriSource.ToString().Split('\\').Last();
            if (under == tile)
                return;

            if (firstTime)
            {
                if (editor.Tiles.ContainsKey(point3D))
                    otherTile = under;
                else
                    otherTile = "";
            }

            if (otherTile == "" || under == otherTile)
                SetMapTile(x, y/*(int)editor.InvertHeight(float.Parse(y.ToString()))*/, selTile);
            else
                return;

            // Horizontal
            for (int h = -1; h < 2; h += 2)
            {
                point3D = new Point3D(((x + editor.TileSize * h) / editor.TileSize), editor.InvertHeight(y / editor.TileSize), z);
                if (!editor.Tiles.ContainsKey(point3D))
                    under = "";
                else
                    under = ((BitmapImage)(editor.Tiles[point3D].Fill.GetValue(ImageBrush.ImageSourceProperty))).UriSource.ToString().Split('\\').Last();
                if (otherTile == "" || under == otherTile)
                    Fill(tile, (int)point3D.X * editor.TileSize, (int)editor.InvertHeight(float.Parse(point3D.Y.ToString())) * editor.TileSize, z, false, otherTile);
            }

            // Vertical
            for (int v = -1; v < 2; v += 2)
            {
                point3D = new Point3D((x / editor.TileSize), editor.InvertHeight((y + editor.TileSize * v) / editor.TileSize), z);
                if (!editor.Tiles.ContainsKey(point3D))
                    under = "";
                else
                    under = ((BitmapImage)(editor.Tiles[point3D].Fill.GetValue(ImageBrush.ImageSourceProperty))).UriSource.ToString().Split('\\').Last();
                if (otherTile == "" || under == otherTile)
                    Fill(tile, (int)point3D.X * editor.TileSize, (int)editor.InvertHeight(float.Parse((point3D.Y).ToString())) * editor.TileSize, z, false, otherTile);
            }

        }

        private void CnvMap_MouseDown(object sender, MouseButtonEventArgs e)
        {
            int x = (int)e.GetPosition(cnvMap).X;
            int y = (int)e.GetPosition(cnvMap).Y;

            if (e.LeftButton == MouseButtonState.Pressed)
            {
                if (cnvState == CanvasState.EntityMode)
                {
                    UpdateSelectedMapTile(x, y);
                }
                else if ((cnvState == CanvasState.NormalMode || cnvState == CanvasState.TileAttributeMode) && x > 0 && x < cnvMap.Width && y > 0 && y < cnvMap.Height)
                {
                    if (fill)
                        Fill(selTile, x, y, slrDepth.Value, true, "");
                    else
                        SetMapTile(x, y, selTile);
                }
            }
            else if (e.RightButton == MouseButtonState.Pressed)
            {
                contextMenuX = x / editor.TileSize;
                contextMenuY = y / editor.TileSize;
                FrameworkElement fe = e.Source as FrameworkElement;
                if (cnvState == CanvasState.EntityMode)
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
                else if (fe.ContextMenu != null)
                    fe.ContextMenu.Items.Clear();
            }

            if (cnvState == CanvasState.EntityMode && e.ClickCount == 2 && (x / editor.TileSize) == clickprevx && (y / editor.TileSize) == clickprevy)
            {
                SetMapTile(x, y, selTile);
            }

            clickprevx = x / editor.TileSize;
            clickprevy = y / editor.TileSize;
        }

        private void CnvTilePicker_MouseDown(object sender, MouseButtonEventArgs e)
        {
            int x = (int)e.GetPosition(cnvTilePicker).X;
            int y = (int)e.GetPosition(cnvTilePicker).Y;

            selTile = "";
            selectedPickerTile.RenderTransform = new TranslateTransform((x / editor.TileSize) * editor.TileSize, (y / editor.TileSize) * editor.TileSize);
            // -1 at the end because of the empty tile at the beginning for erasing
            int index = (x / editor.TileSize) + ((y / editor.TileSize) * ((int)cnvTilePicker.Width / editor.TileSize)) - 1;

            // DONT CRASH PLZZ
            if (index >= editor.Brushes.Count || index < 0)
                return;

            selTile = editor.Brushes.ElementAt(index).Key;
        }

        // Sets up the images for the sprite selecter
        private void SetupTileSelecter()
        {
            int offx = editor.TileSize;
            int offy = 0;
            int width = (int)cnvTilePicker.Width;
            foreach (var x in editor.Brushes)
            {
                Rectangle r = new Rectangle
                {
                    Width = editor.TileSize,
                    Height = editor.TileSize,
                    Fill = x.Value,
                    RenderTransform = new TranslateTransform(offx, offy)
                };

                if (x.Key.Contains("entity"))
                {
                    r.StrokeThickness = 2;
                    r.Stroke = new SolidColorBrush(Colors.OrangeRed);
                }

                cnvTilePicker.Children.Add(r);
                offx += editor.TileSize;
                if (offx >= width)
                {
                    offx = 0;
                    offy += editor.TileSize;
                }
            }

            cnvTilePicker.Height = offy + editor.TileSize;
        }

        private void SetMapTile(int x, int y, string brush)
        {
            mustSave = true;
            x /= editor.TileSize;
            y /= editor.TileSize;

            if (cnvState == CanvasState.EntityMode)
            {
                EntitySelected(x, y, slrDepth.Value);
                return;
            }

            // Dont want to spam add a tile on the same spot
            if (x == prevx && y == prevy)
                return;
            prevx = x;
            prevy = y;
            if (cnvState == CanvasState.NormalMode)
            {
                var t = editor.AddTile(x, y, slrDepth.Value, brush);
                if (cnvMap.Children.Contains(t.Item2))
                    cnvMap.Children.Remove(t.Item2);
                if (t.Item1 != null)
                    cnvMap.Children.Add(t.Item1);
            }
            else if (cnvState == CanvasState.TileAttributeMode)
            {
                Point3D key = new Point3D(x, editor.InvertHeight(y), slrDepth.Value);
                if (editor.Tiles.ContainsKey(key))
                {
                    if (editor.Tiles[key].Tag != null)
                    {
                        editor.Tiles[key].Stroke = null;
                        editor.Tiles[key].Tag = null;
                    }
                    else
                    {
                        editor.Tiles[key].Tag = "noWalkOn";
                        editor.Tiles[key].Stroke = new SolidColorBrush(Colors.Red);
                    }
                }
            }
        }

        private void EntitySelected(int x, int y, double z)
        {
            Point3D point = new Point3D(x, editor.InvertHeight(y), z);
            if (editor.Entities.ContainsKey(point))
            {
                // Load up that entity into the new window
                EntityWindow window = new EntityWindow(point, editor.Entities, editor.Height, this, editor);
                bool? result = window.ShowDialog();
                ReSyncOnEditor();
            }
            else
            {
                // Open entity window
                EntityWindow window = new EntityWindow(point, editor.Entities, editor.Height, this, editor);
                window.ShowDialog();

                if (window.Entity_.id == -1)
                    return;

                Rectangle rectangle = editor.EntityRectangle(window.Entity_);

                editor.Entities.Add(point, new Tuple<Entity, Rectangle>(window.Entity_, rectangle));
                cnvMap.Children.Add(rectangle);
            }
        }

        private void CnvMap_MouseMove(object sender, MouseEventArgs e)
        {

            if (e.LeftButton == MouseButtonState.Pressed)
            {
                int x = (int)e.GetPosition(cnvMap).X;
                int y = (int)e.GetPosition(cnvMap).Y;

                //prevx = x / editor.TileSize;
                //prevy = y / editor.TileSize;

                if (cnvState == CanvasState.NormalMode && x > 0 && x < cnvMap.Width && y > 0 && y < cnvMap.Height)
                    SetMapTile(x, y, selTile);
            }
        }

        private void ReSyncOnEditor()
        {
            cnvMap.Children.Clear();

            // Draw the tiles
            foreach (var x in editor.Tiles)
            {
                ((TranslateTransform)x.Value.RenderTransform).Y = (float)(editor.InvertHeight((float)x.Key.Y)) * editor.TileSize;
                cnvMap.Children.Add(x.Value);
            }

            // Draw the entities
            if (cnvState == CanvasState.EntityMode)
                foreach (var x in editor.Entities)
                {
                    ((TranslateTransform)x.Value.Item2.RenderTransform).Y = (float)(editor.InvertHeight((float)x.Key.Y)) * editor.TileSize;
                    cnvMap.Children.Add(x.Value.Item2);
                }
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
            Editor.ChangeOpacity(cnvMap.Children, slrDepth.Value, opaqueView, cnvState);
        }


        private void ChkOpaqueView_Checked(object sender, RoutedEventArgs e)
        {
            opaqueView = chkOpaqueView.IsChecked ?? false;
            Editor.ChangeOpacity(cnvMap.Children, slrDepth.Value, opaqueView, cnvState);
        }

        private void MenuNew_Click(object sender, RoutedEventArgs e)
        {
            NewMap newMap = new NewMap();
            newMap.ShowDialog();
            if (newMap.MapWidth != -1 && newMap.Height != -1)
                Setup(newMap.MapWidth, newMap.MapHeight, true);
            prevPath = "";
        }

        private void MenuSave_Click(object sender, RoutedEventArgs e)
        {
            SaveMap();
        }

        private bool SaveQuery()
        {
            if ((prevPath != "" && mustSave) || mustSave)
            {
                MessageBoxResult mbr = MessageBoxResult.Cancel;
                mbr = MessageBox.Show("Save changes?", "Save", MessageBoxButton.YesNoCancel);
                if (mbr == MessageBoxResult.Cancel)
                    return false;
                else if (mbr == MessageBoxResult.Yes)
                    SaveMap();
            }
            return true;
        }

        private void MenuOpen_Click(object sender, RoutedEventArgs e)
        {
            if (!SaveQuery())
                return;
            FileDialog fileDialog = new OpenFileDialog();
            fileDialog.DefaultExt = ".json";
            fileDialog.Filter = "JSON files (.json)|*.json";

            bool? result = fileDialog.ShowDialog();

            if (result == true)
            {
                // Open document 
                LoadMap(fileDialog.FileName);
                //string filename = fileDialog.FileName;
                //prevPath = filename;
                //editor.Load(filename);
                //ReleaseMouse();
                //cnvMap.Height = editor.Height * editor.TileSize;
                //cnvMap.Width = editor.Width * editor.TileSize;
                //ReSyncOnEditor();
            }

            //Editor.ChangeOpacity(cnvMap.Children, slrDepth.Value, opaqueView, cnvState);
        }

        private void LoadMap(string path)
        {
            prevPath = path;
            editor.Load(path);
            ReleaseMouse();
            cnvMap.Height = editor.Height * editor.TileSize;
            cnvMap.Width = editor.Width * editor.TileSize;
            ReSyncOnEditor();

            Editor.ChangeOpacity(cnvMap.Children, slrDepth.Value, opaqueView, cnvState);
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            Point3D p = new Point3D(contextMenuX, editor.InvertHeight(contextMenuY), slrDepth.Value);
            if (editor.Entities.ContainsKey(p))
            {
                editor.Entities.Remove(p);
                ReSyncOnEditor();
                mustSave = true;
            }
        }

        private void Modify_Click(object sender, RoutedEventArgs e)
        {
            EntitySelected(contextMenuX, contextMenuY, slrDepth.Value);
        }

        private void menuChangeSize_Click(object sender, RoutedEventArgs e)
        {
            NewMap newMap = new NewMap();
            newMap.MapWidth = editor.Width;
            newMap.MapHeight = editor.Height;
            newMap.ShowDialog();
            if (newMap.MapWidth == -1 || newMap.Height == -1 ||
                ((newMap.MapWidth < editor.Width || newMap.MapHeight < editor.Height) &&
                    MessageBox.Show("The new size is smaller than your current size. Continuing may result in loss of tiles. Continue?", "Are you sure?", MessageBoxButton.YesNo) == MessageBoxResult.No))
                return;

            editor.Width = newMap.MapWidth;
            editor.Height = newMap.MapHeight;
            cnvMap.Width = editor.Width * editor.TileSize;
            cnvMap.Height = editor.Height * editor.TileSize;

            List<Point3D> list = new List<Point3D>();
            foreach (var x in editor.Tiles)
                if (x.Key.X > editor.Width - 1)
                    list.Add(x.Key);
                else if (x.Key.Y > editor.Height - 1)
                    list.Add(x.Key);

            foreach(var x in list)
                editor.Tiles.Remove(x);

            ReSyncOnEditor();
        }

        private void Window_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            // Save
            if (e.KeyboardDevice.IsKeyDown(Key.LeftCtrl) && e.KeyboardDevice.IsKeyDown(Key.S))
            {
                SaveMap();
            }

            if (cnvState == CanvasState.EntityMode)
            {
                // Entity delete
                if (!(clickprevx == -1 || clickprevy == -1))
                {
                    if (e.KeyboardDevice.IsKeyDown(Key.Left))
                    {
                        if (clickprevx > 0)
                            clickprevx -= 1;
                    }
                    else if (e.KeyboardDevice.IsKeyDown(Key.Right))
                    {
                        if (clickprevx < editor.Width - 1)
                            clickprevx += 1;
                    }
                    else if (e.KeyboardDevice.IsKeyDown(Key.Up))
                    {
                        if (clickprevy > 0)
                            clickprevy -= 1;
                    }
                    else if (e.KeyboardDevice.IsKeyDown(Key.Down))
                    {
                        if (clickprevy < editor.Height - 1)
                            clickprevy += 1;
                    }
                    UpdateSelectedMapTile();
                }

                // Entity delete
                if (e.KeyboardDevice.IsKeyDown(Key.Delete))
                {
                    Point3D point3D = new Point3D(clickprevx, editor.InvertHeight(clickprevy), slrDepth.Value);
                    if (editor.Entities.ContainsKey(point3D))
                    {
                        editor.Entities.Remove(point3D);
                        mustSave = true;
                        ReSyncOnEditor();
                    }
                }

                // Entity copy pasta
                if (e.KeyboardDevice.IsKeyDown(Key.LeftCtrl) && e.KeyboardDevice.IsKeyDown(Key.C))
                {
                    Point3D point3D = new Point3D(clickprevx, editor.InvertHeight(clickprevy), slrDepth.Value);
                    if (editor.Entities.ContainsKey(point3D))
                    {
                        entityCopy = new Entity(editor.Entities[point3D].Item1);
                    }
                }

                // Entity copy pasta
                if (e.KeyboardDevice.IsKeyDown(Key.LeftCtrl) && e.KeyboardDevice.IsKeyDown(Key.V))
                {
                    Point3D point3D = new Point3D(clickprevx, editor.InvertHeight(clickprevy), slrDepth.Value);
                    if (!editor.Entities.ContainsKey(point3D))
                    {
                        Entity ent = new Entity(entityCopy);
                        ent.x = clickprevx;
                        ent.y = editor.InvertHeight(clickprevy);
                        ent.z = (float)slrDepth.Value;
                        editor.Entities.Add(point3D, new Tuple<Entity, Rectangle>(ent, editor.EntityRectangle(ent)));
                        mustSave = true;

                        ReSyncOnEditor();
                    }
                }
            }
            else
            {
                // Undo tiling
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
                        mustSave = true;
                    }
                }

                // Redo tiling
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
                        mustSave = true;
                    }
                }
            }
        }

        private void menuSaveAs_Click(object sender, RoutedEventArgs e)
        {
            SaveAs();
        }

        private void SaveAs()
        {
            FileDialog fileDialog = new SaveFileDialog();
            fileDialog.FileName = "1";
            if (prevPath != "")
                fileDialog.FileName = prevPath;
            fileDialog.DefaultExt = ".json";
            fileDialog.Filter = "JSON files (.json)|*.json";

            bool? result;
            while ((result = fileDialog.ShowDialog()) == true)
            {
                // Open document 
                string filename = fileDialog.FileName;
                if (int.TryParse(filename.Split('\\').Last().Split('.').First(), out int temp) != true)
                {
                    MessageBox.Show("Name must be a number (ex: 1.json)");
                    continue;
                }
                editor.Serialize(filename);
                mustSave = false;
                break;
            }
        }

        private void SaveMap()
        {
            if (File.Exists(prevPath))
            {
                mustSave = false;
                editor.Serialize(prevPath);
            }
            else
                SaveAs();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            SaveQuery();
        }

        // When you type your own depth, it'll update
        private void txtDepth_PreviewKeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                Keyboard.ClearFocus();
        }

        private void SetToGray()
        {
            grdEntityMode.Background = unselectedMode;
            grdNormalMode.Background = unselectedMode;
            grdAttributeMode.Background = unselectedMode;
        }

        private void SetCnvMode(CanvasState state)
        {
            SetToGray();

            if (state == cnvState)
                state = CanvasState.NormalMode;

            cnvState = state;
            ReSyncOnEditor();
            Editor.ChangeOpacity(cnvMap.Children, slrDepth.Value, opaqueView, cnvState);

            switch (state)
            {
                case CanvasState.NormalMode:
                    grdNormalMode.Background = selectedMode;
                    break;
                case CanvasState.EntityMode:
                    grdEntityMode.Background = selectedMode;
                    break;
                case CanvasState.TileAttributeMode:
                    grdAttributeMode.Background = selectedMode;
                    break;
            }
        }

        private void imgEntityMode_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            SetCnvMode(CanvasState.EntityMode);
        }

        private void imgNormalMode_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            SetCnvMode(CanvasState.NormalMode);
        }

        private void imgAttributeMode_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            SetCnvMode(CanvasState.TileAttributeMode);
        }

        // Fill
        private void CheckBox_Checked(object sender, RoutedEventArgs e)
        {
            fill = !fill;
        }

        private void menuLocalization_Click(object sender, RoutedEventArgs e)
        {
            Localization loc = new Localization();
            loc.Show();
        }
    }
}
