#include "fade.h"

Fade::Fade() : m_fadeProgress(1.0f)
{
}

void Fade::SetFade(bool fadeIn)
{
	m_fadeIn = fadeIn;
	m_fadeProgress = fadeIn ? 0.01f : 0.99f;
}

bool Fade::IsDone()
{
	return (m_fadeProgress <= 0.0f || m_fadeProgress >= 1.0f);
}

void Fade::Begin()
{
	m_fadeProgress += m_fadeIn ? 0.06f : -0.06f;
	FadeEffect::GetInstance().Enable();
	FadeEffect::GetInstance().SetFade(m_fadeProgress);
}