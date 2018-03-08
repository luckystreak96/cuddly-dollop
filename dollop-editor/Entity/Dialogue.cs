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

        public override string ToString()
        {
            return "       " + id.ToString() + "\t" + next.ToString() + "\n" + text + " (Q : " + (queues == null ? 0 : queues.Count) + ")";
        }

    }
}
