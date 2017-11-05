using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class Event
    {
        public Event(int value)
        {
            if (value == 1)
            {
                type = "move_down";
                execution_type = "blocking";
                args = new Dictionary<string, object>() { { "id", 0 }, { "distance", 3.0 } };
            }
            else if (value == 2)
            {
                type = "move_up";
                execution_type = "blocking";
                args = new Dictionary<string, object>() { { "id", 0 }, { "distance", 3.0 } };
            }
        }
        public string type { get; set; }
        public string execution_type { get; set; }
        public Dictionary<string, object> args { get; set; }

        public override string ToString()
        {
            string result = "Type: " + type + "{";
            foreach(object x in args)
            {
                result += x.ToString() + ", ";
            }
            return result + "}";
        }
    }
}
