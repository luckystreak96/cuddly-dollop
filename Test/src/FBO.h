#ifndef FBO_H__
#define FBO_H__

#include <GL/glew.h>
#include "vector3f.h"

class FBO
{
public:
	static Vector2f _fboGLVersion;

private:
	int m_width;
	int m_height;

	GLuint frameBuffer;
	GLuint colourTexture;
	GLuint depthrenderbuffer;

public:
	FBO(int width = 1, int height = 1);
	~FBO();

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	void ResetTextures(int width, int height);


	/**
	* Deletes the frame buffer and its attachments when the game closes.
	*/
	void CleanUp();


	/**
	* Binds the frame buffer, setting it as the current render target. Anything
	* rendered after this will be rendered to this FBO, and not to the screen.
	*/
	void BindFrameBuffer();

	//void bindTexture();

	/**
	* Unbinds the frame buffer, setting the default frame buffer as the current
	* render target. Anything rendered after this will be rendered to the
	* screen, and not this FBO.
	*/
	void UnbindFrameBuffer();


	/**
	* @return The ID of the texture containing the colour buffer of the FBO.
	*/
	GLuint GetColourTexture();


	/**
	* Creates the FBO along with a colour buffer texture attachment, and
	* possibly a depth buffer.
	*
	* @param type
	*            - the type of depth buffer attachment to be attached to the
	*            FBO.
	*/
	void InitialiseFrameBuffer();

};

#endif
