#ifndef BLUR_FRAMEBUFFER_H__
#define BLUF_FRAMEBUFFER_H_

#include <GL/glew.h>

class BlurFramebuffer
{
public:
	static BlurFramebuffer& GetInstance()
	{
		static BlurFramebuffer instance;
		return instance;
	}
	GLuint* GetPingPongFBOs() { return pingpongFBO; };
	GLuint* GetPingPongBuffers() { return pingpongBuffer; };
	GLuint GetColorBuffer1() { return colorBuffers[1]; };
private:

	GLuint pingpongFBO[2];
	GLuint pingpongBuffer[2];
	GLuint hdrFBO;
	GLuint colorBuffers[2];
	BlurFramebuffer();
};

#endif
