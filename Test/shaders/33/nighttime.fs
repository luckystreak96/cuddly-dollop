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
	color.b *= 0.7;
	color.rg /= 4.5;

	// Contrast
	//float Contrast = 1.15;
	//color.rgb = ((color.rgb - 0.5) * max(Contrast, 0)) + 0.5;


	FragColor = color;
}
