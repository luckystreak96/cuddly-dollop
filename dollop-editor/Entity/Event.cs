using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public enum EventType { move_down, move_up, move_left, move_right,
        dialogue, call_queue, map_change, teleport, weather, particle,
        play_sound, play_bgm, set_flag, toggle_flag, add_to_flag, sprite_change }
    public enum EventExecution { blocking, async }
    public enum EventArgEventCall { id, queue_id }
    public enum EventArgMove { id, distance }
    public enum EventArgMapChange { id }
    public enum EventArgTeleport { id, x, y, z }
    public enum EventArgDialogue { choice, dialogue }
    public enum EventArgWeather { type, count }
    public enum EventArgParticle { type, count, id }
    public enum EventArgPlaySound { sound_file }
    public enum EventArgSetFlag { name, value }
    public enum EventArgSpriteChange { sprite, id }
    public enum EventArgToggleFlag { name }

    public class Event
    {
        public string type { get; set; }
        public string execution_type { get; set; }
        public Dictionary<string, object> args { get; set; }

        public Event()
        {
            type = EventType.move_down.ToString();
            execution_type = EventExecution.blocking.ToString();
            args = new Dictionary<string, object>();
        }

        public Event(Event e)
        {
            type = e.type;
            execution_type = e.execution_type;
            args = new Dictionary<string, object>();
            foreach (var o in e.args)
                args.Add(o.Key, o.Value);
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
