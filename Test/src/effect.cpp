#include "effect.h"

GLuint Effect::m_currentShaderProgram = 0;

Effect::Effect()
{
	Init();
}

Effect::~Effect()
{
	glDeleteProgram(m_shaderProg);
}

bool Effect::Init()
{
	m_shaderProg = glCreateProgram();
	if (m_shaderProg == 0)
	{
		std::cout << "Error creating shader program" << std::endl;
		system("PAUSE");
		exit(1);
		return false;
	}

	return true;
}

void Effect::SetModelPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gModel"), 1, GL_TRUE, (const GLfloat*)mat);
}

void Effect::SetWorldPosition(float* mat)
{
	glUniformMatrix4fv(GetUniformLocation("gWorld"), 1, GL_TRUE, (const GLfloat*)mat);
}

void Effect::Enable()
{
	if (m_currentShaderProgram != m_shaderProg)
		glUseProgram(m_shaderProg);

	m_currentShaderProgram = m_shaderProg;
}

bool Effect::AddShader(const char* shaderFileName, GLenum shaderType)
{
	std::string shaderString;
	shaderString = Utils::ReadFile(shaderFileName);
	const char* shaderText = shaderString.c_str();

	GLuint ShaderObj = glCreateShader(shaderType);

	if (ShaderObj == 0)
	{
		std::cout << "Error creating shader type " << shaderType << std::endl;
		system("PAUSE");
		exit(0);
	}

	//To compile a shader, we gotta specify stuff aboout it's source code
	const GLchar* p[1];
	p[0] = shaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(shaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	//Check for success / return error
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		std::cout << "Error compiling shader type " << shaderType << ": " << InfoLog << std::endl;
		system("PAUSE");
		return false;
	}

	m_shaderObjList.push_back(ShaderObj);

	//Attach it to the program so we can link it
	glAttachShader(m_shaderProg, ShaderObj);

	return true;
}

bool Effect::Finalize()
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << "Error linking shader program: " << ErrorLog << std::endl;
		return false;
	}

	Enable();

	GLuint lol = GetUniformLocation("gSampler");
	GLuint lol2 = GetUniformLocation("gSampler2");
	glUniform1i(lol, 0);
	glUniform1i(lol2, 1);

	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << "Invalid shader program: " << ErrorLog << std::endl;
		return false;
	}

	for (std::list<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
		glDeleteShader(*it);

	return true;
}

GLuint Effect::GetUniformLocation(const GLchar* name)
{
	return glGetUniformLocation(m_currentShaderProgram, name);
}
