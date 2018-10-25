#version 330 core

in vec2 TexCoord0;
in vec3 Color0;
in float Alpha0;
out vec4 FragColor;

uniform sampler2D gSampler;

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

void main()
{
	vec4 color = texture2D(gSampler, TexCoord0.st);
	vec3 tempColor;

	// Modify color here
	tempColor = rgb2hsv(color.rgb);
	tempColor.g *= 0.95;
	tempColor.b *= 0.3;

	//tempColor.g *= 1.1;

	color.rgb = hsv2rgb(tempColor);

	//color.r *= 1.075;
	//color.b *= 0.9;
	//color.g *= 1.15;

	// Contrast
	//float Contrast = 1.15;
	//color.rgb = ((color.rgb - 0.5) * max(Contrast, 0)) + 0.5;


	FragColor = color;
}
