#include "hudHealthBar.h"

HudHealthBar::HudHealthBar(int* observable)
{
	m_observed = observable;
	m_prevValue = *m_observed;
}

void HudHealthBar::Update()
{
	if (m_prevValue == *m_observed)
		return;

	// Handle updating text and health bar here
}

void HudHealthBar::AdjustPosition()
{
	// Handle moving all components if the screen is resized
}
