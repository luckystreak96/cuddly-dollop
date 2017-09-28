#ifndef BLOOM_H_
#define BLOOM_H_

#include "FBO.h"
#include "basicEffect.h"
#include "bloomEffect.h"
#include "combineEffect.h"
#include "blurEffect.h"
#include "post_processing_screen.h"

class Bloom {
public:
	Bloom();

	void Begin();
	void End(bool dark);
	void ResetTextureSizes();

private:
	//Decides how much to blur
	const float m_divisor = 2.0f;

	int m_width;
	int m_height;
	FBO m_fbo;
	FBO m_bloom;
	FBO m_gaussV;
	FBO m_gaussH;
	Post_Processing_Screen pps;
};

#endif