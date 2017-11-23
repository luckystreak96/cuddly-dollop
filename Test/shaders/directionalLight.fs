#version 300 es

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in float SpecularPower0;
in float SpecularIntensity0;

out vec4 FragColor;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    vec3 Direction;
    float DiffuseIntensity;
};

uniform sampler2D gSampler;
uniform DirectionalLight gDirectionalLight;
uniform vec3 gEyeWorldPos;

void main()
{
    vec4 AmbientColor = vec4(gDirectionalLight.Color * gDirectionalLight.AmbientIntensity, 1.0f);
    vec3 LightDirection = -gDirectionalLight.Direction;
    vec3 Normal = normalize(Normal0);

    float DiffuseFactor = dot(Normal, LightDirection);

    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

        if (DiffuseFactor > 0) {
            DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *
            gDirectionalLight.DiffuseIntensity *
            DiffuseFactor;

            vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
            vec3 LightReflect = normalize(reflect(gDirectionalLight.Direction, Normal));
            float SpecularFactor = dot(VertexToEye, LightReflect);
            if (SpecularFactor > 0) {
                SpecularFactor = pow(SpecularFactor, SpecularPower0);
                SpecularColor = vec4(gDirectionalLight.Color * SpecularIntensity0 * SpecularFactor, 1.0f);
            }
        }

    FragColor = texture(gSampler, TexCoord0.st) * (AmbientColor + DiffuseColor + SpecularColor - SpecularColor);
}
