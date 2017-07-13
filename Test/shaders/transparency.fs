#version 330

in vec2 TexCoord0;
out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
		vec4 color = texture2D(gSampler, TexCoord0.st);
		color.a /= 10.0;
		FragColor = color;
}
