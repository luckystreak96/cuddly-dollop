#include "transparencyEffect.h"

TransparencyEffect::TransparencyEffect()
{
	Init();
}

bool TransparencyEffect::Init()
{
	const char* vs = "shaders/shader.vs";
	const char* fs = "shaders/transparency.fs";

	AddShader(vs, GL_VERTEX_SHADER);
	AddShader(fs, GL_FRAGMENT_SHADER);
	bool isFinalized = Finalize();
	assert(isFinalized == true);

	return true;
}

void TransparencyEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void TransparencyEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void TransparencyEffect::SetSize(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gSize"), 1, GL_TRUE, (const GLfloat*)mat);
}