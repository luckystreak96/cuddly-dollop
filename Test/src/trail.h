#ifndef TRAIL_H_
#define TRAIL_H_

#include "FBO.h"
#include "basicEffect.h"
#include "transparencyEffect.h"
#include "combineEffect.h"
#include "blurEffect.h"
#include "post_processing_screen.h"
#include <vector>

class Trail {
public:
	Trail();

	void Begin();
	void End();
	void ResetTextureSizes();

private:
	//Decides how much to blur
	const float m_divisor = 2.0f;
	const int m_numFrames = 4;

	int m_frameCounter;
	int m_width;
	int m_height;
	std::vector<FBO> m_frames;
	FBO m_fbo;
	FBO m_trans;
	FBO m_gaussV;
	FBO m_gaussH;
	Post_Processing_Screen pps;
};

#endif