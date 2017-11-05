using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class EventQueue
    {
        private static int eyedee = 0;
        public EventQueue()
        {
            id = ++eyedee;
            flag = 1;
            repeating = true;
            events = new List<Event>();// { new Event(1), new Event(2) };
        }
        public string name { get; set; }
        public int id { get; set; }
        public int flag { get; set; }
        public bool repeating { get; set; }
        public List<Event> events { get; set; }

        public override string ToString()
        {
            return this.name + ": ID " + this.id;
        }
    }
}
