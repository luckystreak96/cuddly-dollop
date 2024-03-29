#ifndef FADE_H__
#define FADE_H__

#include "FBO.h"
#include "basicEffect.h"
#include "fadeEffect.h"
#include "combineEffect.h"

class Fade
{
public:
	Fade();
	void Begin();
	void SetFade(bool fadeIn);
	void ForceFadeValue(float value);
	bool IsDone();
private:
	float m_fadeProgress;
	bool m_fadeIn;
};

#endif