#version 110

in vec2 TexCoord0;
varying out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);
	float brightness = (color.y * 0.1126) + (color.x * 0.3722) + (color.z * 0.6152);
	//can also set up an if brightness < 0.8, FragColor = black
	//color.x = 1 - color.x;
	//color.y = 1 - color.y;
	//color.z = 1 - color.z;
	if(brightness > 0.4)
	//if(color.x > 0.8 && color.y > 0.4)
		FragColor = color;
	else
		FragColor = vec4(0.0, 0, 0, 1);
}
