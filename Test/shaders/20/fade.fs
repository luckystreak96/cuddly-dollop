#version 300 es

//in vec4 Color;
in highp vec2 TexCoord0;
out highp vec4 FragColor;

uniform sampler2D gSampler;
uniform highp float gFade;

void main()
{
	//gl_FragCoord.z += gl_FragCoord.y * 0.000001;
	highp vec4 color = texture(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;

	//color.xyz -=  1.0 - gFade;
	highp float temp = clamp(gFade, 0.0, 1.0);
	color.xyz *= temp;

	FragColor = color;
}
