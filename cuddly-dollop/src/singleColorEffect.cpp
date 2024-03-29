#include "singleColorEffect.h"

SingleColorEffect::SingleColorEffect()
{
	Effect::Init(m_shaderProg);
	Init();
}

bool SingleColorEffect::Init()
{
	std::string vs = "shaders/" + GetGLVersionString() + "/shader.vs";
	std::string fs = "shaders/" + GetGLVersionString() + "/singleColor.fs";

	AddShader(vs, GL_VERTEX_SHADER, m_shaderProg);
	AddShader(fs, GL_FRAGMENT_SHADER, m_shaderProg);
	if (_efctGLVersion.x == 1.0f)
	{
		glBindAttribLocation(m_shaderProg, 0, "get_position");
		glBindAttribLocation(m_shaderProg, 1, "TexCoord");
		glBindAttribLocation(m_shaderProg, 5, "Model");
	}
	bool isFinalized = Finalize(m_shaderProg);
	assert(isFinalized == true);

	return true;
}

void SingleColorEffect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void SingleColorEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void SingleColorEffect::SetSize(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gSize"), 1, GL_TRUE, (const GLfloat*)mat);
}