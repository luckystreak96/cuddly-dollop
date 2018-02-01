#ifndef EFFECT_H__
#define EFFECT_H__

#include <iostream>
#include "GL/glew.h"
#include "utils.h"
#include "vector3f.h"
#include <string>
#include <fstream>
#include <cstring>
#include <cassert>
#include <list>
#include <iterator>

enum DrawType { DT_FLAT, DT_ENTITY };

class Effect
{
public:
	Effect();
	~Effect();
	virtual bool Init(GLuint& program);//create shader program
	void Enable(GLuint program = 0);//enable shader -- glUseShaderProgram(m_shaderProgram);
	static void SetModelPosition(float* mat);
	static void SetWorldPosition(float* mat);
	static void SetTileSize(float size);
	static void SetDrawType(DrawType dt);
	static float* GetPrevWorldPos();
	static Vector2f _efctGLVersion;
	std::string GetGLVersionString();
protected:
	bool AddShader(std::string shaderFileName, GLenum shaderType, GLuint& program);//add a shader thru its contents
	bool Finalize(GLuint& program);//Link objects
	static GLuint GetUniformLocation(const GLchar* name);
	GLuint m_shaderProg;
	static GLuint m_currentShaderProgram;
	static GLuint m_prevShaderProgram;
	static float* m_prevWorld;
private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shaderObjList;
};

#endif