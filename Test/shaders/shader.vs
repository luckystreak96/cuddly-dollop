#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 5) in vec3 ModelMat;
out vec2 TexCoord0;

uniform mat4 gWorld = mat4(1.0);
uniform mat4 gModel = mat4(1.0);

void main()
{
    gl_Position = gWorld * gModel * vec4(Position.x, Position.y, Position.z + ModelMat.y * 0.05, 1.0);
    TexCoord0 = TexCoord;
}
