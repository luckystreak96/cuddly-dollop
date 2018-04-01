#ifndef NIGHTTIME_EFFECT_H__
#define NIGHTTIME_EFFECT_H__

#include "effect.h"

class NightTimeEffect : public Effect
{
public:
	static NightTimeEffect& GetInstance()
	{
		static NightTimeEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	NightTimeEffect();
};

#endif // !BASIC_EFFECT_H__
