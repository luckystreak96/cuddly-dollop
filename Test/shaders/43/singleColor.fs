#version 330 core
//in vec4 Color;
in vec2 TexCoord0;
out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
    vec4 color = texture(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;
    else
        color = vec4(0.0, 0.0, 0.0, 0.9);

	FragColor = color;
}
