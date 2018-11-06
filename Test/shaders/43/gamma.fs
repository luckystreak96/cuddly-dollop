#version 330 core

in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;

uniform vec3 gLightColor = vec3(0.9, 0.9, 0.98);//sunlight
uniform float gAmbientLight = 1.2;
uniform float gLightIntensity = 1;

// Functions
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

// Main
void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);

	if(color.a < 0.05)
		discard;


	//vec3 toneMapped = color.rgb / (color.rgb + vec3(1.0));
	const float exposure = 1.0;
	vec3 toneMapped = vec3(1.0) - exp(-color.rgb * exposure);
	color.rgb = toneMapped;

	const float gamma = 2.2;
    color.rgb = pow(color.rgb, vec3(1.0/gamma));

	
	color.rgb = rgb2hsv(color.rgb);
	//color.r -= 0.2;
	//color.b *= 0.7;
	//color.b = clamp(color.b, 0, 1);
	color.rgb = hsv2rgb(color.rgb);

	//vec3 LightColor = vec3(0.4, 0.6, 0.4);//greenlight
	//color.rgb *= LightColor;



	vec2 textureSize = textureSize(gSampler, 0);

	//if(abs(gl_FragCoord.x - textureSize.x / 2) < 10)
	//	discard;

	float a2 = pow(((textureSize.x / 2) - gl_FragCoord.x) / textureSize.x, 2);
	float b2 = pow(((textureSize.y / 2) - gl_FragCoord.y) / textureSize.y, 2);

	float distance = sqrt(a2 + b2);
	float intensity = 1 - clamp(pow(distance * 1.1, 2), 0, 1);
	color.rgb *= gAmbientLight * gLightIntensity * intensity * gLightColor;

	FragColor = color;
}
