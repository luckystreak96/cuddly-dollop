#version 330 core

out vec4 FragColor;
in vec2 TexCoord0;

uniform sampler2D gSampler;

uniform int horizontal = 1;

float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
float weight[3] = float[] (0.2270270270, 0.3162162162, 0.0702702703);
//float weight[3] = float[] (0.2270270270, 0.2262162162, 0.2202702703);

void main()
{
    vec2 size = textureSize(gSampler, 0);
    vec2 tex_offset = 1.0 / size; // gets size of single texel
    //if(texture(gSampler, TexCoord0).a < 0.2)
    //  discard;
    vec3 result = texture(gSampler, TexCoord0).rgb * weight[0]; // current fragment's contribution
    if(horizontal == 1)
    {
        for(int i = 1; i < 3; ++i)
        {
            result += texture(gSampler, TexCoord0 + vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
            result += texture(gSampler, TexCoord0 - vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 3; ++i)
        {
            result += texture(gSampler, TexCoord0 + vec2(0.0, tex_offset.y + offset[i] / size.y)).rgb * weight[i];
            result += texture(gSampler, TexCoord0 - vec2(0.0, tex_offset.y + offset[i] / size.y)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
