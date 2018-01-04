#include "bloomEffect.h"

BloomEffect::BloomEffect()
{
	Effect::Init(m_darkBloomProgram);
	Init();
}

bool BloomEffect::Init()
{
	std::string vs = "shaders/" + GetGLVersionString() + "/shader.vs";
	std::string fs = "shaders/" + GetGLVersionString() + "/bloom.fs";

	AddShader(vs, GL_VERTEX_SHADER, m_shaderProg);
	AddShader(fs, GL_FRAGMENT_SHADER, m_shaderProg);
	if (_efctGLVersion.x == 1.0f)
	{
		glBindAttribLocation(m_shaderProg, 0, "Position");
		glBindAttribLocation(m_shaderProg, 1, "TexCoord");
		glBindAttribLocation(m_shaderProg, 5, "Model");
	}
	bool isFinalized = Finalize(m_shaderProg);
	assert(isFinalized == true);

	std::string vs2 = "shaders/" + GetGLVersionString() + "/shader.vs";
	std::string fs2 = "shaders/" + GetGLVersionString() + "/bloomDark.fs";

	AddShader(vs2, GL_VERTEX_SHADER, m_darkBloomProgram);
	AddShader(fs2, GL_FRAGMENT_SHADER, m_darkBloomProgram);
	if (_efctGLVersion.x == 1.0f)
	{
		glBindAttribLocation(m_shaderProg, 0, "Position");
		glBindAttribLocation(m_shaderProg, 1, "TexCoord");
		glBindAttribLocation(m_shaderProg, 5, "Model");
	}
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