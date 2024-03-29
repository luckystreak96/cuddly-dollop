﻿using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor.Battle
{
    public class BattleData
    {
        public Dictionary<string, Dictionary<string, CurveStyle>> curves;
        public List<PassiveSkill> passives { get; set; }
        public List<Fighter> fighters { get; set; }

        public BattleData()
        {
            passives = new List<PassiveSkill>();
            curves = new Dictionary<string, Dictionary<string, CurveStyle>>();
            fighters = new List<Fighter>();
        }

        // Ensure a proper copy
        public BattleData(BattleData data)
        {
            curves = new Dictionary<string, Dictionary<string, CurveStyle>>();
            foreach (var x in data.curves)
            {
                Dictionary<string, CurveStyle> dic = new Dictionary<string, CurveStyle>();
                foreach(var y in x.Value)
                    dic.Add(y.Key, new CurveStyle(y.Value));
                curves.Add(x.Key, dic);
            }

            passives = new List<PassiveSkill>();
            foreach (var x in data.passives)
                passives.Add(new PassiveSkill(x));

            fighters = new List<Fighter>();
            foreach(Fighter x in data.fighters)
                fighters.Add(new Fighter(x));
        }

        public static BattleData LoadBattleData(string battledata)
        {
            BattleData result = new BattleData();

            string data = File.ReadAllText(battledata);
            result = JsonConvert.DeserializeObject<BattleData>(data);

            return result;
        }

        public static void SerializeBattleData(string filename, BattleData data)
        {
            File.WriteAllText(filename, JsonConvert.SerializeObject(data, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore }).Replace("\r", "").Replace("\\r", ""));
        }
    }
}
