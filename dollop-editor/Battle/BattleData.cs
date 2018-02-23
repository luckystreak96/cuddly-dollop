using Newtonsoft.Json;
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

        public BattleData()
        {
            curves = new Dictionary<string, Dictionary<string, CurveStyle>>();
        }

        public static BattleData LoadBattleData(string filename)
        {
            BattleData result = new BattleData();

            string data = File.ReadAllText(filename);
            result = JsonConvert.DeserializeObject<BattleData>(data);

            return result;
        }

        public static void SerializeBattleData(string filename, BattleData data)
        {
            File.WriteAllText(filename, JsonConvert.SerializeObject(data, Formatting.Indented, new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore }).Replace("\r", "").Replace("\\r", ""));
        }
    }
}
