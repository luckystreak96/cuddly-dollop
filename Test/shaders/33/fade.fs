#version 330 core

//in vec4 Color;
in vec2 TexCoord0;
out vec4 FragColor;

uniform sampler2D gSampler;
uniform float gFade;

void main()
{
	vec4 color = texture(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;

	float temp = clamp(gFade, 0.0, 1.0);
	color.xyz *= temp;

	FragColor = color;
}
