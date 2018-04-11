#include "hudAttackPrediction.h"

#include "fontManager.h"
#include "localizationData.h"
#include "renderer.h"
#include "actor.h"
#include "lerper.h"
#include "skill.h"

HudAttackPrediction::HudAttackPrediction(Actor* owner)
{
	_actor = owner;

	if (_actor->_Fighter->PredictedSkill != NULL)
		m_observedDamage = _actor->_Fighter->PredictedSkill->_preCalculatedDamage;
	else
		m_observedDamage = Damage();

	m_prevDamage._value = -43893893;// set this to a fucked up number so itll do it's first update

	// Box
	GraphComp_ptr ptr = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "res/sprites/special/blank.png"));
	//dynamic_cast<FontGraphicsComponent*>(ptr.get())->SetStatic(true);

	ptr->SetPhysics(_actor->_Graphics->GetPosRef().OnlyXY() - Vector3f(0.5f, -0.25f, -0.1f), Vector3f());
	ptr->GetModelMat()->SetScale(Vector3f(0.25f, 0.25f, 1));

	ptr->Update();

	// damage
	unsigned int font = FontManager::GetInstance().AddFont(false, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(font, 0.2f, 0.2f);
	FontManager::GetInstance().GetFont(font)->_letterSpacing = 0.55f;

	// Set everything
	_background = ptr;
	_damageFont = font;

	Update();
}

void HudAttackPrediction::Destroy()
{
	// Make sure to destroy the fonts
	FontManager::GetInstance().RemoveFont(_damageFont);

	HudComponent::Destroy();
}

void HudAttackPrediction::Update()
{
	if (_actor->_Fighter->PredictedSkill != NULL)
		m_observedDamage = _actor->_Fighter->PredictedSkill->_preCalculatedDamage;
	else
		m_observedDamage._value = 0;

	// Ensure that the value we're following actually changed to do something
	if (m_prevDamage._value == m_observedDamage._value && m_prevDamage._type == m_observedDamage._type)
		return;

	// Handle text display
	m_mustDisplay = m_mustDisplayNextUpdate;
	if (m_mustDisplay)
		FontManager::GetInstance().EnableFont(_damageFont);
	else
		FontManager::GetInstance().DisableFont(_damageFont);

	m_prevDamage = m_observedDamage;

	// Show damage type
	Vector3f color;
	if (m_observedDamage._type == SkillType::ST_Physical)
		color = Vector3f(0.5f, 0.10f, 0.025f);
	else if (m_observedDamage._type == SkillType::ST_Magical)
		color = Vector3f(0.3f, 0.05f, 0.5f);
	else if (m_observedDamage._type == SkillType::ST_Healing)
		color = Vector3f(0.05f, 0.5f, 0.1f);

	_background->SetColorAll(color, 0.7f);

	// Update the font
	Vector3f pos = _background->GetPosRef().OnlyXY() + Vector3f(0.5f, 0.375f, 0);
	std::string text = std::to_string(m_observedDamage._value);
	std::string current = FontManager::GetInstance().GetFont(_damageFont)->_text;

	// Only update font when it changes
	if (text != current)
		FontManager::GetInstance().SetText(_damageFont, text, pos, true);

	FontManager::GetInstance().GetFont(_damageFont)->Update(0);

	_background->Update();
}

void HudAttackPrediction::ToggleDisplay(bool display)
{
	m_mustDisplayNextUpdate = display;
}

void HudAttackPrediction::AdjustPosition()
{
	// Handle moving all components if the screen is resized
}

void HudAttackPrediction::SetRender()
{
	if (!m_mustDisplay)
		return;

	// FontManager handles setting the render for the label
	Renderer::GetInstance().Add(_background);
}
