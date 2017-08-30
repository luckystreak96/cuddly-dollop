#ifndef FBO_H__
#define FBO_H__

#include <gl/glew.h>
#include "define_gl.h"
#include "drawable.h"

class FBO
{
public:
	static const int NONE = 0;
	static const int DEPTH_TEXTURE = 1;
	static const int DEPTH_RENDER_BUFFER = 2;

private:
	int m_width;
	int m_height;

	GLuint frameBuffer;

	GLuint colourTexture;
	GLuint depthTexture;

	GLuint depthBuffer;
	GLuint colourBuffer;

	int m_numColorBuffers;

	/**
	* Creates an FBO of a specified width and height, with the desired type of
	* depth buffer attachment.
	*
	* @param width
	*            - the width of the FBO.
	* @param height
	*            - the height of the FBO.
	* @param depthBufferType
	*            - an int indicating the type of depth buffer attachment that
	*            this FBO should use.
	*/
public:
	FBO(int width = 1, int height = 1, int depthBufferType = FBO::DEPTH_TEXTURE);
	~FBO();

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	void resetTextures(int width, int height);


	/**
	* Deletes the frame buffer and its attachments when the game closes.
	*/
	void cleanUp();


	/**
	* Binds the frame buffer, setting it as the current render target. Anything
	* rendered after this will be rendered to this FBO, and not to the screen.
	*/
	void bindFrameBuffer();


	/**
	* Unbinds the frame buffer, setting the default frame buffer as the current
	* render target. Anything rendered after this will be rendered to the
	* screen, and not this FBO.
	*/
	void unbindFrameBuffer();


	/**
	* Binds the current FBO to be read from (not used in tutorial 43).
	*/
	void bindToRead();

	/**
	* @return The ID of the texture containing the colour buffer of the FBO.
	*/
	int getColourTexture();

	/**
	* @return The texture containing the FBOs depth buffer.
	*/
	int getDepthTexture();

	/**
	* Creates the FBO along with a colour buffer texture attachment, and
	* possibly a depth buffer.
	*
	* @param type
	*            - the type of depth buffer attachment to be attached to the
	*            FBO.
	*/
	void initialiseFrameBuffer(int type);

	/**
	* Creates a new frame buffer object and sets the buffer to which drawing
	* will occur - colour attachment 0. This is the attachment where the colour
	* buffer texture is.
	*
	*/
	void createFrameBuffer();

	/**
	* Creates a texture and sets it as the colour buffer attachment for this
	* FBO.
	*/
	void createTextureAttachment();

	/**
	* Adds a depth buffer to the FBO in the form of a texture, which can later
	* be sampled.
	*/
private:
	void createDepthTextureAttachment();

	/**
	* Adds a depth buffer to the FBO in the form of a render buffer. This can't
	* be used for sampling in the shaders.
	*/
	void createDepthBufferAttachment();

};

#endif