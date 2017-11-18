#ifndef FADE_EFFECT_H__
#define FADE_EFFECT_H__

#include "effect.h"

class FadeEffect : public Effect
{
public:
	static FadeEffect& GetInstance()
	{
		static FadeEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetFade(float intensity);
private:
	FadeEffect();
};

#endif // !BASIC_EFFECT_H__