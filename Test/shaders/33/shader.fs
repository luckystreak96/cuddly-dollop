#version 330 core

in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;
//uniform vec3 gLightColor = vec3(1.0, 0.9, 0.95);//pinkish
uniform vec3 gLightColor = vec3(1.0, 0.98, 0.95);//sunlight
//uniform vec3 gLightColor = vec3(1.0, 1.2, 0.65);//greenlight
uniform float gAmbientLight = 1.2;
uniform float gLightIntensity = 1;



void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);


	color *= vec4(Color0, 1.0);
	color.a *= Alpha0;

	if(color.a < 0.1)
		discard;



	//vec2 textureSize = textureSize(gSampler, 0);

	//if(abs(gl_FragCoord.x - textureSize.x / 2) < 10)
	//	discard;

	//float a2 = pow((textureSize.x) - gl_FragCoord.x, 2);
	//float b2 = pow((textureSize.y) - gl_FragCoord.y, 2);

	//float distance = sqrt(a2 + b2);
	//float intensity = 1 - clamp(pow(distance * 0.00125, 2) - 0.000925 * distance, 0, 1);
	//color.rgb *= gAmbientLight * gLightIntensity * intensity * gLightColor;
	color.rgb *= gLightIntensity;

	FragColor = color;
}
	//color.rgb *= 1.1 - sin(gl_FragCoord.x / 50) * 0.25;

	//intensity = intensity * gLightIntensity;
	//float intensity = clamp(pow(distance / 350, -1), 0, 1);
	//intensity = 1 - clamp(/*pow(distance, 2) * 0.0000012*/0.00125 * distance + 0.0001, 0, 1);