#version 110

//in vec4 Color;
in vec2 TexCoord0;
varying out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	//gl_FragCoord.z += gl_FragCoord.y * 0.000001;
	vec4 color = texture2D(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;

	//color.x += 0.05 * gl_FragCoord.z;
	//color.y += 0.05 * gl_FragCoord.z;
	//color.z += 0.05 * gl_FragCoord.z;

	FragColor = color;
}
