#version 110

// Vertex position
in vec3 Position;
in vec2 TexCoord;
// World Position
in mat4 Model;
varying out vec2 TexCoord0;

uniform mat4 gWorld;
uniform int gType;
uniform float gSize;

mat4 transpose(mat4 inMatrix) {
    vec4 i0 = inMatrix[0];
    vec4 i1 = inMatrix[1];
    vec4 i2 = inMatrix[2];
    vec4 i3 = inMatrix[3];

    mat4 outMatrix = mat4(
                 vec4(i0.x, i1.x, i2.x, i3.x),
                 vec4(i0.y, i1.y, i2.y, i3.y),
                 vec4(i0.z, i1.z, i2.z, i3.z),
                 vec4(i0.w, i1.w, i2.w, i3.w)
                 );

    return outMatrix;
}

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
}
