#version 330 core

// Vertex position
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
// World Position
layout(location = 5) in mat4 Model;
out vec2 TexCoord0;

uniform mat4 gWorld = mat4(1.0);
uniform int gType = 0;
uniform float gSize = 64;

void main()
{
	if(gType == 0) //Draw it flat
	{
		gl_Position = gWorld * transpose(Model) * vec4(Position.x * gSize, Position.y * gSize, Position.z, 1.0f);
	}
	else if (gType == 1) // Dont draw flat
	{
		gl_Position = gWorld * transpose(Model) * vec4(Position.x * gSize, Position.y * gSize, ((0.2f + Position.y) * -1.4f), 1.0f);
	}
    TexCoord0 = TexCoord;
}
