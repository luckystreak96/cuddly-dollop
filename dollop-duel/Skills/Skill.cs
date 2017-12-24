using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_duel.Skills
{
    interface Skill
    {
        Dictionary<Actor, int> Use(List<Actor> targets);
        Dictionary<Actor, int> ApplyEffects(List<Actor> targets);
        bool IsDone();
        void TurnEnd();
        bool IsReady();
        void Reset();
    }
}
