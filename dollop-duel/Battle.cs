using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using dollop_duel.Skills;

namespace dollop_duel
{
    class Battle
    {
        LinkedList<Actor> actors = new LinkedList<Actor>();
        private Actor _hero;
        private Actor _villain;
        private Random _random = new Random(DateTime.Now.Millisecond);


        internal void Begin()
        {
            _hero = new Actor(0, 50) { Name = "Hero" };
            _villain = new Actor(1, 50) { Name = "Villain" };

            actors.AddFirst(_hero);
            actors.AddLast(_villain);

            Loop();
        }

        internal void Loop()
        {
            while (_hero.IsAlive() && _villain.IsAlive())
            {
                Actor temp = actors.First.Value;
                List<Actor> targets = new List<Actor>();
                Skill skill;

                // Turn choices
                if (temp.Team == 0)
                {
                    Console.ForegroundColor = ConsoleColor.White;
                    int choice = 0;
                    while (true)
                    {
                        if (temp.InUse != null && !temp.InUse.IsDone())
                            break;
                        choice = GetNumber("Choose skill:", temp.Skills);
                        if (temp.Skills[choice].IsReady())
                            break;
                    }
                    skill = temp.UseSkill(choice);

                    // Choose a target if necessary
                    if (temp.InUse.MustTarget)
                    {
                        List<Actor> tg = actors.Where(x => x.Team != 0).ToList();
                        int num = GetNumber("Choose target:", tg);
                        targets.Add(tg[num]);
                    }
                }
                else
                {
                    skill = temp.UseSkill(0);

                    List<Actor> tg = actors.Where(x => x.Team != temp.Team).ToList();
                    int num;
                    if (tg.Count > 0)
                    {
                        num = _random.Next(0, tg.Count);
                        targets.Add(tg[num]);
                    }
                }


                Dictionary<Actor, int> dmgs = skill.Use(targets);
                foreach (var x in dmgs)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine(x.Key.Name + ": " + x.Value);
                }
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine(temp.Name + ": " + temp.Health + "\t" + actors.Last.Value.Name + ": " + actors.Last.Value.Health + "\n");

                actors.First.Value.EndTurn();

                actors.RemoveFirst();
                actors.AddLast(temp);

                Thread.Sleep(1500);
            }

            Console.WriteLine("Battle is over!");
            Console.ReadLine();
        }

        public int GetNumber<T>(string message, List<T> list)
        {
            while (true)
            {
                Console.ForegroundColor = ConsoleColor.White;
                Console.WriteLine(message);
                Console.ForegroundColor = ConsoleColor.DarkYellow;
                for (int i = 0; i < list.Count; i++)
                {
                    Console.WriteLine(i + " - " + list[i].ToString());
                }
                Console.ForegroundColor = ConsoleColor.White;
                string x = Console.ReadLine();
                int result;
                try
                {
                    result = int.Parse(x);
                }
                catch
                {
                    Console.WriteLine("Wrong input!");
                    continue;
                }

                if (result >= list.Count || result < 0)
                    continue;
                return result;
            }
        }
    }
}
