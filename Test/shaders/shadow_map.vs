#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in float SpecularPower;
layout(location = 4) in float SpecularIntensity;

out vec3 WorldPos0;

uniform mat4 gWorld = mat4(1.0);
uniform mat4 gModel = mat4(1.0);

void main()
{
    vec4 pos4 = vec4(Position, 1.0);
    gl_Position = gWorld * pos4;
    WorldPos0 = (gModel * pos4).xyz;
}
