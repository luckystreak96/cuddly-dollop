using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class Entity
    {
        public Entity()
        {
            sprite = "";
            id = 1;
            player = true;
            x = 0;
            y = 0;
            z = 0;
            queues = new List<EventQueue>();// { new EventQueue() };
        }
        public int id { get; set; }
        public bool player { get; set; }
        public string sprite { get; set; }
        public float x { get; set; }
        public float y { get; set; }
        public float z { get; set; }
        public List<EventQueue> queues { get; set; }
    }
}
