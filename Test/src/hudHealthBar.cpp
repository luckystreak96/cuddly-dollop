#include "hudHealthBar.h"

#include "fontManager.h"
#include "localizationData.h"
#include "renderer.h"
#include "actor.h"
#include "lerper.h"
#include "animXpBar.h"

HudHealthBar::HudHealthBar(std::pair<Fighter_ptr, Actor_ptr> ap, Vector3f position)
{
	// Set initial values
	_actor = ap;
	m_observed = &ap.first->Health;
	m_max = ap.first->GetMaxHealth().Modified;
	std::string name = ap.first->GetName();

	m_observedMaxHP = &ap.first->GetMaxHealthPointer()->Modified;

	m_startXP = ap.first->GetExp();
	m_currentLevel = ap.first->GetLevel();

	m_observedXP = ap.first->GetExp();
	m_xpMax = ap.first->CalculateNextLevelExp() - ap.first->CalculateLevelExp(ap.first->GetLevel() - 1);

	m_prevValue = m_prevXP = m_prevMaxHP = -43893893;// set this to a fucked up number so itll do it's first update

	// Smaller bars
	{
		Vector3f scale(0.33, 0.3, 1);

		_smallForeground = GraphComp_ptr(new FontGraphicsComponent("BAR", "res/sprites/special/bar_foreground.png"));
		_smallBackground = GraphComp_ptr(new FontGraphicsComponent("BAR", "res/sprites/special/bar_background.png"));

		m_actorPrevPos = _actor.second->GetPosRef();
		_smallForeground->GetModelMat()->SetScale(scale);
		_smallBackground->GetModelMat()->SetScale(scale);
		_smallBackground->SetColorAll(Vector3f(0.08f, 0.4f, 1.0f));
		UpdateSmallHPPosition();
	}

	// Bar
	GraphComp_ptr ptr = GraphComp_ptr(new FontGraphicsComponent("BAR", "res/sprites/special/bar.png"));
	GraphComp_ptr ptr2 = GraphComp_ptr(new FontGraphicsComponent("BAR", "res/sprites/special/bar.png"));
	GraphComp_ptr ptrExp = GraphComp_ptr(new FontGraphicsComponent("BAR", "res/sprites/special/blank.png"));
	dynamic_cast<FontGraphicsComponent*>(ptr.get())->SetStatic(true);
	dynamic_cast<FontGraphicsComponent*>(ptr2.get())->SetStatic(true);
	dynamic_cast<FontGraphicsComponent*>(ptrExp.get())->SetStatic(true);

	ptr->SetPhysics(position, Vector3f());
	ptr2->SetPhysics(position + Vector3f(0, 0, 0.1f), Vector3f());
	ptrExp->SetPhysics(position + Vector3f(0, -0.07f, 0.1f), Vector3f());
	ptrExp->GetModelMat()->SetScale(Vector3f(fmax(((float)m_observedXP - (float)ap.first->CalculateLevelExp(ap.first->GetLevel() - 1)) / (float)m_xpMax, 0.005f), 0.1f, 1));

	ptr2->SetColorAll(Vector3f(3), 0.05f);
	ptrExp->SetColorAll(Vector3f(0.6f, 0.5f, 0.05f), 1.f);

	ptr->Update();
	ptr2->Update();
	ptrExp->Update();

	// health 
	unsigned int font = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(font, 0.25f, 0.25f);
	FontManager::GetInstance().GetFont(font)->_letterSpacing = 0.55f;

	// name
	unsigned int namefont = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(namefont, 0.25f, 0.25f);
	FontManager::GetInstance().GetFont(namefont)->_letterSpacing = 0.6f;
	Vector3f offset = Vector3f(0.05f, 0.275f, -1);
	FontManager::GetInstance().SetText(namefont, _(name), ptr->GetPos() + offset, false);
	FontManager::GetInstance().GetFont(namefont)->Update(0);

	// level
	_levelFont = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(_levelFont, 0.25f, 0.25f);
	FontManager::GetInstance().GetFont(_levelFont)->_letterSpacing = 0.4f;
	offset = Vector3f(1.45f, 0.275f, -1);
	FontManager::GetInstance().SetText(_levelFont, "Lv " + std::to_string(m_currentLevel), ptr->GetPos() + offset, false);
	FontManager::GetInstance().GetFont(_levelFont)->Update(0);

	// Set everything
	_foreground = ptr;
	_background = ptr2;
	_xpBar = ptrExp;
	_healthFont = font;
	_nameFont = namefont;

	Update();
}

void HudHealthBar::Destroy()
{
	// Make sure to destroy the fonts
	FontManager::GetInstance().RemoveFont(_nameFont);
	FontManager::GetInstance().RemoveFont(_healthFont);
	FontManager::GetInstance().RemoveFont(_levelFont);

	HudComponent::Destroy();
}

void HudHealthBar::UpdateSmallHPPosition()
{
	m_actorPrevPos = _actor.second->GetPosRef();
	_smallForeground->SetPhysics(m_actorPrevPos + Vector3f(0.18f, -0.1f, -0.1f), Vector3f());
	_smallBackground->SetPhysics(m_actorPrevPos + Vector3f(0.18f, -0.1f, 0.1f), Vector3f());
	_smallForeground->Update();
	_smallBackground->Update();
}

void HudHealthBar::Update()
{
	if (_actor.second->GetPosRef() != m_actorPrevPos)
		UpdateSmallHPPosition();

	// Ensure that the value we're following actually changed to do something
	if (m_prevValue == *m_observed && m_prevXP == m_observedXP && *m_observedMaxHP == m_prevMaxHP)
		return;

	// Handle updating text and health bar here
	Vector3f color = Vector3f(0, 0.6f, 0.8f);
	int health = *m_observed;

	m_prevValue = health;
	m_max = *m_observedMaxHP;
	m_prevMaxHP = *m_observedMaxHP;

	// Set color
	if (health > m_max / 2)
		color = Vector3f(0.08f, 0.4f, 1.0f);
	else if (health <= m_max / 8)
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
		std::string text = std::to_string(health) + " / " + std::to_string(*m_observedMaxHP);
		std::string current = FontManager::GetInstance().GetFont(_healthFont)->_text;

		// Only update font when it changes
		if (current.substr(0, current.length() - 1) != text)
		{
			FontManager::GetInstance().SetText(_healthFont, text, _foreground->GetPos() + offset, false);
			FontManager::GetInstance().GetFont(_healthFont)->GetGraphics()->SetColorAll(color + Vector3f(0.6f), 1.0f);
		}

		FontManager::GetInstance().GetFont(_healthFont)->Update(0);
	}

	_smallBackground->SetColorAll(color * Vector3f(1.5f), 0.9f);
	_smallBackground->GetModelMat()->SetScale(fmax((float)health / (float)m_max, 0) / 3.0f, 0.3f, 1);
	_smallBackground->Update();

	_foreground->SetColorAll(color, 0.9f);
	_foreground->GetModelMat()->SetScale(fmax((float)health / (float)m_max, 0), 1, 1);
	_foreground->Update();
	_background->Update();
}


Anim_ptr HudHealthBar::SetupExpAnimation(int targetXP)
{
	m_targetXP = targetXP;
	m_targetLevel = _actor.first->GetLevel();
	Anim_ptr result = std::make_shared<AnimXpBar>(AnimXpBar(this, m_observedXP, m_targetXP));
	return result;
}

bool HudHealthBar::UpdateExpAnimation(float newxp)
{
	float actualXP = newxp;
	if (actualXP > m_targetXP)
		actualXP = m_targetXP;

	newxp -= _actor.first->CalculateLevelExp(m_currentLevel - 1);
	float max = _actor.first->CalculateLevelExp(m_currentLevel) - _actor.first->CalculateLevelExp(m_currentLevel - 1);

	// Level up
	if (newxp >= max)
	{
		// Update the level font
		m_currentLevel++;
		Vector3f offset = Vector3f(1.45f, 0.275f, -1);
		FontManager::GetInstance().SetText(_levelFont, "Lv " + std::to_string(m_currentLevel), _foreground->GetPos() + offset, false);

		FontManager::GetInstance().CreateFloatingText(_actor.second->GetPosRef(), "Level up!", Vector3f(0.95f, 0.8f, 0.05f));
		return UpdateExpAnimation(actualXP);
	}

	_xpBar->GetModelMat()->SetScale(Vector3f(fmax(newxp / (float)max, 0.005f), 0.1f, 1));
	_xpBar->Update();

	_actor.first->SetExp((int)actualXP);

	// done
	if (actualXP >= m_targetXP)
		return true;

	// not done
	return false;
}

void HudHealthBar::SetRender()
{
	// FontManager handles setting the render for the label

	Renderer::GetInstance().Add(_smallForeground);
	Renderer::GetInstance().Add(_smallBackground);
	Renderer::GetInstance().Add(_foreground);
	Renderer::GetInstance().Add(_background);
	Renderer::GetInstance().Add(_xpBar);
}
