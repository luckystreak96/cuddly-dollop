#include "shadowMapFBO.h"

ShadowMapFBO::ShadowMapFBO()
{
	m_fbo = 0;
	m_shadowMap = 0;
	m_depth = 0;
}

ShadowMapFBO::~ShadowMapFBO()
{
	if (m_fbo != 0) {
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_shadowMap != 0) {
		glDeleteTextures(1, &m_shadowMap);
	}

	if (m_depth != 0) {
		glDeleteTextures(1, &m_depth);
	}
}

bool ShadowMapFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	//If a new one needs to be initialized
	if (m_fbo != 0) {
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_shadowMap != 0) {
		glDeleteTextures(1, &m_shadowMap);
	}

	if (m_depth != 0) {
		glDeleteTextures(1, &m_depth);
	}

	// Create the FBO
	glGenFramebuffers(1, &m_fbo);

	// Create the depth buffer
	glGenTextures(1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create the cube map
	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, WindowWidth, WindowHeight, 0, GL_RED, GL_FLOAT, NULL);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return glGetError() == GL_NO_ERROR;
}


void ShadowMapFBO::BindForWriting(GLenum CubeFace)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CubeFace, m_shadowMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}


void ShadowMapFBO::BindForReading(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
}
