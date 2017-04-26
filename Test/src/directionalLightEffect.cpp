#include "directionalLightEffect.h"

DirectionalLightEffect::DirectionalLightEffect()
{
	Init();

	light.AmbientIntensity = 0.2f;
	light.Color = Vector3f(1, 0.9f, 0.9f);
	light.DiffuseIntensity = 0.75f;
	light.Direction = Vector3f(0.1f, 1.0f, 1.0f);

	Enable();

	SetAmbientIntensity(light);
}

bool DirectionalLightEffect::Init()
{
	const char* vs = "shaders/directionalLight.vs";
	const char* fs = "shaders/directionalLight.fs";

	AddShader(vs, GL_VERTEX_SHADER);
	AddShader(fs, GL_FRAGMENT_SHADER);

	bool isFinalized = Finalize();
	assert(isFinalized == true);

	return true;
}

void DirectionalLightEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void DirectionalLightEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void DirectionalLightEffect::SetAmbientIntensity(DirectionalLight& light)
{
	glUniform3f(GetUniformLocation("gDirectionalLight.Color"), light.Color.x, light.Color.y, light.Color.z);
	glUniform1f(GetUniformLocation("gDirectionalLight.AmbientIntensity"), light.AmbientIntensity);
	glUniform3f(GetUniformLocation("gDirectionalLight.Direction"), light.Direction.x, light.Direction.y, light.Direction.z);
	glUniform1f(GetUniformLocation("gDirectionalLight.DiffuseIntensity"), light.DiffuseIntensity);
}

void DirectionalLightEffect::SetEyeWorldPos(const Vector3f& EyeWorldPos)
{
	glUniform3f(GetUniformLocation("gEyeWorldPos"), EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}