#ifndef POINT_LIGHT_EFFECT_H__
#define POINT_LIGHT_EFFECT_H__

#include "effect.h"
#include "vector3f.h"
#include <limits.h>
#include "mat4f.h"


class PointLightEffect : public Effect
{
public:
	static const unsigned int MAX_POINT_LIGHTS = 2;

	PointLightEffect();
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetEyeWorldPos(const Vector3f& EyeWorldPos);
	void SetPointLights(unsigned int NumLights, const PointLight* pLights);
	void Move(Mat4f* mat);
	void SetLightWVP(const Mat4f& LightWVP);
	void SetTextureUnit(unsigned int TextureUnit);
	void SetShadowMapTextureUnit(unsigned int TextureUnit);
	Vector3f GetPointLightLocation(int index);
private:
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;
	PointLight* m_pointLights = NULL;

	GLuint m_LightWVPLocation;
	GLuint m_shadowMapLocation;
	GLuint m_textureLocation;

	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
	GLuint m_samplerLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_dirLightLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct
		{
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];
};

#endif