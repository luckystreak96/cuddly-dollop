using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class Choice
    {
        public int d { get; set; }
        public int next { get; set; }
        public string text { get; set; }
        public List<EventQueue> queues { get; set; }

        public override string ToString()
        {
            return next + " " + text;
        }
    }
}
