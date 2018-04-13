#version 330 core

in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);
	color *= vec4(Color0, 1.0);
	color.a *= Alpha0;
	if(color.a < 0.1)
		discard;

	FragColor = color;
}
