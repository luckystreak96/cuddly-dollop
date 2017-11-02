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
                brush.ImageSource = new BitmapImage(new Uri(x, UriKind.Relative));
                Brushes.Add(x.Split('\\').Last(), brush);
            }
        }

        public Tuple<Rectangle, Rectangle> AddTile(int x, int y, double z, string brush)
        {
            // Plz dont crash
            if (Width == 0 || Height == 0 || !Brushes.ContainsKey(brush))
            {
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
            Point3D pos = new Point3D(x, y, z);
            if (Tiles.ContainsKey(pos))
            {
                r = Tiles[pos];
                Tiles.Remove(pos);
            }
            Tiles.Add(pos, rectangle);

            return new Tuple<Rectangle, Rectangle>(rectangle, r);
        }

        public void Serialize()
        {
            Tiles tiles = new Tiles();

            foreach (var x in Tiles)
            {
                Tile tile = new Tile
                {
                    x = (float)x.Key.X,
                    // -1 because height - y on its own makes an off by 1 error
                    y = Height - (float)x.Key.Y - 1,
                    z = (float)x.Key.Z,
                    sprite = ((BitmapImage)(x.Value.Fill.GetValue(ImageBrush.ImageSourceProperty))).UriSource.ToString().Split('\\').Last()
                };

                tiles.tiles.Add(tile);
            }


            string json = JsonConvert.SerializeObject(tiles);

            string data = File.ReadAllText("../../../Test/res/data/data.json");
            JObject o = JObject.Parse(data);
            var map = o["maps"][0] as JObject;
            //if(!map["tiles"].)
            if (map["tiles"] == null)
                map.Property("id").AddAfterSelf(new JProperty("tiles", JObject.Parse(json)["tiles"]));
            else
                map["tiles"].Replace(JToken.Parse(json)["tiles"]);

            string result = JsonConvert.SerializeObject(o);
            File.WriteAllText("../../../Test/res/data/data.json", result);
        }

        internal bool Load()
        {
            try
            {
                Dictionary<Point3D, Rectangle> dictionary = new Dictionary<Point3D, Rectangle>();
                string data = File.ReadAllText("../../../Test/res/data/data.json");

                JObject o = JObject.Parse(data);
                var map = o["maps"][0] as JObject;

                // If the tiles dont exist, dont try to load them
                if (map["tiles"] == null)
                    return false;

                // Load the tiles
                string text = map["tiles"].ToString();
                JsonTextReader reader = new JsonTextReader(new StringReader(text));

                Tile tile = new Tile();
                int counter = 0;
                // Read all the tiles
                while (reader.Read())
                {
                    // every 4 properties, add to list and reset counter
                    if (counter >= 4)
                    {
                        counter = 0;
                        Rectangle rectangle = new Rectangle
                        {
                            Fill = Brushes[tile.sprite].Clone(),
                            Width = TileSize,
                            Height = TileSize
                        };
                        rectangle.SetCurrentValue(Canvas.ZIndexProperty, (int)(20 - tile.z * 2));
                        rectangle.RenderTransform = new TranslateTransform((tile.x) * 32, (Height - 1 - tile.y) * 32);
                        if (!(tile.x >= Width || Height - 1 - tile.y >= Height))
                            dictionary.Add(new Point3D(tile.x, (Height - 1 - tile.y), tile.z), rectangle);
                    }

                    if (reader.TokenType == JsonToken.PropertyName && reader.Value.ToString() == "x")
                    {
                        reader.Read();
                        tile.x = (float)(double)reader.Value;
                        counter++;
                    }

                    if (reader.TokenType == JsonToken.PropertyName && reader.Value.ToString() == "y")
                    {
                        reader.Read();
                        tile.y = (float)(double)reader.Value;
                        counter++;
                    }

                    if (reader.TokenType == JsonToken.PropertyName && reader.Value.ToString() == "z")
                    {
                        reader.Read();
                        tile.z = (float)(double)reader.Value;
                        counter++;
                    }

                    if (reader.TokenType == JsonToken.PropertyName && reader.Value.ToString() == "sprite")
                    {
                        reader.Read();
                        tile.sprite = (string)reader.Value;
                        counter++;
                    }
                }
                Tiles = dictionary;
            }
            catch (Exception e)
            {
                MessageBox.Show("Error loading file : " + e.Message);
            }

            return true;
        }
    }
}
