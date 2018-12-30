using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public enum Skills { Smack, Heal, Protect, Bloody_Mess };

    public class Fighter
    {
        public Fighter()
        {
            skills = new List<string>();
            passives = new List<int>();
            curve = "";
            sprite = "";
            name = "";
        }

        public Fighter(Fighter x)
        {
            name = x.name;
            sprite = x.sprite;
            curve = x.curve;
            health = x.health;
            level = x.level;
            exp = x.exp;
            skillpoints = x.skillpoints;
            dead = x.dead;
            id = x.id;
            skills = new List<string>();
            foreach (var y in x.skills)
                skills.Add(y);
            passives = new List<int>();
            foreach (var y in x.passives)
                passives.Add(y);
        }

        string SkillToString(Skills skill)
        {
            return skill.ToString().Replace('_', ' ');
        }

        public override string ToString()
        {
            return name + " -> " + curve;
        }

        public int id { get; set; }
        public string name { get; set; }
        public string sprite { get; set; }
        public string curve { get; set; }
        public int health { get; set; }
        public int level { get; set; }
        public int exp { get; set; }
        public int skillpoints { get; set; }
        public bool dead { get; set; }
        public List<string> skills { get; set; }
        public List<int> passives { get; set; }
    }
}
