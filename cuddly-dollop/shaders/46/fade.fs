#version 460

//in vec4 Color;
in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;
uniform float gFade;

void main()
{
	vec4 color = texture(gSampler, TexCoord0.st);
	color *= vec4(Color0, 1.0);
	color.a *= Alpha0;
	if(color.a < 0.1)
		discard;

	float temp = clamp(gFade, 0.0, 1.0);
	color.xyz *= temp;

	FragColor = color;
}
