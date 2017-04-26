#version 330

//in vec4 Color;
in vec2 TexCoord0;
out vec4 FragColor;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
};

uniform sampler2D gSampler;
uniform DirectionalLight gDirectionalLight;

void main()
{
    FragColor = texture2D(gSampler, TexCoord0.st) * vec4(gDirectionalLight.Color, 1) * gDirectionalLight.AmbientIntensity;
}
