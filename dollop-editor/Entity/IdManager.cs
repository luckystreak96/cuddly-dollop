using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class IdManager
    {
        private static IdManager entityId = new IdManager();
        private static IdManager queueId = new IdManager();
        public static IdManager EntityId { get { return entityId; } }
        public static IdManager QueueId { get { return queueId; } }
        public HashSet<int> Values { get; set; }

        public IdManager()
        {
            Values = new HashSet<int>();
        }

        public int Assign()
        {
            for (int i = 1; i < 99999; i++)
                if (!Values.Contains(i))
                {
                    Values.Add(i);
                    return i;
                }

            return -1;
        }

        public void UnAssign(int value)
        {
            if (Values.Contains(value))
                Values.Remove(value);
        }

        public void ManualAssign(int id)
        {
            if (id == -1)
                return;
            if (!Values.Contains(id))
                Values.Add(id);
        }

        public void Reset()
        {
            Values.Clear();
        }
    }
}
