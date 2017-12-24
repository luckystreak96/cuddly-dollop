using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_duel.Skills
{
    class DoNothing : SkillMethods, Skill
    {
        Actor User;
        List<Actor> Targets;
        Random random = new Random();

        public DoNothing(Actor user)
        {
            User = user;
            MustTarget = false;
        }

        public Dictionary<Actor, int> ApplyEffects(List<Actor> targets)
        {
            Dictionary<Actor, int> dictionary = new Dictionary<Actor, int>();
            return dictionary;
        }

        public bool IsDone()
        {
            return Done;
        }

        public Dictionary<Actor, int> Use(List<Actor> targets)
        {
            Done = true;
            return ApplyEffects(targets);
        }

    }
}
