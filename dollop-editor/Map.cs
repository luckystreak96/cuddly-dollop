using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Shapes;

namespace dollop_editor
{
    public class Map
    {
        public int id { get; set; }
        public List<Tile> tiles { get; set; }
        public List<Entity> entities { get; set; }

        public static void SerializeMap(Dictionary<Point3D, Rectangle> Tiles, Dictionary<Point3D, Tuple<Entity, Rectangle>> Entities, string filename)
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
                    sprite = ((BitmapImage)(x.Value.Fill.GetValue(ImageBrush.ImageSourceProperty))).UriSource.ToString().Split('\\').Last(),
                    walkOn = ((string)x.Value.Tag == "noWalkOn" ? false : (bool?)null),
                    deco = ((string)x.Value.Tag == "deco" ? true : (bool?)null)
                };

                tiles.Add(tile);
            }

            List<Entity> ents = new List<Entity>();
            foreach (var x in Entities)
                ents.Add(x.Value.Item1);

            map.tiles = tiles;
            map.entities = ents;
            File.WriteAllText(filename, JsonConvert.SerializeObject(map, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore }).Replace("\r", "").Replace("\\r", ""));
        }
    }
}
