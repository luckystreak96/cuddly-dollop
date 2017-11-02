using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class Map
    {
        public int id { get; set; }
        public List<Tile> tiles { get; set; }
        public List<Entity> entities { get; set; }
    }
}
