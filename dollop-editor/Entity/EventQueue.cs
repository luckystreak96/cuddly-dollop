using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public enum EventActivation { interact, touch, autorun}

    public class EventQueue
    {
        ~EventQueue()
        {
            IdManager.QueueId.UnAssign(ID);
        }
        public EventQueue()
        {
            ID = IdManager.QueueId.Assign();
            flag = 1;
            repeating = false;
            events = new List<Event>();// { new Event(1), new Event(2) };
        }

        public EventQueue(EventQueue queue)
        {
            ID = queue.id;
            flag = queue.flag;
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
        public int flag { get; set; }
        public string activation { get; set; }
        public bool repeating { get; set; }
        public List<Event> events { get; set; }

        public override string ToString()
        {
            return this.name + ": ID " + this.id + (repeating ? " - REPEATING" : "");
        }
    }
}
