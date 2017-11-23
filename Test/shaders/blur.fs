#version 300 es

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoord0;
//out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	vec4 color = texture(gSampler, TexCoord0.st);
	if(color.a < 0.2)
		discard;

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0)
		BrightColor = vec4(FragColor.rgb, color.a);

	FragColor = color;
}
