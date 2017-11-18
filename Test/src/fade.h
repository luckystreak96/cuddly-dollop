#ifndef FADE_H__
#define FADE_H__

#include "FBO.h"
#include "basicEffect.h"
#include "fadeEffect.h"
#include "combineEffect.h"
#include "post_processing_screen.h"

class Fade
{
public:
	Fade();
	void Begin();
	void SetFade(bool fadeIn);
	bool IsDone();
private:
	float m_fadeProgress;
	bool m_fadeIn;
};

#endif