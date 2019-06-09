#version 110

//in vec4 Color;
in vec2 TexCoord0;
varying out vec4 FragColor;

uniform sampler2D gSampler;
uniform float gFade;

void main()
{
	//gl_FragCoord.z += gl_FragCoord.y * 0.000001;
	vec4 color = texture2D(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;

	//color.xyz -=  1.0 - gFade;
	float temp = clamp(gFade, 0.0, 1.0);
	color.xyz *= temp;

	FragColor = color;
}
