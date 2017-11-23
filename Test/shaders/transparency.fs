#version 300 es

in highp vec2 TexCoord0;
out highp vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
		highp vec4 color = texture(gSampler, TexCoord0.st);
		color.a /= 10.0;
		FragColor = color;
}
