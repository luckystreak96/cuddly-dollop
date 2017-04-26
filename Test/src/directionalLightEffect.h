#ifndef DIRECTIONAL_LIGHT_EFFECT_H__
#define DIRECTIONAL_LIGHT_EFFECT_H__

#include "effect.h"
#include "vector3f.h"



class DirectionalLightEffect : public Effect
{
public:
	DirectionalLightEffect();
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetAmbientIntensity(DirectionalLight& light);
	void SetEyeWorldPos(const Vector3f& EyeWorldPos);
private:
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	DirectionalLight light = DirectionalLight();
};

#endif