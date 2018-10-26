#ifndef BLOOM_H_
#define BLOOM_H_

#include "FBO.h"
#include "basicEffect.h"
#include "bloomEffect.h"
#include "combineEffect.h"
#include "blurEffect.h"
#include "post_processing_screen.h"
#include "postProcessing.h"

#include <memory>

class Bloom : public PostProcessing
{
public:
	Bloom();
	~Bloom();

	void Apply(Post_Processing_Screen* pps, FBO* fbo);
	void ResetTextureSizes();

private:
	void ApplyBlur(Post_Processing_Screen* pps, int numTimes);
	int get_texture_size(int iteration);

private:
	std::vector<FBO> m_gaussesH;
	std::vector<FBO> m_gaussesV;
	FBO m_bloom;
	FBO m_gauss;
	//FBO m_gaussV;	
};

#endif