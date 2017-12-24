using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_duel.Skills
{
    class ChargeAttack : SkillMethods, Skill
    {
        bool FirstTurn = true;
        Actor User;
        double Accuracy { get; set; }
        List<Actor> Targets;
        Random random = new Random();

        public ChargeAttack(Actor user)
        {
            User = user;
            Accuracy = 0.85;
            Cooldown = 0;
            MustTarget = false;
        }

        public override void Reset()
        {
            FirstTurn = true;
            Done = false;
            MustTarget = false;
        }

        public Dictionary<Actor, int> ApplyEffects(List<Actor> targets)
        {
            Dictionary<Actor, int> dictionary = new Dictionary<Actor, int>();
            if (targets.Count <= 0)
                return dictionary;
            random = new Random(DateTime.Now.Millisecond);

            foreach (var x in targets)
            {
                // Attack calculation
                int result = CalcDamage();

                // Chance to miss
                result = CalcMissChance(result, Accuracy, random);

                dictionary = AddDictionaries(dictionary, targets[0].TakeDamage(result, User));
            }

            return dictionary;
        }

        public int CalcDamage()
        {
            int interval = random.Next(User.Attack / 10 + 3);
            return (User.Attack - interval) * 3;
        }

        public bool IsDone()
        {
            return Done;
        }

        public Dictionary<Actor, int> Use(List<Actor> targets)
        {
            if(FirstTurn)
            {
                FirstTurn = !FirstTurn;
                MustTarget = true;
                Console.WriteLine("CHAAAARGINNNNNNGGGG");
                return new Dictionary<Actor, int>();
            }
            Done = true;
            Cooldown = 2;
            return ApplyEffects(targets);
        }
    }
}
