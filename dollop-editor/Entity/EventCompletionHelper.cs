using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace dollop_editor
{
    public static class EventCompletionHelper
    {
        public static Dictionary<int, Type> eventArgDictionary = new Dictionary<int, Type>() {
            { (int)EventType.move_down, typeof(EventArgMove) },
            { (int)EventType.move_left, typeof(EventArgMove) },
            { (int)EventType.move_right, typeof(EventArgMove) },
            { (int)EventType.move_up, typeof(EventArgMove) },
            { (int)EventType.dialogue, typeof(EventArgDialogue) },
            { (int)EventType.call_queue, typeof(EventArgEventCall) },
            { (int)EventType.map_change, typeof(EventArgMapChange) },
            { (int)EventType.teleport, typeof(EventArgTeleport) },
            { (int)EventType.weather, typeof(EventArgWeather) },
            { (int)EventType.particle, typeof(EventArgParticle) },
            { (int)EventType.play_sound, typeof(EventArgPlaySound) },
            { (int)EventType.play_bgm, typeof(EventArgPlaySound) },
            { (int)EventType.set_flag, typeof(EventArgSetFlag) },
            { (int)EventType.sprite_change, typeof(EventArgSpriteChange) },
            { (int)EventType.add_to_flag, typeof(EventArgSetFlag) },
            { (int)EventType.toggle_flag, typeof(EventArgToggleFlag) },
        };
        public static Dictionary<string, string> eventArgComplexExamples = new Dictionary<string, string>() {
            { EventType.dialogue.ToString(), @"--choice 1--{
                                            ""d"": 0,
                                            ""text"": ""Can you walk down for me?"",
                                            ""next"": 2,
                                            ""queues"": [
                                                {
                                                    ""id"": 2,
                                                    ""repeating"": true,
                                                    ""events"": [
                                                        {
                                                            ""type"": ""move_down"",
                                                            ""execution_type"": ""blocking"",
                                                            ""args"": {
                                                                ""id"": 2,
                                                                ""distance"": 3.0
                                                            }
                                                        },
                                                        {
                                                            ""type"": ""move_up"",
                                                            ""execution_type"": ""blocking"",
                                                            ""args"": {
                                                                ""id"": 2,
                                                                ""distance"": 3.0
                                                            }
                                                        }
                                                    ]
                                                }
                                            ]
                                        }

                                        --""dialogue 0""--{
                                            ""id"": 0,
                                            ""text"": ""Hey man, what's up?"",
                                            ""next"": 0,
                                            ""type"": ""choice""
                                        }

                                        --""dialogue 1"":--{
                                            ""id"": 1,
                                            ""text"": ""Roses are red,\n Violets are blue,\n\n This poem is amazing,\n Unlike you!"",
                                            ""next"": 0,
                                            ""type"": ""simple""
                                        }"
            },
        };
    }
}
