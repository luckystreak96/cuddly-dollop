#version 330

// Vertex position
layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
// World Position
layout(location = 5) in vec3 ModelMat;
out vec2 TexCoord0;
out vec3 pos;

uniform mat4 gWorld = mat4(1.0);
uniform mat4 gModel = mat4(1.0);

void main()
{
	if(gModel[3][0] == 0 && gModel[3][1] == 0 && gModel[3][2] == 0)
	{
		gl_Position = gWorld * gModel * vec4(Position.x, Position.y, Position.z, 1.0);
	}
	else
	{
		gl_Position = gWorld * gModel * vec4(Position.x, Position.y, ((-0.5 + Position.y) * -2.0), 1.0);
	}
    TexCoord0 = TexCoord;
}
