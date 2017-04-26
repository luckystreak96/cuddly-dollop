#version 330

//layout(early_fragment_tests) in;

const int MAX_POINT_LIGHTS = 1;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;
in float SpecularPower0;
in float SpecularIntensity0;
in vec3 AbsPos0;
in vec3 CameraPos0;

out vec4 FragColor;

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
//uniform sampler2D gSampler;
uniform DirectionalLight gDirectionalLight;
uniform vec3 gEyeWorldPos;
uniform samplerCube gShadowMap;
uniform sampler2D gColorMap;

#define EPSILON 0.05

float CalcShadowFactor(vec3 LightDirection)
{
float shadow = 0.0;
float bias = 0.05;
float samples = 4.0;
float offset = 0.1;
//vec3 temp = AbsPos0;
//vec3 tempLight = gPointLights[0].Position;
//temp.x = AbsPos0.x * 2;
//tempLight.x = gPointLights[0].Position.x * 2;
//LightDirection = temp - tempLight;
LightDirection.z = -LightDirection.z;
//LightDirection.x /= 2;
//LightDirection.y /= 2;
//LightDirection /= 2;
float Distance = length(LightDirection);

for(float x = -offset; x < offset; x += offset / (samples * 0.5))
{
    for(float y = -offset; y < offset; y += offset / (samples * 0.5))
    {
        for(float z = -offset; z < offset; z += offset / (samples * 0.5))
        {
            float closestDepth = texture(gShadowMap, LightDirection + vec3(x, y, z)).r;
            if(Distance - bias > closestDepth)
                shadow += 0.8;
        }
    }
}
shadow /= (samples * samples * samples);
return 1.0 - shadow;
/*
    LightDirection.z = -LightDirection.z;
    float SampledDistance = texture(gShadowMap, LightDirection).r;

    float Distance = length(LightDirection);

    if (Distance <= SampledDistance + EPSILON)
        return 1.0;
    else
        return 0.0;*/
}

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal, float ShadowFactor)
{
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0f);
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0f);

        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos0);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        if (SpecularFactor > 0) {
            SpecularFactor = pow(SpecularFactor, SpecularPower0);
            SpecularColor = vec4(Light.Color * SpecularIntensity0 * SpecularFactor, 1.0f);
        }
    }

    //return (AmbientColor + ShadowFactor * (DiffuseColor + SpecularColor));
    return (ShadowFactor * (AmbientColor + (DiffuseColor + SpecularColor)));
}

vec4 CalcPointLight(int Index, vec3 Normal)
{
    vec3 LightDirection = AbsPos0 - gPointLights[Index].Position;
    float Distance = length(LightDirection);
    float ShadowFactor = CalcShadowFactor(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(gPointLights[Index].Base, LightDirection, Normal, ShadowFactor);
    float Attenuation = gPointLights[Index].Atten.Constant +
                        gPointLights[Index].Atten.Linear * Distance +
                        gPointLights[Index].Atten.Exp * Distance * Distance;

    return Color / Attenuation;
}

void main()
{
    vec3 Normal = normalize(Normal0);
    vec4 TotalLight = CalcPointLight(0, Normal);

    //for (int i = 0 ; i < gNumPointLights ; i++) {
    //    TotalLight = CalcPointLight(i, Normal);
    //}

    vec4 SampledColor = texture(gColorMap, TexCoord0.xy);

    FragColor = SampledColor * TotalLight;

    //vec3 LightDirection = AbsPos0 - gPointLights[0].Position;
    //LightDirection.z = -LightDirection.z;
    //float Depth = texture(gShadowMap, LightDirection).r;
    //FragColor = vec4(vec3(Depth / 1010), 1.0);
}
