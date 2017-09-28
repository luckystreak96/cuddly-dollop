#ifndef BLOOM_EFFECT_H__
#define BLOOM_EFFECT_H__

#include "effect.h"

class BloomEffect : public Effect
{
public:
	static BloomEffect& GetInstance()
	{
		static BloomEffect instance;
		return instance;
	}
	bool Init();
	void SetWorldPosition(float* mat);
	void SetModelPosition(float* mat);
	void SetSize(float* mat);
	GLuint GetNormal() { return m_shaderProg; }
	GLuint GetDark() { return m_darkBloomProgram; }
private:
	BloomEffect();
	//GLuint m_normalBloomProgram;
	GLuint m_darkBloomProgram;
};

#endif // !BASIC_EFFECT_H__