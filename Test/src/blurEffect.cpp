#include "blurEffect.h"

BlurEffect::BlurEffect()
{
	Init();
}

bool BlurEffect::Init()
{
	const char* vs = "shaders/shader.vs";
	//const char* fs = "shaders/blur.fs";
	const char* fs = "shaders/gaussian11.fs";

	AddShader(vs, GL_VERTEX_SHADER);
	AddShader(fs, GL_FRAGMENT_SHADER);
	bool isFinalized = Finalize();
	assert(isFinalized == true);

	return true;
}

void BlurEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void BlurEffect::SetHorizontalBlur(GLboolean hor)
{
	glUniform1i(GetUniformLocation("horizontal"), hor);
}

void BlurEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void BlurEffect::SetSize(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gSize"), 1, GL_TRUE, (const GLfloat*)mat);
}