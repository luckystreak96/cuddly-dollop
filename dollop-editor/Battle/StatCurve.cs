using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public enum Stats { endurance, strength, speed, crit, defense }
    public enum Operators { power, sqrt }

    public struct CurveStyle
    {
        public string op;
        public int value;
    }

}
