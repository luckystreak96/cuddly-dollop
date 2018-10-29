#include "blurEffect.h"
#include "gameData.h"

BlurEffect::BlurEffect()
{
	Init();
}

bool BlurEffect::Init()
{
	std::string vs = "shaders/" + GetGLVersionString() + "/shader.vs";
	std::string fs = "shaders/" + GetGLVersionString() + "/gaussian.fs";

	/*
	std::string level = "11";
	int eq = std::get<int>(GameData::Options.at("effect_quality"));
	switch (eq)
	{
	case 0:
		level = "9";
		break;
	case 1:
		level = "9";
		break;
	case 2:
		level = "11";
		break;
	}
	std::string fs = "shaders/" + GetGLVersionString() + "/gaussian" + level + ".fs";
	*/

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

void BlurEffect::SetHorizontalBlur(bool hor)
{
	glUniform1i(GetUniformLocation("horizontal"), hor == false ? 0 : 1);
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