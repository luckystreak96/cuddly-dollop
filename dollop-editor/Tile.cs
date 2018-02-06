using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class Tile
    {
        public Tile()
        {
            x = y = z = 0;
            sprite = "ghost.png";
            walkOn = true;
            deco = false;
        }
        public float x;
        public float y;
        public float z;
        public string sprite;
        public bool? walkOn;
        public bool? deco;
        //public string tile_type;
    }
}
