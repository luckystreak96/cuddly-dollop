#include "fade.h"
#include "effectManager.h"
#include "elapsedTime.h"

Fade::Fade() : m_fadeProgress(1.0f)
{
}

void Fade::SetFade(bool fadeIn)
{
	m_fadeIn = fadeIn;
	m_fadeProgress = fadeIn ? 0.01f : 0.99f;
}

void Fade::ForceFadeValue(float value)
{
	m_fadeProgress = value;
}

bool Fade::IsDone()
{
	return (m_fadeProgress <= 0.0f && !m_fadeIn || m_fadeProgress >= 1.0f && m_fadeIn);
}

void Fade::Begin()
{
	m_fadeProgress += (m_fadeIn ? 2.f : -2.f) * (float)ElapsedTime::GetInstance().GetElapsedTime();
	//FadeEffect::GetInstance().SetFade(m_fadeProgress);
	EffectManager::GetInstance().Enable(E_Gamma);
	EffectManager::GetInstance().SetLightIntensity(m_fadeProgress <= 0 ? 0 : m_fadeProgress);
}