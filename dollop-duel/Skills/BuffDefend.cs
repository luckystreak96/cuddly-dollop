using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_duel.Skills
{
    class BuffDefend : Buff
    {
        public BuffDefend(int turns)
        {
            Timer = turns;
        }

        public override int ApplyBuff(int damage)
        {
            return (int)(damage * 0.5);
        }
    }
}
