#version 110

in vec2 TexCoord0;
varying out vec4 FragColor;

uniform sampler2D gSampler;
uniform sampler2D gSampler2;
uniform float gIntensity;

void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);
	vec4 highlightColor = texture2D(gSampler2, TexCoord0.st);

	FragColor = color + highlightColor * gIntensity;
}
