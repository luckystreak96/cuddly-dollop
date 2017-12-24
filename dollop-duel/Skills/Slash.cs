using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_duel.Skills
{
    class Slash : SkillMethods, Skill
    {
        Actor User;
        List<Actor> Targets;
        double Accuracy { get; set; }
        Random random = new Random();

        public Slash(Actor user)
        {
            User = user;
            Accuracy = 0.97;
        }

        public Dictionary<Actor, int> ApplyEffects(List<Actor> targets)
        {
            // Setup
            Dictionary<Actor, int> dictionary = new Dictionary<Actor, int>();
            if (targets.Count <= 0)
                return dictionary;
            random = new Random(DateTime.Now.Millisecond);

            foreach (var x in targets)
            {
                // Attack calculation
                int result = CalcDamage();

                // Chance to miss
                result = CalcMissChance(result, 0.97, random);

                dictionary = AddDictionaries(dictionary, targets[0].TakeDamage(result, User));
            }

            return dictionary;
        }

        public int CalcDamage()
        {
            int interval = random.Next(User.Attack / 10 + 3);
            return (User.Attack - interval);
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
