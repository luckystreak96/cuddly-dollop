#version 330 core

in vec2 TexCoord0;
out vec4 FragColor;

uniform sampler2D gSampler;
uniform sampler2D gSampler2;
uniform float gIntensity;

void main()
{
	vec4 color = texture(gSampler, TexCoord0.st);
	vec4 highlightColor = texture(gSampler2, TexCoord0.st);

	float brightness = dot(highlightColor.rgb, vec3(0.2126, 0.7152, 0.0722));

	color = color + highlightColor * gIntensity;
	//float average = (color.r + color.g + color.b) / 3.0;
	FragColor = color;//vec4(average, average, average, 1.0);
}
