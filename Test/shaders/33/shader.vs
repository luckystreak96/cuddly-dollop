#version 330 core

// Vertex position
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Color;
layout(location = 3) in float Alpha;
// World Position
layout(location = 4) in mat4 Model;

out vec2 TexCoord0;
out vec3 Color0;
out float Alpha0;

uniform mat4 gWorld = mat4(1.0);
uniform int gType = 0;
uniform float gSize = 32;

void main()
{
	if(gType == 0) //Draw it flat
	{
		gl_Position = gWorld * transpose(Model) * vec4(Position.x * gSize, Position.y * gSize, Position.z, 1.0);
	}
	else if (gType == 1) // Dont draw flat
	{
		gl_Position = gWorld * transpose(Model) * vec4(Position.x * gSize, Position.y * gSize, ((0.2 + Position.y) * -1.4), 1.0);
	}

    TexCoord0 = TexCoord;
	Color0 = Color;
	Alpha0 = Alpha;
}
