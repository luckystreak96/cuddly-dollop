#version 330

in vec2 TexCoord0;
out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);
	float brightness = (color.y * 0.2126) + (color.x * 0.7152) + (color.z * 0.0722);
	//can also set up an if brightness < 0.8, FragColor = black
	//if(brightness > 0.7)
	if(color.x > 0.8 || color.y > 0.8 || color.z > 0.8)
		FragColor = color;
	else
		FragColor = vec4(0.0, 0, 0, 1);
}
