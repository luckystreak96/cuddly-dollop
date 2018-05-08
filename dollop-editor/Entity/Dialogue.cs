using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class Dialogue
    {
        public int id { get; set; }
        public string text { get; set; }
        public int next { get; set; }
        public string type { get; set; }
        public List<EventQueue> queues { get; set; }

        public Dialogue()
        {
            text = "text";
            id = 0;
            next = -1;
            type = "simple";
        }

        public Dialogue(Dialogue d)
        {
            id = d.id;
            text = d.text;
            next = d.next;
            type = d.type;

            if(d.queues != null)
            {
                queues = new List<EventQueue>();
                foreach (var x in d.queues)
                    queues.Add(new EventQueue(x));
            }
        }


        public override string ToString()
        {
            return "       " + id.ToString() + "\t" + next.ToString() + "\n" + text + " (Q : " + (queues == null ? 0 : queues.Count) + ")";
        }

    }
}
