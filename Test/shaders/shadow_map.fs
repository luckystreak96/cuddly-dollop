#version 330

in vec3 WorldPos0;

out float FragColor;

uniform vec3 gLightWorldPos;

void main()
{
    vec3 LightToVertex = WorldPos0 - gLightWorldPos;
    float LightToPixelDistance = length(LightToVertex);
    FragColor = LightToPixelDistance;
    //gl_FragDepth = LightToPixelDistance;
}
