#include "FBO.h"
#include "glfwBackend.h"
#include <iostream>

Vector2f FBO::_fboGLVersion = Vector2f(2, 0);

FBO::FBO(int width, int height) {
	m_width = width;
	m_height = height;
	depthrenderbuffer = 0;
	colourTexture = 0;
	frameBuffer = 0;
	InitialiseFrameBuffer();
	GLenum status;
	BindFrameBuffer();
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Frambuffer not completed!" << std::endl;
	}

	UnbindFrameBuffer();
}


FBO::~FBO()
{
	CleanUp();
}


/**
* Deletes the frame buffer and its attachments when the game closes.
*/
void FBO::CleanUp() {
	// If ever there's some kind of memory leak problem, it might be because of this.
	// However, I removed these 2 lines to solve a problem related to delete textures deleting the texture of the slime.
	// To reproduce: dont active ppe until you enter battle, then trigger it and the slime texture becomes the last deleted framebuffer texture.
	glDeleteTextures(1, &colourTexture);
	glDeleteRenderbuffers(1, &depthrenderbuffer);
	glDeleteFramebuffers(1, &frameBuffer);
	colourTexture = 0;
	depthrenderbuffer = 0;
	frameBuffer = 0;
}

/**
* Binds the frame buffer, setting it as the current render target. Anything
* rendered after this will be rendered to this FBO, and not to the screen.
*/
void FBO::BindFrameBuffer() {
	if (!glIsFramebuffer(frameBuffer))
		std::cout << "Framebuffer error: framebuffer not valid" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	//glBindTexture(GL_TEXTURE_2D, colourTexture);
}

/**
* Unbinds the frame buffer, setting the default frame buffer as the current
* render target. Anything rendered after this will be rendered to the
* screen, and not this FBO.
*/
void FBO::UnbindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
* @return The ID of the texture containing the colour buffer of the FBO.
*/
GLuint FBO::GetColourTexture() {
	return colourTexture;
}

/**
* Creates the FBO along with a colour buffer texture attachment, and
* possibly a depth buffer.
*
* @param type
*            - the type of depth buffer attachment to be attached to the
*            FBO.
*/
void FBO::InitialiseFrameBuffer() {
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// The texture we're going to render to -- only create a new one if there isnt already one
	glGenTextures(1, &colourTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, colourTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colourTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	UnbindFrameBuffer();
}

void FBO::ResetTextures(int width, int height)
{
	m_width = width;
	m_height = height;

	CleanUp();
	//BindFrameBuffer();
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//if(depthrenderbuffer)
	//	glDeleteRenderbuffers(1, &depthrenderbuffer);
	//depthrenderbuffer = 0;

	InitialiseFrameBuffer();
}
