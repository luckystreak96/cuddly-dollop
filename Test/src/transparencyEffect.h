#ifndef TRANSPARENCY_EFFECT_H__
#define TRANSPARENCY_EFFECT_H__

#include "effect.h"

class TransparencyEffect : public Effect
{
public:
	static TransparencyEffect& GetInstance()
	{
		static TransparencyEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	TransparencyEffect();
};

#endif // !BASIC_EFFECT_H__