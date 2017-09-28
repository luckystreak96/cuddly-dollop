#include "postProcessingEffect.h"

PostProcessingEffect::PostProcessingEffect()
{
	Init();
}

bool PostProcessingEffect::Init()
{
	const char* vs = "shaders/shader.vs";
	const char* fs = "shaders/shader.fs";

	AddShader(vs, GL_VERTEX_SHADER, m_shaderProg);
	AddShader(fs, GL_FRAGMENT_SHADER, m_shaderProg);
	bool isFinalized = Finalize(m_shaderProg);
	assert(isFinalized == true);

	return true;
}

void PostProcessingEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void PostProcessingEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void PostProcessingEffect::SetSize(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gSize"), 1, GL_TRUE, (const GLfloat*)mat);
}