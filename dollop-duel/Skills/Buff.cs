using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_duel.Skills
{
    abstract class Buff
    {
        public int Timer;

        public abstract int ApplyBuff(int damage);
    }
}
