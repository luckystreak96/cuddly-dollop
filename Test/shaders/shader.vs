#version 330

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
//out vec4 Color;
out vec2 TexCoord0;

uniform mat4 gWorld = mat4(1.0);
uniform mat4 gModel = mat4(1.0);

void main()
{
    gl_Position = gWorld * gModel * vec4(Position, 1.0); // the position is a vec3, so we need to add 1.0 at the end to make it 4 long for good mat4 multiplication
    //Color = vec4(clamp(Position, 0.0, 1.0) + 0.2, 1.0);
    TexCoord0 = TexCoord;
}
