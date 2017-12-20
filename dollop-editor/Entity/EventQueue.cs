using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public enum EventActivation { interact, touch, autorun}
    public enum FlagCondition { Value, NotValue, LessThan, GreaterThan }

    public class EventQueue
    {
        ~EventQueue()
        {
            IdManager.QueueId.UnAssign(ID);
        }
        public EventQueue()
        {
            ID = IdManager.QueueId.Assign();
            flag = "";
            flag_condition = 0;
            flag_value = 1;
            repeating = false;
            events = new List<Event>();// { new Event(1), new Event(2) };
        }

        public EventQueue(EventQueue queue)
        {
            ID = queue.id;
            flag = queue.flag;
            flag_value = queue.flag_value;
            flag_condition = queue.flag_condition;
            repeating = queue.repeating;
            activation = queue.activation;
            name = queue.name;
            events = new List<Event>();
            foreach (Event e in queue.events)
                events.Add(new Event(e));
        }
        public string name { get; set; }
        public int id { get { return ID; } set { IdManager.QueueId.UnAssign(ID); ID = value; IdManager.QueueId.ManualAssign(ID); } }
        private int ID;
        public string flag { get; set; }
        public int flag_value { get; set; }
        public int flag_condition { get; set; }
        public string activation { get; set; }
        public bool repeating { get; set; }
        public List<Event> events { get; set; }

        public override string ToString()
        {
            return this.name + ": ID " + this.id + (repeating ? " - REPEATING" : "");
        }
    }
}
