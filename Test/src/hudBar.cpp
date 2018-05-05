#include "hudBar.h"

#include "fontManager.h"
#include "localizationData.h"
#include "renderer.h"
#include "actor.h"

HudBar::HudBar(int* ap, int max, Vector3f position)
{
	m_observed = ap;
	m_prevValue = -43893893;// set this to a fucked up number so itll do it's first update
	m_max = max;

	// Bar
	GraphComp_ptr ptr = GraphComp_ptr(new FontGraphicsComponent("BAR", "res/sprites/special/bar.png"));
	dynamic_cast<FontGraphicsComponent*>(ptr.get())->SetStatic(true);

	ptr->SetPhysics(position, Vector3f());
	ptr->Update();

	// Set everything
	_foreground = ptr;

	Update();
}

void HudBar::Destroy()
{
	HudComponent::Destroy();
}

void HudBar::Update()
{
	// Ensure that the value we're following actually changed to do something
	if (m_prevValue == *m_observed)
		return;

	// Handle updating text and health bar here
	Vector3f color = Vector3f(0, 0.6f, 0.8f);
	int value = *m_observed;

	_foreground->SetColorAll(color, 0.9f);
	_foreground->GetModelMat()->SetScale(fmax((float)value / (float)m_max, 0), 1, 1);
	_foreground->Update();
}

void HudBar::SetRender()
{
	// FontManager handles setting the render for the label

	Renderer::GetInstance().Add(_foreground);
}
