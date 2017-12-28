#version 300 es

//in vec4 Color;
in highp vec2 TexCoord0;
out highp vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
    highp vec4 color = texture(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;
    else
        color = vec4(1, 0.2, 0.2, 1.0);

	FragColor = color;
}
