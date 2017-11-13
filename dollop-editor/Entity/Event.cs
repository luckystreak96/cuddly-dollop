using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public enum EventType { move_down, move_up, move_left, move_right, dialogue, call_queue, map_change }
    public enum EventExecution { blocking, async }
    public enum EventArgEventCall { id, queue_id }
    public enum EventArgMove { id, distance }
    public enum EventArgMapChange { id }
    public enum EventArgDialogue { choice, dialogue }

    public class Event
    {
        public string type { get; set; }
        public string execution_type { get; set; }
        public Dictionary<string, object> args { get; set; }

        public Event(/*int value*/)
        {
            type = EventType.move_down.ToString();
            execution_type = EventExecution.blocking.ToString();
            args = new Dictionary<string, object>();
            //if (value == 1)
            //{
            //    type = "move_down";
            //    execution_type = "blocking";
            //    args = new Dictionary<string, object>() { { "id", 0 }, { "distance", 3.0 } };
            //}
            //else if (value == 2)
            //{
            //    type = "move_up";
            //    execution_type = "blocking";
            //    args = new Dictionary<string, object>() { { "id", 0 }, { "distance", 3.0 } };
            //}
        }

        public override string ToString()
        {
            string result = "Type: " + type + "{";
            foreach (object x in args)
            {
                result += x.ToString() + ", ";
            }
            return result + "}";
        }
    }
}
