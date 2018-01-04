#include "blurEffect.h"

BlurEffect::BlurEffect()
{
	Init();
}

bool BlurEffect::Init()
{
	std::string vs = "shaders/" + GetGLVersionString() + "/shader.vs";
	//std::string fs = "shaders/" + GetGLVersionString() + "/blur.fs";
	std::string fs = "shaders/" + GetGLVersionString() + "/gaussian11.fs";

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

void BlurEffect::SetTextureSize(int w, int h)
{
	glUniform2i(GetUniformLocation("texSize"), w, h);
}

void BlurEffect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void BlurEffect::SetSize(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gSize"), 1, GL_TRUE, (const GLfloat*)mat);
}