#ifndef BLUR_EFFECT_H__
#define BLUR_EFFECT_H__

#include "effect.h"

class BlurEffect : public Effect
{
public:
	static BlurEffect& GetInstance()
	{
		static BlurEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetHorizontalBlur(GLboolean hor);
	void SetTextureSize(int w, int h);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	BlurEffect();
};

#endif // !BASIC_EFFECT_H__