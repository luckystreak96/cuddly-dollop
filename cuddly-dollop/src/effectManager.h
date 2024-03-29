#ifndef EFFECT_MANAGER_H__
#define EFFECT_MANAGER_H__

#include "effect.h"
#include "transform.h"
#include <map>

enum Effects { E_Basic, E_Gamma, E_SingleColor, E_Combine, E_Bloom, E_Blur, E_Fade, E_Contrast, E_NightTime, E_Last };

class EffectManager
{
public:
	static EffectManager& GetInstance()
	{
		static EffectManager manager;
		return manager;
	}

	void Enable(Effects ef = E_Basic, GLuint program = 0);
	void EnablePrevious();
	void SetWorldTrans(float* withTranslate, float* noTranslate);
	void UpdateWorld();
	void ResetWorldUpdateFlag();
	void SetAllTilePositions(float size);
	void SetLightIntensity(float intensity);
	void SetNoTranslateMode(bool translateMode);

private:
	EffectManager();
	std::pair<Effect*, Effects> m_prevEffect;
	std::pair<Effect*, Effects> m_currentEffect;
	std::map<Effects, bool> m_firstUpdates;
	std::map<Effects, Effect*> m_effects;
	float* m_worldTrans;
	float* m_worldNoTrans;
	bool m_noTranslateMode;
};

#endif