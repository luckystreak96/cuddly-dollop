#version 300 es

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in float SpecularPower;
layout(location = 4) in float SpecularIntensity;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;
out vec3 AbsPos0;
out vec3 CameraPos0;
out float SpecularPower0;
out float SpecularIntensity0;

uniform mat4 gWorld = mat4(1.0);
uniform mat4 gModel = mat4(1.0);

void main()
{
    gl_Position = gWorld * gModel * vec4(Position, 1.0);
    TexCoord0 = TexCoord;
    Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;//0.0 cuz we dont want to translate the vec, only rotate or scale
    WorldPos0 = (gWorld * gModel * vec4(Position, 1.0)).xyz;
    AbsPos0 = (gModel * vec4(Position, 1.0)).xyz;

    SpecularPower0 = SpecularPower;
    SpecularIntensity0 = SpecularIntensity;
}
