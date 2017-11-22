#version 300 es

in highp vec2 TexCoord0;
out highp vec4 FragColor;

uniform sampler2D gSampler;
uniform sampler2D gSampler2;
uniform highp float gIntensity;

void main()
{
	highp vec4 color = texture2D(gSampler, TexCoord0.st);
	highp vec4 highlightColor = texture2D(gSampler2, TexCoord0.st);

	FragColor = color + highlightColor * gIntensity;
}
