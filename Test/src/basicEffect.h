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

class GammaEffect : public Effect
{
public:
	static GammaEffect& GetInstance()
	{
		static GammaEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	GammaEffect();
};

#endif // !BASIC_EFFECT_H__