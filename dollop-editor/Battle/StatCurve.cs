using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public enum Stats { endurance, strength, speed, crit, defense, max_health }
    public enum Operators { quadratic, square_root, linear }

    public struct CurveStyle
    {
        public string op;
        public float value;
    }
}
