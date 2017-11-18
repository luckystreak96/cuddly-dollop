#include "effect.h"

float* Effect::m_prevWorld = NULL;
GLuint Effect::m_currentShaderProgram = 0;

Effect::Effect()
{
	Init(m_shaderProg);
}

Effect::~Effect()
{
	glDeleteProgram(m_shaderProg);
}

bool Effect::Init(GLuint& program)
{
	program = glCreateProgram();
	if (program == 0)
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
	m_prevWorld = mat;
}

void Effect::SetDrawType(DrawType dt)
{
	glUniform1i(GetUniformLocation("gType"), (const GLint)dt);
}

void Effect::Enable(GLuint program)
{
	//Default value
	if (program == 0)
		program = m_shaderProg;

	if (m_currentShaderProgram != program)
		glUseProgram(program);

	m_currentShaderProgram = program;
}

bool Effect::AddShader(const char* shaderFileName, GLenum shaderType, GLuint& program)
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
	glAttachShader(program, ShaderObj);

	return true;
}

bool Effect::Finalize(GLuint& program)
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << "Error linking shader program: " << ErrorLog << std::endl;
		return false;
	}

	Enable(program);

	GLuint lol = GetUniformLocation("gSampler");
	GLuint lol2 = GetUniformLocation("gSampler2");
	glUniform1i(lol, 0);
	glUniform1i(lol2, 1);

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(program, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << "Invalid shader program: " << ErrorLog << std::endl;
		return false;
	}

	for (std::list<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
		glDeleteShader(*it);

	m_shaderObjList.clear();

	return true;
}

GLuint Effect::GetUniformLocation(const GLchar* name)
{
	return glGetUniformLocation(m_currentShaderProgram, name);
}

float* Effect::GetPrevWorldPos()
{
	return m_prevWorld;
}
