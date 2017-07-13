#include "bloomEffect.h"

BloomEffect::BloomEffect()
{
	Init();
}

bool BloomEffect::Init()
{
	const char* vs = "shaders/shader.vs";
	const char* fs = "shaders/bloom.fs";

	AddShader(vs, GL_VERTEX_SHADER);
	AddShader(fs, GL_FRAGMENT_SHADER);
	bool isFinalized = Finalize();
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