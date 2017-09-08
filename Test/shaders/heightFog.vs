#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 5) in vec3 ModelMat;
out vec2 TexCoord0;
out vec3 position;
out vec3 playerOut;

uniform mat4 gWorld = mat4(1.0);
uniform mat4 gModel = mat4(1.0);
uniform vec3 playerPos = vec3(0.0);

void main()
{
    vec4 pos = gWorld * gModel * vec4(Position.x, Position.y, Position.z + ModelMat.y * 0.05, 1.0);
    gl_Position = pos;
    TexCoord0 = TexCoord;
    position = Position;
    playerOut = playerPos;
}
