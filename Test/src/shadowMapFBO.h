#ifndef SHADOW_MAP_FBO_H__
#define SHADOW_MAP_FBO_H__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "effect.h"

class ShadowMapFBO
{
public:
	static ShadowMapFBO& GetInstance()
	{
		static ShadowMapFBO instance;
		return instance;
	}
	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
	void BindForWriting(GLenum Cubeface);
	void BindForReading(GLenum TextureUnit);

private:
	ShadowMapFBO();
	~ShadowMapFBO();

private:
	GLuint m_fbo;
	GLuint m_shadowMap;
	GLuint m_depth;
};

#endif
