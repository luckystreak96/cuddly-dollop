#ifndef NIGHTTIME_PROCESSING_H_
#define NIGHTTIME_PROCESSING_H_

#include "FBO.h"
#include "basicEffect.h"
#include "post_processing_screen.h"
#include "postProcessing.h"

#include <memory>

class NightTimeProcessing : public PostProcessing
{
public:
	NightTimeProcessing();

	void Apply(Post_Processing_Screen* pps, FBO* fbo);
	void ResetTextureSizes();

private:
	FBO m_contrast;
};

#endif
