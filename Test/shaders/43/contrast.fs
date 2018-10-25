#version 330 core

in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);

	// Modify color here
	//color.r += 0.1;
	float Contrast = 1.2;
	color.rgb = ((color.rgb - 0.5) * max(Contrast, 0)) + 0.5;

	FragColor = color;
}
