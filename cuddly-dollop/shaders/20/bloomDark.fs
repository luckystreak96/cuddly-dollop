#version 300 es

in highp vec2 TexCoord0;
out highp vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	highp vec4 color = texture(gSampler, TexCoord0.st);
	highp float brightness = (color.y * 0.1126) + (color.x * 0.3722) + (color.z * 0.6152);
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
