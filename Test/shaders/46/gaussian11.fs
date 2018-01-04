#version 460

out vec4 FragColor;
in vec2 TexCoord0;

uniform sampler2D gSampler;

uniform bool horizontal;

highp float weight[6] = float[] (0.092925, 0.09173,	0.090598,	0.095977,	0.09229,	0.09500003);
//uniform float weight[6] = float[] (0.382925, 0.24173,	0.060598,	0.005977,	0.000229,	0.000003);

void main()
{
    ivec2 tex_offset = 1 / textureSize(gSampler, 0); // gets size of single texel
    //if(texture(gSampler, TexCoord0).a < 0.2)
    //  discard;
    vec3 result = texture(gSampler, TexCoord0).rgb * weight[0]; // current fragment's contribution
    if(result.r == 0.0)
        discard;
    if(horizontal)
    {
        for(int i = 1; i < 6; ++i)
        {
            result += texture(gSampler, TexCoord0 + vec2(float(tex_offset.x) * float(i), 0.0)).rgb * weight[i];
            result += texture(gSampler, TexCoord0 - vec2(float(tex_offset.x) * float(i), 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 6; ++i)
        {
            result += texture(gSampler, TexCoord0 + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(gSampler, TexCoord0 - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
