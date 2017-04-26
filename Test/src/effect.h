#ifndef EFFECT_H__
#define EFFECT_H__

#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "utils.h"
#include "vector3f.h"
#include <string>
#include <fstream>
#include <cstring>
#include <cassert>
#include <list>
#include <iterator>

#define SHADOW_TEXTURE_UNIT GL_TEXTURE1

struct BaseLight
{
	Vector3f Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = Vector3f(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.5f;
		DiffuseIntensity = 0.0f;
	}
};

struct PointLight : public BaseLight
{
	Vector3f Position;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		Position = Vector3f(0.0f, 0.0f, 0.0f);
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

struct DirectionalLight : BaseLight
{
	Vector3f Direction;

	DirectionalLight()
	{
		Direction = Vector3f(0.0f, 0.0f, 0.0f);
	}
};

class Effect
{
public:
	Effect();
	~Effect();
	virtual bool Init();//create shader program
	void Enable();//enable shader -- glUseShaderProgram(m_shaderProgram);
protected:
	bool AddShader(const char* shaderFileName, GLenum shaderType);//add a shader thru its contents
	bool Finalize();//Link objects
	GLuint GetUniformLocation(const GLchar* name);
private:
	static GLuint m_currentShaderProgram;
	GLuint m_shaderProg;
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};

#endif