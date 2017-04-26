#ifndef BASIC_EFFECT_H__
#define BASIC_EFFECT_H__

#include "effect.h"

class BasicEffect : public Effect
{
public:
	static BasicEffect& GetInstance()
	{
		static BasicEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	BasicEffect();
};

#endif // !BASIC_EFFECT_H__