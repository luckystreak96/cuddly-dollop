using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_duel.Skills
{
    class SkillMethods : Skill
    {
        public int Cooldown { get; set; }
        public bool Done { get; set; }
        public bool MustTarget { get; set; }

        public override string ToString()
        {
            return this.GetType().ToString().Split('.').Last();
        }

        public SkillMethods()
        {
            Cooldown = 0;
            Done = false;
            MustTarget = true;
        }

        public void TurnEnd()
        {
            Cooldown--;
            if (Cooldown < 0)
                Cooldown = 0;
        }

        public virtual void Reset()
        {
            Done = false;
        }

        public bool IsReady()
        {
            return Cooldown <= 0;
        }

        public int CalcMissChance(int dmg, double hitChance, Random random)
        {
            return dmg * (random.NextDouble() < hitChance ? 1 : 0);
        }

        public Dictionary<Actor, int> AddDictionaries(Dictionary<Actor, int> one, Dictionary<Actor, int> two)
        {
            foreach(var x in one)
            {
                if (!two.ContainsKey(x.Key))
                    two.Add(x.Key, x.Value);
                else
                    two[x.Key] += x.Value;
            }

            return two;
        }

        public Dictionary<Actor, int> Use(List<Actor> targets)
        {
            throw new NotImplementedException();
        }

        public Dictionary<Actor, int> ApplyEffects(List<Actor> targets)
        {
            throw new NotImplementedException();
        }

        public bool IsDone()
        {
            return Done;
        }
    }
}
