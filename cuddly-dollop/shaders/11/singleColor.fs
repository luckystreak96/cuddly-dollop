#version 110

//in vec4 Color;
in vec2 TexCoord0;
varying out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
    vec4 color = texture2D(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;
    else
        color = vec4(1, 0.2, 0.2, 1.0);

	FragColor = color;
}
