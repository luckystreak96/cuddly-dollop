#version 330 core

in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);

	//if(color.a < 0.1)
	//	discard;


	//vec3 toneMapped = color.rgb / (color.rgb + vec3(1.0));
	const float exposure = 1.0;
	vec3 toneMapped = vec3(1.0) - exp(-color.rgb * exposure);
	color.rgb = toneMapped;

	const float gamma = 2.2;
    color.rgb = pow(color.rgb, vec3(1.0/gamma));
	FragColor = color;
}
