#include "hudHealthBar.h"

#include "fontManager.h"
#include "localizationData.h"
#include "renderer.h"

HudHealthBar::HudHealthBar(int* observable, int maxHealth, Vector3f position, std::string name)
{
	m_observed = observable;
	m_prevValue = -43893893;// set this to a fucked up number so itll do it's first update
	m_maxHealth = maxHealth;

	// Bar
	GraphComp_ptr ptr = GraphComp_ptr(new FontGraphicsComponent("BAR", "res/sprites/special/bar.png"));
	GraphComp_ptr ptr2 = GraphComp_ptr(new FontGraphicsComponent("BAR", "res/sprites/special/bar.png"));
	dynamic_cast<FontGraphicsComponent*>(ptr.get())->SetStatic(true);
	dynamic_cast<FontGraphicsComponent*>(ptr2.get())->SetStatic(true);

	ptr2->SetPhysics(position + Vector3f(0, 0, 0.1f), Vector3f());
	ptr->SetPhysics(position, Vector3f());

	ptr->Update();
	ptr2->SetColorAll(Vector3f(3), 0.05f);
	//ptr2->SetColorAll(Vector3f(0.5f, 0.5f, 0.5f), 0.2f);
	ptr2->Update();

	// health 
	unsigned int font = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(font, 0.25f, 0.25f);
	FontManager::GetInstance().GetFont(font)->_letterSpacing = 0.55f;

	// name
	unsigned int namefont = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(namefont, 0.25f, 0.25f);
	FontManager::GetInstance().GetFont(namefont)->_letterSpacing = 0.75f;
	Vector3f offset = Vector3f(0.05f, 0.275f, -1);
	//Vector3f offset = Vector3f(-1.45f, -0.25f, -1);
	FontManager::GetInstance().SetText(namefont, _(name), ptr->GetPos() + offset, false);
	FontManager::GetInstance().GetFont(namefont)->Update(0);

	// Set everything
	_foreground = ptr;
	_background = ptr2;
	_healthFont = font;
	_nameFont = namefont;

	Update();
}

void HudHealthBar::Destroy()
{
	// Make sure to destroy the fonts
	FontManager::GetInstance().RemoveFont(_nameFont);
	FontManager::GetInstance().RemoveFont(_healthFont);

	HudComponent::Destroy();
}

void HudHealthBar::Update()
{
	// Ensure that the value we're following actually changed to do something
	if (m_prevValue == *m_observed)
		return;

	// Handle updating text and health bar here
	Vector3f color = Vector3f(0, 0.6f, 0.8f);
	int health = *m_observed;

	// Set color
	if (health > m_maxHealth / 2)
		color = Vector3f(0.08f, 0.4f, 1.0f);
	else if (health <= m_maxHealth / 10)
		color = Vector3f(0.8f, 0, 0.4f);
	else
		color = Vector3f(1.0f, 0.6f, 0.2f);

	// Update the font
	if (health <= 0)
		FontManager::GetInstance().DisableFont(_healthFont);
	else
	{
		FontManager::GetInstance().EnableFont(_healthFont);
		Vector3f offset = Vector3f(0.075f, 0.0f, -1);
		//Vector3f offset = Vector3f(-0.75f, -0.7f, -1);
		std::string text = std::to_string(health) + " / " + std::to_string(m_maxHealth);
		std::string current = FontManager::GetInstance().GetFont(_healthFont)->_text;

		// Only update font when it changes
		if (current.substr(0, current.length() - 1) != text)
		{
			FontManager::GetInstance().SetText(_healthFont, text, _foreground->GetPos() + offset, false);
			FontManager::GetInstance().GetFont(_healthFont)->GetGraphics()->SetColorAll(color + Vector3f(0.6f), 1.0f);
		}

		FontManager::GetInstance().GetFont(_healthFont)->Update(0);
	}

	_foreground->SetColorAll(color, 0.9f);
	_foreground->GetModelMat()->SetScale(fmax((float)health / (float)m_maxHealth, 0), 1, 1);
	_foreground->Update();
	_background->Update();
}

void HudHealthBar::AdjustPosition()
{
	// Handle moving all components if the screen is resized
}

void HudHealthBar::SetRender()
{
	// FontManager handles setting the render for the label

	Renderer::GetInstance().Add(_foreground);
	Renderer::GetInstance().Add(_background);
}
