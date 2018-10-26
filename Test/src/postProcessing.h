#ifndef POST_PROCESSING_EFFECT_H__
#define POST_PROCESSING_EFFECT_H__

#include "post_processing_screen.h"
#include "FBO.h"

class PostProcessing
{
public:
	PostProcessing();
	virtual void Apply(Post_Processing_Screen* screen, FBO* fbo) = 0;
	virtual void ResetTextureSizes() = 0;
public:
	//Decides how much to blur
	int m_divisor;

	int _width;
	int _height;
};

#endif // !BASIC_EFFECT_H__