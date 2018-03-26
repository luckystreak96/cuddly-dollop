#include "FBO.h"
#include "define_gl.h"
#include "glfwBackend.h"
#include <iostream>

Vector2f FBO::_fboGLVersion = Vector2f(2, 0);

FBO::FBO(int width, int height, int depthBufferType) {
	m_width = width;
	m_height = height;
	m_depthBufferType = depthBufferType;
	colourBuffer = 0;
	m_numColorBuffers = 0;
	initialiseFrameBuffer(depthBufferType);
	GLenum status;

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		std::cout << "Frambuffer not completed!" << std::endl;
	}

	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//{
	//	std::cout << "Frambuffer not loaded properly!" << std::endl;
	//}//std::cout << "\n Error:: FrameBufferObject::Initialize() :: FBO loading not complete \n";

}


FBO::~FBO()
{
	cleanUp();
}


/**
* Deletes the frame buffer and its attachments when the game closes.
*/
void FBO::cleanUp() {
	glDeleteFramebuffersEXT(1, &frameBuffer);
	glDeleteTextures(1, &colourTexture);
	glDeleteTextures(1, &depthTexture);
	glDeleteRenderbuffers(1, &depthBuffer);
	glDeleteRenderbuffers(1, &colourBuffer);
}

/**
* Binds the frame buffer, setting it as the current render target. Anything
* rendered after this will be rendered to this FBO, and not to the screen.
*/
void FBO::bindFrameBuffer() {
	glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, frameBuffer);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//glBindTexture(GL_TEXTURE_2D, colourTexture);
	//int w, h;
	//glfwGetWindowSize(GLFWManager::m_window, &w, &h);
	//float r = w / h;
	//glViewport(0, 0, m_width, m_height);
	//glViewport(0, 0, (GLsizei)(WINDOW_WIDTH * RATIO), (GLsizei)(WINDOW_HEIGHT * RATIO));
}

/**
* Unbinds the frame buffer, setting the default frame buffer as the current
* render target. Anything rendered after this will be rendered to the
* screen, and not this FBO.
*/
void FBO::unbindFrameBuffer() {
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//int w, h;
	//glfwGetWindowSize(GLFWManager::m_window, &w, &h);
	//glViewport(0, 0, w, h);
	//glViewport(0, 0, (GLsizei)(WINDOW_WIDTH * RATIO), (GLsizei)(WINDOW_HEIGHT * RATIO));
}

/**
* Binds the current FBO to be read from (not used in tutorial 43).
*/
void FBO::bindToRead() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_READ_FRAMEBUFFER, frameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

/**
* @return The ID of the texture containing the colour buffer of the FBO.
*/
GLuint FBO::getColourTexture() {
	return colourTexture;
}

/**
* @return The texture containing the FBOs depth buffer.
*/
GLuint FBO::getDepthTexture() {
	return depthTexture;
}

/**
* Creates the FBO along with a colour buffer texture attachment, and
* possibly a depth buffer.
*
* @param type
*            - the type of depth buffer attachment to be attached to the
*            FBO.
*/
void FBO::initialiseFrameBuffer(int type) {
	createFrameBuffer();
	createTextureAttachment();
	if (type == DEPTH_RENDER_BUFFER) {
		createDepthBufferAttachment();
	}
	else if (type == DEPTH_TEXTURE) {
		createDepthTextureAttachment();
	}
	unbindFrameBuffer();
}

/**
* Creates a new frame buffer object and sets the buffer to which drawing
* will occur - colour attachment 0. This is the attachment where the colour
* buffer texture is.
*
*/
void FBO::createFrameBuffer() {
	glGenFramebuffersEXT(1, &frameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//glDisable(GL_DEPTH_TEST);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/**
* Creates a texture and sets it as the colour buffer attachment for this
* FBO.
*/
void FBO::createTextureAttachment() {
	glGenTextures(1, &colourTexture);
	glBindTexture(GL_TEXTURE_2D, colourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture,
		0);
}

/**
* Adds a depth buffer to the FBO in the form of a texture, which can later
* be sampled.
*/
void FBO::createDepthTextureAttachment() {
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT,
		GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

/**
* Adds a depth buffer to the FBO in the form of a render buffer. This can't
* be used for sampling in the shaders.
*/
void FBO::createDepthBufferAttachment() {
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

void FBO::resetTextures(int width, int height)
{
	m_width = width;
	m_height = height;

	glBindFramebufferEXT(GL_FRAMEBUFFER, frameBuffer);

	glDeleteTextures(1, &colourTexture);
	glDeleteRenderbuffers(1, &depthBuffer);

	glGenTextures(1, &colourTexture);
	glBindTexture(GL_TEXTURE_2D, colourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture, 0);

	if (m_depthBufferType == FBO::DEPTH_TEXTURE)
	{
		glDeleteTextures(1, &depthTexture);

		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT,
			GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	}

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	unbindFrameBuffer();
}
