#version 330

//in vec4 Color;
in vec2 TexCoord0;
out vec4 FragColor;

uniform sampler2D gSampler;
uniform float gFade = 0.0;

void main()
{
	//gl_FragCoord.z += gl_FragCoord.y * 0.000001;
	vec4 color = texture2D(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;

	color.xyz *= gFade;

	FragColor = color;
}