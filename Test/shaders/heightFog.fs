#version 300 es

//in vec4 Color;
in highp vec2 TexCoord0;
in highp vec3 position;
in highp vec3 playerOut;
out highp vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
   //if(zpos == playerzo + 1)
   //   discard;

   highp vec3 player = playerOut;
   player.z += 1.0;

   highp vec4 color = texture2D(gSampler, TexCoord0.st);
   if(color.a < 0.2)
      discard;
   //color.x += 0.03 * ((playerzo + 1) - zpos);
   //color.y += 0.03 * ((playerzo + 1) - zpos);
   //color.z += 0.03 * ((playerzo + 1) - zpos);

   color = mix(color * 1.0/*(1.0 - (abs(0.05 * pow((player.z - position.z), 2.0)) + (abs(0.001 * pow(((player.x - position.x) + (player.y - position.y)), 2.0)))))*/, //use this for spotlight effect
      mix(vec4(0.5, 0.5, 0.5, 1.0), color, 1.0 + 0.1 * (player.z - position.z)), 0.5);

   //color.a += 0.1;

	FragColor = color;
}
