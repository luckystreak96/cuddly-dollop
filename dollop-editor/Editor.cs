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

        public void Serialize(string filename)
        {
            try
            {

                List<Tile> tiles = new List<Tile>();

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

                    tiles.Add(tile);
                }

                List<Entity> ents = new List<Entity>();
                foreach (var x in Entities)
                    ents.Add(x.Value.Item1);

                string data = "";
                if (File.Exists(filename))
                {
                    // The tiles data
                    string json = JsonConvert.SerializeObject(new Tiles() { tiles = tiles });
                    string entityjson = JsonConvert.SerializeObject(new Entities() { entities = ents });

                    // The existing data file
                    data = File.ReadAllText(filename);
                    JObject o = JObject.Parse(data);

                    JObject map;
                    if (o["maps"] != null)
                        map = o["maps"][0] as JObject;
                    else
                        map = o;

                    if (map["tiles"] == null)
                        map.Property("id").AddAfterSelf(new JProperty("tiles", JObject.Parse(json)["tiles"]));
                    else
                        map["tiles"].Replace(JToken.Parse(json)["tiles"]);

                    if (map["entities"] == null)
                        map.Property("id").AddAfterSelf(new JProperty("entities", JObject.Parse(entityjson)["entities"]));
                    else
                        map["entities"].Replace(JToken.Parse(entityjson)["entities"]);

                    string result = JsonConvert.SerializeObject(o);
                    File.WriteAllText(filename, result);
                }
                else
                {
                    Map newMap = new Map() { id = int.Parse(filename.Split('\\').Last().Split('.').First()), entities = ents, tiles = tiles };
                    string result = JsonConvert.SerializeObject(newMap);
                    File.WriteAllText(filename, result);
                }
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
                Dictionary<Point3D, Rectangle> dictionary = new Dictionary<Point3D, Rectangle>();
                string data = File.ReadAllText(filename);

                JObject o = JObject.Parse(data);

                JObject map;
                if (o["maps"] != null)
                    map = o["maps"][0] as JObject;
                else
                    map = o;

                // If the tiles dont exist, dont try to load them
                if (!(map["tiles"] == null))
                {
                    // Load the tiles
                    string text = map["tiles"].ToString();
                    JsonTextReader reader = new JsonTextReader(new StringReader(text));

                    Tile tile = new Tile();
                    // Read all the tiles
                    while (reader.Read())
                    {
                        if (reader.TokenType == JsonToken.EndObject)
                        {
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

                        if (reader.TokenType == JsonToken.PropertyName)
                        {
                            if (reader.Value.ToString() == "x" && reader.Read())
                                tile.x = (float)(double)reader.Value;
                            if (reader.Value.ToString() == "y" && reader.Read())
                                tile.y = (float)(double)reader.Value;
                            if (reader.Value.ToString() == "z" && reader.Read())
                                tile.z = (float)(double)reader.Value;
                            if (reader.Value.ToString() == "sprite" && reader.Read())
                                tile.sprite = (string)reader.Value;
                        }
                    }
                    Tiles = dictionary;
                }


                // If the entities dont exist, dont try to load them
                if (!(map["entities"] == null))
                {
                    // Entity dictionary
                    Dictionary<Point3D, Tuple<Entity, Rectangle>> entities = new Dictionary<Point3D, Tuple<Entity, Rectangle>>();

                    // Load the tiles
                    string text = map["entities"].ToString();
                    JsonTextReader reader = new JsonTextReader(new StringReader(text));

                    Entity entity = new Entity();

                    // Read all the tiles
                    bool hasBegun = false;
                    while (reader.Read())
                    {
                        if (reader.TokenType == JsonToken.EndObject && hasBegun)
                        {
                            hasBegun = false;
                            Rectangle rectangle = EntityRectangle(entity);
                            int z = (int)(20 - Math.Floor(entity.z) * 2);
                            if (!(entity.x >= Width || Height - 1 - entity.y >= Height))
                                entities.Add(new Point3D(Math.Floor(entity.x), Height - 1 - (Math.Floor(entity.y)), entity.z), new Tuple<Entity, Rectangle>(entity, rectangle));
                            entity = new Entity();
                        }

                        if (reader.TokenType == JsonToken.PropertyName)
                        {
                            hasBegun = true;
                            if (reader.Value.ToString() == "x" && reader.Read())
                                entity.x = (float)(double)reader.Value;
                            else if (reader.Value.ToString() == "y" && reader.Read())
                                entity.y = (float)(double)reader.Value;
                            else if (reader.Value.ToString() == "z" && reader.Read())
                                entity.z = (float)(double)reader.Value;
                            else if (reader.Value.ToString() == "id" && reader.Read())
                                entity.id = int.Parse(reader.Value.ToString());
                            else if (reader.Value.ToString() == "player" && reader.Read())
                                entity.player = (bool)reader.Value;
                            else if (reader.Value.ToString() == "sprite" && reader.Read())
                                entity.sprite = reader.Value.ToString();
                        }
                    }
                    Entities = entities;
                }
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
                Stroke = new SolidColorBrush() { Color = Colors.White, Opacity = 1.0 }
            };
            rectangle.SetCurrentValue(Canvas.ZIndexProperty, (int)(20 - entity.z * 2));
            rectangle.RenderTransform = new TranslateTransform(entity.x * 32, (Height - 1 - entity.y) * 32);
            if (Brushes.ContainsKey(entity.sprite))
                rectangle.Fill = Brushes[entity.sprite].Clone();

            return rectangle;
        }
    }
}
