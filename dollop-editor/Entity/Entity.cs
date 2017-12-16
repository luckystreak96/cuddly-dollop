using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public class Entity
    {
        //~Entity()
        //{
        //    IdManager.EntityId.UnAssign(ID);
        //}

        public Entity()
        {
            ethereal = false;
            sprite = "";
            ID = IdManager.EntityId.Assign();
            player = false;
            x = 0;
            y = 0;
            z = 4.0f;
            queues = new List<EventQueue>();// { new EventQueue() };
        }

        public Entity(Entity entity)
        {
            ethereal = entity.ethereal;
            sprite = entity.sprite;
            ID = IdManager.EntityId.Assign();
            player = entity.player;
            x = entity.x;
            y = entity.y;
            z = entity.z;
            queues = new List<EventQueue>();
            foreach (EventQueue eq in entity.queues)
                queues.Add(new EventQueue(eq));
        }
        public void SneakyId(int id)
        {
            ID = id;
        }
        public int id { get { return ID; } set { IdManager.EntityId.UnAssign(ID); ID = value; IdManager.EntityId.ManualAssign(ID); } }
        private int ID;
        public bool player { get; set; }
        public bool ethereal { get; set; }
        public string sprite { get; set; }
        public float x { get; set; }
        public float y { get; set; }
        public float z { get; set; }
        public List<EventQueue> queues { get; set; }
    }
}
