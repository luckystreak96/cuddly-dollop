#include "effectManager.h"
#include "basicEffect.h"
#include "singleColorEffect.h"
#include "combineEffect.h"
#include "bloomEffect.h"
#include "blurEffect.h"
#include "fadeEffect.h"

EffectManager::EffectManager() : m_prevEffect(std::pair<Effect*, Effects>(NULL, E_Basic)), m_currentEffect(std::pair<Effect*, Effects>(NULL, E_Basic))
{
	for (int i = 0; i < E_Last; i++)
		m_firstUpdates.emplace((Effects)i, false);

	m_effects.emplace(E_Basic, &BasicEffect::GetInstance());
	m_effects.emplace(E_SingleColor, &SingleColorEffect::GetInstance());
	m_effects.emplace(E_Combine, &CombineEffect::GetInstance());
	m_effects.emplace(E_Bloom, &BloomEffect::GetInstance());
	m_effects.emplace(E_Blur, &BlurEffect::GetInstance());
	m_effects.emplace(E_Fade, &FadeEffect::GetInstance());
}

void EffectManager::ResetWorldUpdateFlag()
{
	for (auto &x : m_firstUpdates)
		x.second = false;
}

void EffectManager::SetAllTilePositions(float size)
{
	for (auto &x : m_effects)
	{
		x.second->Enable();
		x.second->SetTileSize(size);
	}
}

void EffectManager::SetWorldTrans(float* f)
{
	m_worldTrans = f;
}

void EffectManager::Enable(Effects ef, GLuint program)
{
	m_prevEffect = m_currentEffect;

	m_currentEffect.first = m_effects.at(ef);
	m_currentEffect.second = ef;

	m_currentEffect.first->Enable(program);
	UpdateWorld();
}

void EffectManager::EnablePrevious()
{
	if (m_prevEffect.first == NULL)
		return;

	std::pair<Effect*, Effects> temp = m_prevEffect;
	m_prevEffect = m_currentEffect;
	m_currentEffect = temp;

	m_currentEffect.first->Enable();
	UpdateWorld();
}

void EffectManager::UpdateWorld()
{
	// World trans not updated yet
	if (!m_firstUpdates.at(m_currentEffect.second))
	{
		m_currentEffect.first->SetWorldPosition(m_worldTrans);
		m_firstUpdates.at(m_currentEffect.second) = true;
	}
}