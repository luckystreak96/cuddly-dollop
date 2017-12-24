using dollop_duel.Skills;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_duel
{
    class Actor
    {
        public int Team { get; set; }
        public string Name { get; set; }
        public int Health { get; set; }
        public int Attack { get; set; }
        public SkillMethods InUse { get; set; }
        public List<SkillMethods> Skills { get; set; }
        public List<Buff> Buffs { get; set; }

        public Actor()
        {
            SetStats(1);
        }

        public Actor(int team, int level)
        {
            Team = team;
            SetStats(level);
        }

        public override string ToString()
        {
            return Name;
        }

        public SkillMethods UseSkill(int index)
        {
            if (InUse != null && !InUse.IsDone())
                return InUse;

            InUse = Skills[index];
            InUse.Reset();

            // Add do nothing skill 4 this plz :(
            return InUse;
        }

        // Calculate effects that lower dmg (or debuffs that increase it) here
        // Actor in case of thornmail
        public Dictionary<Actor, int> TakeDamage(int dmg, Actor actor)
        {
            Dictionary<Actor, int> dictionary = new Dictionary<Actor, int>();

            foreach (var x in Buffs)
                dmg = x.ApplyBuff(dmg);

            dictionary.Add(this, dmg);
            Health -= dmg;
            return dictionary;
        }

        private void SetStats(int level)
        {
            Health = 10 + (int)Math.Ceiling(Math.Pow(2, level * 0.2)) + level * 6;
            Attack = 2 + (int)Math.Ceiling(Math.Pow(2, level * 0.05)) + (int)(level * 1.5);
            Buffs = new List<Buff>();
            Skills = new List<SkillMethods>();
            Skills.Add(new Slash(this));
            Skills.Add(new ChargeAttack(this));
            Skills.Add(new Defend(this));
            Skills.Add(new DoNothing(this));
        }

        public void EndTurn()
        {
            // CD
            foreach (var x in Skills)
                x.TurnEnd();

            List<Buff> toRemove = new List<Buff>();
            // Count down
            foreach (var x in Buffs)
            {
                x.Timer--;
                if (x.Timer <= 0)
                    toRemove.Add(x);
            }

            // Remove if done
            foreach (var x in toRemove)
                Buffs.Remove(x);
        }

        public bool IsAlive()
        {
            return Health > 0;
        }
    }
}
