using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Media3D;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Controls;
using System.IO;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace dollop_editor
{
    public class Editor
    {
        public Dictionary<string, ImageBrush> Brushes { get; }
        public Dictionary<Point3D, Rectangle> Tiles { get; set; }
        public Dictionary<Point3D, Tuple<Entity, Rectangle>> Entities { get; set; }
        public List<Dictionary<Point3D, Rectangle>> TileHistory { get; set; }
        public int HistoryProgress { get; set; }
        public int Width { get; set; }
        public int Height { get; set; }
        public int TileSize { get; set; }

        public Editor()
        {
            Brushes = new Dictionary<string, ImageBrush>();
            Tiles = new Dictionary<Point3D, Rectangle>();
            TileHistory = new List<Dictionary<Point3D, Rectangle>>();
            Entities = new Dictionary<Point3D, Tuple<Entity, Rectangle>>();
            HistoryProgress = -1;
            Width = 0;
            Height = 0;
            TileSize = 32;
            PopulateBrushes();
            ResetTiles();
        }
        public void Setup(int width, int height)
        {
            Width = width;
            Height = height;
            ResetTiles();
        }

        public void ChangeTileSize(int size)
        {
            TileSize = size;
            foreach (var x in Tiles)
            {
                Rectangle r = x.Value;
                r.Width = TileSize;
                r.Height = TileSize;
            }
        }

        private void ResetTiles()
        {
            Tiles.Clear();
            TileHistory.Clear();
        }

        private void PopulateBrushes()
        {
            // Need to automate this to find all png files in res folder (make it res/tiles?)
            string[] files = Directory.GetFiles(@"..\..\..\Test\res\sprites", "*.png", SearchOption.TopDirectoryOnly);
            List<string> ls = new List<string>(files);
            foreach (var x in ls)
            {
                ImageBrush brush = new ImageBrush();
                string source = @"..\..\..\Test\res\" + x;
                BitmapImage bmi = new BitmapImage(new Uri(x, UriKind.Relative));
                if (bmi.PixelWidth > 32 || bmi.PixelHeight > 32)
                {
                    BitmapSource bms = new CroppedBitmap(bmi, new Int32Rect(0, 32, 32, 32));
                    brush.ImageSource = bms;
                }
                else
                    brush.ImageSource = bmi;
                Brushes.Add(x.Split('\\').Last(), brush);
            }
        }

        public Tuple<Rectangle, Rectangle> AddTile(int x, int y, double z, string brush)
        {
            // Plz dont crash
            if (Width == 0 || Height == 0 || !Brushes.ContainsKey(brush))
            {
                y = (int)InvertHeight(y);
                Rectangle rekt = null;
                if (!Brushes.ContainsKey(brush))
                {
                    Point3D point3D = new Point3D(x, y, z);
                    if (Tiles.ContainsKey(point3D))
                    {
                        rekt = Tiles[point3D];
                        Tiles.Remove(point3D);
                    }
                }
                return new Tuple<Rectangle, Rectangle>(null, rekt);
            }

            Rectangle rectangle = new Rectangle
            {
                Fill = Brushes[brush].Clone(),
                Width = TileSize,
                Height = TileSize
            };
            rectangle.SetCurrentValue(Canvas.ZIndexProperty, (int)(20 - z * 2));
            rectangle.RenderTransform = new TranslateTransform(x * 32, y * 32);

            Rectangle r = null;
            Point3D pos = new Point3D(x, (int)InvertHeight((float)y), z);
            if (Tiles.ContainsKey(pos))
            {
                r = Tiles[pos];
                Tiles.Remove(pos);
            }
            Tiles.Add(pos, rectangle);

            return new Tuple<Rectangle, Rectangle>(rectangle, r);
        }

        public static void ChangeOpacity(UIElementCollection children, double depth, bool opaqueView)
        {
            foreach (Rectangle x in children)
            {
                int zind = (int)x.GetValue(Canvas.ZIndexProperty);
                if (!opaqueView && zind != (int)((10 - depth) * 2))
                {
                    double opacity = 1 - Math.Abs((20 - depth * 2) - zind) / 10;
                    opacity -= 0.5;
                    if (x.Fill != null)
                        x.Fill.Opacity = opacity;
                    if (x.Stroke != null)
                        x.Stroke.Opacity = opacity + 0.1;
                }
                else
                {
                    if (x.Fill != null)
                        x.Fill.Opacity = 1.0;
                    if (x.Stroke != null)
                        if (opaqueView)
                            x.Stroke.Opacity = 0;
                        else
                            x.Stroke.Opacity = 1.0;
                }
            }
        }

        public void Serialize(string filename)
        {
            try
            {
                Map map = new Map();
                map.id = Convert.ToInt32(filename.Split('\\').Last().Split('.').First());

                List<Tile> tiles = new List<Tile>();
                foreach (var x in Tiles)
                {
                    Tile tile = new Tile
                    {
                        x = (float)x.Key.X,
                        // -1 because height - y on its own makes an off by 1 error
                        y = (float)x.Key.Y,
                        z = (float)x.Key.Z,
                        // Get the name of the sprite
                        sprite = ((BitmapImage)(x.Value.Fill.GetValue(ImageBrush.ImageSourceProperty))).UriSource.ToString().Split('\\').Last()
                    };

                    tiles.Add(tile);
                }

                List<Entity> ents = new List<Entity>();
                foreach (var x in Entities)
                    ents.Add(x.Value.Item1);

                map.tiles = tiles;
                map.entities = ents;
                File.WriteAllText(filename, JsonConvert.SerializeObject(map));
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }

        internal bool Load(string filename)
        {
            try
            {
                Entities.Clear();
                Tiles.Clear();
                TileHistory.Clear();

                Dictionary<Point3D, Rectangle> dictionary = new Dictionary<Point3D, Rectangle>();
                Dictionary<Point3D, Tuple<Entity, Rectangle>> entities = new Dictionary<Point3D, Tuple<Entity, Rectangle>>();

                string data = File.ReadAllText(filename);
                Map loadedMap = JsonConvert.DeserializeObject<Map>(data);

                Width = (int)loadedMap.tiles.Max(x => x.x) + 1;
                Height = (int)loadedMap.tiles.Max(x => x.y) + 1;

                // Setup the tiles in the dictionary
                foreach (Tile tile in loadedMap.tiles)
                {
                    if (tile.x < 0 || tile.y < 0 || tile.z < 0 || InvertHeight(tile.y) < 0)
                        continue;
                    Rectangle rectangle = new Rectangle
                    {
                        Fill = Brushes[tile.sprite].Clone(),
                        Width = TileSize,
                        Height = TileSize
                    };
                    rectangle.SetCurrentValue(Canvas.ZIndexProperty, GameToCanvasZ(tile.z));
                    rectangle.RenderTransform = new TranslateTransform((tile.x) * 32, InvertHeight(tile.y) * 32);
                    if (!(tile.x >= Width || /*InvertHeight(tile.y)*/tile.y >= Height))
                    {
                        Point3D point3D = new Point3D(tile.x,/* InvertHeight(tile.y)*/tile.y, tile.z);
                        if (dictionary.ContainsKey(point3D))
                            dictionary.Remove(point3D);
                        dictionary.Add(point3D, rectangle);
                    }
                }

                Tiles = dictionary;

                // Setup the entities in the dictionary
                foreach (Entity e in loadedMap.entities)
                {
                    Rectangle rectangle = EntityRectangle(e);
                    int z = GameToCanvasZ((float)Math.Floor(e.z));
                    if (!(e.x >= Width || e.y >= Height))
                        entities.Add(new Point3D(Math.Floor(e.x), (float)Math.Floor(e.y), e.z), new Tuple<Entity, Rectangle>(e, rectangle));
                }

                Entities.Clear();
                Entities = entities;

                return true;
            }
            catch (Exception e)
            {
                MessageBox.Show("Error loading file : " + e.Message);
            }

            return true;
        }

        public Rectangle EntityRectangle(Entity entity)
        {
            Rectangle rectangle = new Rectangle()
            {
                Width = TileSize,
                Height = TileSize,
                Stroke = new SolidColorBrush() { Color = entity.player ? Colors.Orange : Colors.White, Opacity = 1.0 }
            };
            rectangle.SetCurrentValue(Canvas.ZIndexProperty, GameToCanvasZ(entity.z));
            rectangle.RenderTransform = new TranslateTransform(entity.x * 32, InvertHeight(entity.y) * 32);
            if (Brushes.ContainsKey(entity.sprite))
                rectangle.Fill = Brushes[entity.sprite].Clone();

            return rectangle;
        }

        public int GameToCanvasZ(float z)
        {
            return (int)(20 - z * 2);
        }

        public float InvertHeight(float height)
        {
            return Height - 1 - height;
        }
    }
}
