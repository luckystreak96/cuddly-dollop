#ifndef HEIGHT_EFFECT
#define HEIGHT_EFFECT

#include "effect.h"

class HeightEffect : public Effect
{
public:
	static HeightEffect& GetInstance()
	{
		static HeightEffect instance;
		return instance;
	}
	bool Init();
	void SetPlayerPos(Vector3f playerz);
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
private:
	HeightEffect();
};

#endif // !HEIGHT_EFFECT
