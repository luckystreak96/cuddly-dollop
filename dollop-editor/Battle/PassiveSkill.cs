﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor.Battle
{
    public enum PassivePriority { PP_AbsoluteFirst, PP_Secondary, PP_Last, PP_BeforeSkill, PP_BeforeDamage, PP_AfterDamage, PP_AfterSkill };
    public enum PassiveType { PT_Skill, PT_SkillUpgrade, PT_Stat, PT_Special };
    public enum PassiveSpecifier { PS_Flat, PS_Percent, PS_None };

    public class PassiveSkill
    {
        public PassiveSkill()
        {
        }

        public PassiveSkill(PassiveSkill x)
        {
            id = x.id;
            name = x.name;
            priority = x.priority;
            type = x.type;
            specifier = x.specifier;
            _string = x._string;
            _int = x._int;
            _float = x._float;
        }

        public int id { get; set; }
        public string name { get; set; }
        public string priority { get; set; }
        public string type { get; set; }
        public string specifier { get; set; }
        public string _string { get; set; }
        public int _int { get; set; }
        public float _float { get; set; }

        public override string ToString()
        {
            return name + " -> " + type;
        }
    }
}
