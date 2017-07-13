#include "FBO.h"

FBO::FBO(int width, int height, int depthBufferType) {
	m_width = width;
	m_height = height;
	initialiseFrameBuffer(depthBufferType);
	GLenum status;

	status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		int i = 0;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		int i = 3422;
	}//std::cout << "\n Error:: FrameBufferObject::Initialize() :: FBO loading not complete \n";

}


FBO::~FBO()
{
	cleanUp();
}


/**
* Deletes the frame buffer and its attachments when the game closes.
*/
void FBO::cleanUp() {
	glDeleteFramebuffers(1, &frameBuffer);
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
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//glBindTexture(GL_TEXTURE_2D, colourTexture);
	//float w = glutGet(GLUT_WINDOW_WIDTH);
	//float h = glutGet(GLUT_WINDOW_HEIGHT);
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	//glViewport(0, 0, (GLsizei)(WINDOW_WIDTH * RATIO), (GLsizei)(WINDOW_HEIGHT * RATIO));
}

/**
* Binds the current FBO to be read from (not used in tutorial 43).
*/
void FBO::bindToRead() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0);
}

/**
* @return The ID of the texture containing the colour buffer of the FBO.
*/
int FBO::getColourTexture() {
	return colourTexture;
}

/**
* @return The texture containing the FBOs depth buffer.
*/
int FBO::getDepthTexture() {
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
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

/**
* Creates a texture and sets it as the colour buffer attachment for this
* FBO.
*/
void FBO::createTextureAttachment() {
	glGenTextures(1, &colourTexture);
	glBindTexture(GL_TEXTURE_2D, colourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture,
		0);
}

/**
* Adds a depth buffer to the FBO in the form of a texture, which can later
* be sampled.
*/
void FBO::createDepthTextureAttachment() {
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT,
		GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}

/**
* Adds a depth buffer to the FBO in the form of a render buffer. This can't
* be used for sampling in the shaders.
*/
void FBO::createDepthBufferAttachment() {
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

void FBO::resetTextures(int width, int height) 
{
	m_width = width;
	m_height = height;

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glDeleteTextures(1, &colourTexture);
	glDeleteTextures(1, &depthTexture);
	glDeleteRenderbuffers(1, &depthBuffer);

	glGenTextures(1, &colourTexture);
	glBindTexture(GL_TEXTURE_2D, colourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture, 0);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT,
		GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	unbindFrameBuffer();
}
