#ifndef POST_PROCESSING_EFFECT_H__
#define POST_PROCESSING_EFFECT_H__

#include "effect.h"

class PostProcessingEffect : public Effect
{
public:
	static PostProcessingEffect& GetInstance()
	{
		static PostProcessingEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	PostProcessingEffect();
};

#endif // !BASIC_EFFECT_H__