#version 330 core

in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;
//uniform vec3 gLightColor = vec3(1, 0.85, 0.6);
uniform vec3 gLightColor = vec3(1.0);
uniform float gLightIntensity = 0.9;

void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);
	color *= vec4(Color0, 1.0);
	color.a *= Alpha0;

	color.rgb *= gLightIntensity * gLightColor;
	//color.rgb *= 1.1 - sin(gl_FragCoord.x / 50) * 0.25;
	if(color.a < 0.1)
		discard;

	FragColor = color;
}
