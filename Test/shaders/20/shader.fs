#version 300 es

//in vec4 Color;
in highp vec2 TexCoord0;
out highp vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	//gl_FragCoord.z += gl_FragCoord.y * 0.000001;
	highp vec4 color = texture(gSampler, TexCoord0.st);
	if(color.a < 0.1)
		discard;

	//color.x += 0.05 * gl_FragCoord.z;
	//color.y += 0.05 * gl_FragCoord.z;
	//color.z += 0.05 * gl_FragCoord.z;

	FragColor = color;
}
