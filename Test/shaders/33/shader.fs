#version 330 core

in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;
//uniform vec3 gLightColor = vec3(1, 0.85, 0.6);
//uniform vec3 gLightColor = vec3(1.0);
//uniform vec3 gLightColor = vec3(1.0, 0.9, 0.95);//pinkish
uniform vec3 gLightColor = vec3(1.0, 0.98, 0.95);//sunlight
uniform float gLightIntensity = 0.9;

void main()
{
	clamp(gLightIntensity, 0.0, 1.0);
	vec4 color = texture2D(gSampler, TexCoord0.st);
	color *= vec4(Color0, 1.0);
	color.a *= Alpha0;

	float intensity = gLightIntensity;
	float distance = sqrt(pow(960 - gl_FragCoord.x, 2) + pow(540 - gl_FragCoord.y, 2));
	intensity = 1 - clamp(pow(distance, 2) * 0.0000003 - 0.000125 * distance + 0.001, 0, 1);
	intensity *= gLightIntensity;
	color.rgb *= intensity * gLightColor;
	//color.rgb *= 1.1 - sin(gl_FragCoord.x / 50) * 0.25;
	if(color.a < 0.1)
		discard;

	FragColor = color;
}
