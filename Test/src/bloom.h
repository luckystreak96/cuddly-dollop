#ifndef BLOOM_H_
#define BLOOM_H_

#include "FBO.h"
#include "basicEffect.h"
#include "bloomEffect.h"
#include "combineEffect.h"
#include "blurEffect.h"
#include "post_processing_screen.h"
#include "postProcessingEffect.h"

#include <memory>

class Bloom : public PostProcessingEffect
{
public:
	Bloom();

	void Apply(Post_Processing_Screen* pps, FBO* fbo);
	void ResetTextureSizes();

private:
	FBO m_bloom;
	FBO m_gaussH;
	FBO m_gaussV;
};

#endif