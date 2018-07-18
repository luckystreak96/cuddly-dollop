#version 330 core

in vec2 TexCoord0;
out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	vec4 color = texture(gSampler, TexCoord0.st);
	//float brightness = (color.y * 0.2126) + (color.x * 0.7152) + (color.z * 0.0722);
	float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
	//can also set up an if brightness < 0.8, FragColor = black
	if(brightness > 0.5)
    {
    	//color.a = brightness;
	color.rgb *= brightness;
	FragColor = color;
    }
	else
		FragColor = vec4(0.0, 0, 0, 1);
		//FragColor = color / 2;
}
