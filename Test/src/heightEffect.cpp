#include "heightEffect.h"

HeightEffect::HeightEffect()
{
	Init();
}

bool HeightEffect::Init()
{
	const char* vs = "shaders/heightFog.vs";
	const char* fs = "shaders/heightFog.fs";

	AddShader(vs, GL_VERTEX_SHADER);
	AddShader(fs, GL_FRAGMENT_SHADER);
	bool isFinalized = Finalize();
	assert(isFinalized == true);

	return true;
}

void HeightEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void HeightEffect::SetPlayerPos(Vector3f pos)
{
	glUniform3f(GetUniformLocation("playerPos"), (GLfloat)pos.x, pos.y, pos.z);
}

void HeightEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void HeightEffect::SetSize(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gSize"), 1, GL_TRUE, (const GLfloat*)mat);
}