﻿using System;
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
        public Battle.BattleData _BattleData { get; set; }

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
            _BattleData = new Battle.BattleData();
            Brushes = new Dictionary<string, ImageBrush>();
            Tiles = new Dictionary<Point3D, Rectangle>();
            TileHistory = new List<Dictionary<Point3D, Rectangle>>();
            Entities = new Dictionary<Point3D, Tuple<Entity, Rectangle>>();
            HistoryProgress = -1;
            Width = 0;
            Height = 0;
            TileSize = 64;
            PopulateBrushes();
            ResetTiles();
        }

        public void Setup(int width, int height, bool newMap)
        {
            Width = width;
            Height = height;
            ResetTiles();
            Entities.Clear();
            if (newMap)
            {
                Entity entity = new Entity()
                {
                    sprite = "",
                    y = height - 1,
                    queues = new List<EventQueue>()
                {
                    new EventQueue()
                    {
                        activation = "autorun",
                        events = new List<Event>()
                        {
                            new Event()
                            {
                                execution_type = "async", type = "play_bgm",
                                args = new Dictionary<string, object>()
                                {
                                    { "sound_file", "res/audio/bgm/washeslow.wav" }
                                }
                            },
                            new Event()
                            {
                                execution_type = "async", type = "weather",
                                args = new Dictionary<string, object>()
                                {
                                    { "type", "snow" },
                                    { "count", 60 }
                                }
                            }
                        }
                    }
                }
                };
                Entities.Add(new Point3D(0, height - 1, 4.0), new Tuple<Entity, Rectangle>(entity, EntityRectangle(entity)));
            }
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

        public void PopulateBrushes()
        {
            Brushes.Clear();
            // Need to automate this to find all png files in res folder (make it res/tiles?)
            string[] files = Directory.GetFiles(@"..\..\..\Test\res\sprites\tiles", "*.png", SearchOption.TopDirectoryOnly);
            List<string> ls = new List<string>(files);
            ls.AddRange(Directory.GetFiles(@"..\..\..\Test\res\sprites\entities", "*.png", SearchOption.TopDirectoryOnly));
            foreach (var x in ls)
            {
                ImageBrush brush = new ImageBrush();
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
            rectangle.SetCurrentValue(Canvas.ZIndexProperty, GameToCanvasZ((float)z));
            rectangle.RenderTransform = new TranslateTransform(x * TileSize, y * TileSize);

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

        public static void ChangeOpacity(UIElementCollection children, double depth, bool opaqueView, CanvasState state)
        {
            foreach (Rectangle x in children)
            {
                int zind = (int)x.GetValue(Canvas.ZIndexProperty);

                // If the tile isnt at the currently selected height
                if (!opaqueView && zind != (int)(100 - depth * 10.0))
                {
                    double opacity = 1 - Math.Abs((100 - depth * 10.0) - zind) / 100;
                    opacity -= 0.5;
                    if (x.Fill != null)
                        x.Fill.Opacity = opacity;
                    //if (x.Stroke != null)
                    //    x.Stroke.Opacity = opacity + 0.1;
                    //if (state == CanvasState.TileAttributeMode)
                    //{
                    //    if ((string)x.Tag == "noWalkOn")
                    //        x.Stroke = new SolidColorBrush(Colors.Red);
                    //    else if ((string)x.Tag == "deco")
                    //        x.Stroke = new SolidColorBrush(Colors.Blue);
                    //}
                    //else if ((string)x.Tag != null)
                    if(x.Stroke != null)
                        x.Stroke.Opacity = 0;
                }
                else
                {
                    if ((string)x.Tag != null)
                        x.Stroke = null;
                    if (x.Fill != null)
                        x.Fill.Opacity = 1.0;
                    if (x.Stroke != null)
                        if (opaqueView)
                            x.Stroke.Opacity = 0;
                        else
                            x.Stroke.Opacity = 1.0;
                    if (state == CanvasState.TileAttributeMode && !opaqueView)
                    {
                        if ((string)x.Tag == "noWalkOn")
                            x.Stroke = new SolidColorBrush(Colors.Red);
                        else if ((string)x.Tag == "deco")
                            x.Stroke = new SolidColorBrush(Colors.Blue);
                    }
                }
            }
        }

        public void Serialize(string filename)
        {
            try
            {
                Map.SerializeMap(Tiles, Entities, filename);
                Battle.BattleData.SerializeBattleData(@"..\..\..\Test\res\data\battle\battle.json", _BattleData);
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message);
            }
        }

        public void LoadBattleData(string path)
        {
            try
            {
                _BattleData = Battle.BattleData.LoadBattleData(path);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error loading BattleData : " + ex.Message);
                _BattleData = new Battle.BattleData();
            }
        }

        internal bool Load(string filename)
        {
            try
            {
                Entities.Clear();
                Tiles.Clear();
                TileHistory.Clear();
                HistoryProgress = -1;
                IdManager.EntityId.Reset();
                IdManager.QueueId.Reset();

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
                    if (tile.deco == true)
                        rectangle.Tag = "deco";
                    else if (tile.walkOn == false)
                        rectangle.Tag = "noWalkOn";

                    if (tile.z > 3.7 && tile.z < 4.0)
                        Console.Write(tile.z);
                    rectangle.SetCurrentValue(Canvas.ZIndexProperty, GameToCanvasZ(tile.z));
                    rectangle.RenderTransform = new TranslateTransform((tile.x) * TileSize, InvertHeight(tile.y) * TileSize);
                    if (!(tile.x >= Width || /*InvertHeight(tile.y)*/tile.y >= Height))
                    {
                        // tile.z needs to be converted this way otherwise the cast makes 4.9 become 4.90000000023 xd
                        Point3D point3D = new Point3D(tile.x, tile.y, System.Convert.ToDouble(tile.z.ToString()));
                        if (dictionary.ContainsKey(point3D))
                            dictionary.Remove(point3D);
                        dictionary.Add(point3D, rectangle);
                    }
                }

                Tiles = dictionary;

                // Setup the entities in the dictionary
                foreach (Entity e in loadedMap.entities)
                {
                    IdManager.EntityId.ManualAssign(e.id);
                    foreach (EventQueue eq in e.queues)
                    {
                        IdManager.QueueId.ManualAssign(eq.id);
                    }

                    Rectangle rectangle = EntityRectangle(e);
                    //int z = GameToCanvasZ((float)Math.Floor(e.z));
                    if (!(e.x >= Width || e.y >= Height))
                    {
                        Point3D point3D = new Point3D(Math.Round(e.x), Math.Round(e.y), e.z);
                        if (!entities.ContainsKey(point3D))
                            entities.Add(point3D, new Tuple<Entity, Rectangle>(e, rectangle));
                    }
                }

                Entities = entities;

                return true;
            }
            catch (Exception e)
            {
                MessageBox.Show("Error loading file : " + e.Message + "\n" + e.StackTrace);
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
            rectangle.RenderTransform = new TranslateTransform(entity.x * TileSize, InvertHeight(entity.y) * TileSize);
            if (Brushes.ContainsKey(entity.sprite))
                rectangle.Fill = Brushes[entity.sprite].Clone();

            return rectangle;
        }

        public int GameToCanvasZ(float z)
        {
            int result = (int)(100.0f - z * 9.99f);
            return result;
        }

        public float InvertHeight(float height)
        {
            return Height - 1 - height;
        }
    }
}
