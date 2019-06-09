#ifndef SINGLE_COLOR_EFFECT_H__
#define SINGLE_COLOR_EFFECT_H__

#include "effect.h"

class SingleColorEffect : public Effect
{
public:
	static SingleColorEffect& GetInstance()
	{
		static SingleColorEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	SingleColorEffect();
};

#endif // !BASIC_EFFECT_H__