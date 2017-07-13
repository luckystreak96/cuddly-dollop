#ifndef COMBINE_EFFECT_H__
#define COMBINE_EFFECT_H__

#include "effect.h"

class CombineEffect : public Effect
{
public:
	static CombineEffect& GetInstance()
	{
		static CombineEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
	void SetIntensity(float value);
private:
	CombineEffect();
};

#endif // !BASIC_EFFECT_H__