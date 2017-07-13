#version 330 core

out vec4 FragColor;
in vec2 TexCoord0;

uniform sampler2D gSampler;

uniform bool horizontal = true;

uniform float weight[2] = float[] (0.44198,	0.27901);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(gSampler, 0); // gets size of single texel
    if(texture(gSampler, TexCoord0).a < 0.2)
      discard;
    vec3 result = texture(gSampler, TexCoord0).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 2; ++i)
        {
            result += texture(gSampler, TexCoord0 + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(gSampler, TexCoord0 - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 2; ++i)
        {
            result += texture(gSampler, TexCoord0 + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(gSampler, TexCoord0 - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
