#ifndef CONTRAST_EFFECT_H__
#define CONTRAST_EFFECT_H__

#include "effect.h"

class ContrastEffect : public Effect
{
public:
	static ContrastEffect& GetInstance()
	{
		static ContrastEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	ContrastEffect();
};

#endif // !BASIC_EFFECT_H__