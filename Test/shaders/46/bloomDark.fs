#version 460

in vec2 TexCoord0;
out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	vec4 color = texture(gSampler, TexCoord0.st);
	float brightness = (color.y * 0.1126) + (color.x * 0.3722) + (color.z * 0.6152);

	if(brightness > 0.4)
		FragColor = color;
	else
		FragColor = vec4(0.0, 0, 0, 1);
}
