#version 300 es

in highp vec2 TexCoord0;
out highp vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	highp vec4 color = texture2D(gSampler, TexCoord0.st);
	highp float brightness = (color.y * 0.2126) + (color.x * 0.7152) + (color.z * 0.0722);
	//can also set up an if brightness < 0.8, FragColor = black
	if(brightness > 0.6)
    {
	//if(color.x > 0.8 || color.y > 0.8 || color.z > 0.8)
    color.a = brightness;
		FragColor = color;
        }
	else
		FragColor = vec4(0.0, 0, 0, 1);
}
