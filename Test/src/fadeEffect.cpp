#include "fadeEffect.h"

FadeEffect::FadeEffect()
{
	Effect::Init(m_shaderProg);
	Init();
}

bool FadeEffect::Init()
{
	const char* vs = "shaders/shader.vs";
	const char* fs = "shaders/fade.fs";

	AddShader(vs, GL_VERTEX_SHADER, m_shaderProg);
	AddShader(fs, GL_FRAGMENT_SHADER, m_shaderProg);
	bool isFinalized = Finalize(m_shaderProg);
	assert(isFinalized == true);

	return true;
}

void FadeEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void FadeEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void FadeEffect::SetFade(float intensity)
{
	glUniform1f(GetUniformLocation("gFade"), intensity);
}