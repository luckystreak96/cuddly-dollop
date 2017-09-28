#include "bloomEffect.h"

BloomEffect::BloomEffect()
{
	Effect::Init(m_darkBloomProgram);
	Init();
}

bool BloomEffect::Init()
{
	const char* vs = "shaders/shader.vs";
	const char* fs = "shaders/bloom.fs";

	AddShader(vs, GL_VERTEX_SHADER, m_shaderProg);
	AddShader(fs, GL_FRAGMENT_SHADER, m_shaderProg);
	bool isFinalized = Finalize(m_shaderProg);
	assert(isFinalized == true);

	const char* vs2 = "shaders/shader.vs";
	const char* fs2 = "shaders/bloomDark.fs";

	AddShader(vs2, GL_VERTEX_SHADER, m_darkBloomProgram);
	AddShader(fs2, GL_FRAGMENT_SHADER, m_darkBloomProgram);
	isFinalized = Finalize(m_darkBloomProgram);
	assert(isFinalized == true);

	return true;
}

void BloomEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void BloomEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void BloomEffect::SetSize(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gSize"), 1, GL_TRUE, (const GLfloat*)mat);
}