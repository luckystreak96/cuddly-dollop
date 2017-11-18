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

enum DrawType { DT_FLAT, DT_ENTITY };

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
	virtual bool Init(GLuint& program);//create shader program
	void Enable(GLuint program = 0);//enable shader -- glUseShaderProgram(m_shaderProgram);
	static void SetModelPosition(float* mat);
	static void SetWorldPosition(float* mat);
	static void SetDrawType(DrawType dt);
	static float* GetPrevWorldPos();
protected:
	bool AddShader(const char* shaderFileName, GLenum shaderType, GLuint& program);//add a shader thru its contents
	bool Finalize(GLuint& program);//Link objects
	static GLuint GetUniformLocation(const GLchar* name);
	GLuint m_shaderProg;
	static GLuint m_currentShaderProgram;
	static float* m_prevWorld;
private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};

#endif