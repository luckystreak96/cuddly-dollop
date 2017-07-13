#include "combineEffect.h"

CombineEffect::CombineEffect()
{
	Init();
}

bool CombineEffect::Init()
{
	const char* vs = "shaders/shader.vs";
	const char* fs = "shaders/combine.fs";

	AddShader(vs, GL_VERTEX_SHADER);
	AddShader(fs, GL_FRAGMENT_SHADER);
	bool isFinalized = Finalize();
	assert(isFinalized == true);

	return true;
}

void CombineEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void CombineEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void CombineEffect::SetSize(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gSize"), 1, GL_TRUE, (const GLfloat*)mat);
}

void CombineEffect::SetIntensity(float value)
{
	glUniform1f(GetUniformLocation("gIntensity"), (GLfloat)value);
}