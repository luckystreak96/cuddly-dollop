using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class LocalizationLoader
    {

        public static List<string> Languages = new List<string>() { "english", "french" };
        public static string path = "../../../Test/res/data/strings/localization.json";

        public static Dictionary<string, Dictionary<string, string>> LoadLocalization()
        {
            Dictionary<string, Dictionary<string, string>> Strings;
              string data;
            if (File.Exists(path))
                data = File.ReadAllText(path);
            else
                data = "";
            Strings = JsonConvert.DeserializeObject<Dictionary<string, Dictionary<string, string>>>(data);
            if (Strings == null)
                Strings = new Dictionary<string, Dictionary<string, string>>();

            return Strings;
        }

    }
}
