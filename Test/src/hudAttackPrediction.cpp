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

	m_prevActorPos = _actor->_Graphics->GetPosRef();

	if (_actor->_Fighter->PredictedSkill != NULL)
		m_observedDamage = _actor->_Fighter->PredictedSkill->_preCalculatedDamage;
	else
		m_observedDamage = Damage();

	m_prevDamage._value = -43893893;// set this to a fucked up number so itll do it's first update

	// Box
	_background = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE", "res/sprites/special/blank.png"));
	//dynamic_cast<FontGraphicsComponent*>(ptr.get())->SetStatic(true);

	SetBoxPosition();
	_background->GetModelMat()->SetScale(Vector3f(0.25f, 0.25f, 1));

	_background->Update();

	// damage
	unsigned int font = FontManager::GetInstance().AddFont(false, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(font, 0.2f, 0.2f);
	FontManager::GetInstance().GetFont(font)->_letterSpacing = 0.55f;

	// Set everything
	_damageFont = font;

	Update();
}

void HudAttackPrediction::SetBoxPosition()
{
	_background->SetPhysics(m_prevActorPos.OnlyXY() - Vector3f(0.5f, -0.25f, -0.1f), Vector3f());
}

Vector3f HudAttackPrediction::CalculateTextPosition()
{
	return _background->GetPosRef().OnlyXY() + Vector3f(0.5f, 0.375f, 0);
}

void HudAttackPrediction::Destroy()
{
	// Make sure to destroy the fonts
	FontManager::GetInstance().RemoveFont(_damageFont);

	HudComponent::Destroy();
}

void HudAttackPrediction::Update()
{
	Vector3f pos = _actor->_Graphics->GetPosRef();
	if (m_prevActorPos != pos)
	{
		m_prevActorPos = pos;

		// box
		SetBoxPosition();
		_background->Update();

		// font
		FontManager::GetInstance().GetFont(_damageFont)->UpdateModel(CalculateTextPosition());
		// Necessary for window size changes
		FontManager::GetInstance().GetFont(_damageFont)->SetOffset(CalculateTextPosition());
	}

	if (_actor->_Fighter->PredictedSkill != NULL)
		m_observedDamage = _actor->_Fighter->PredictedSkill->_preCalculatedDamage;
	else
		m_observedDamage._value = 0;

	// Ensure that the value we're following actually changed to do something
	if (m_prevDamage._value == m_observedDamage._value && m_prevDamage._type == m_observedDamage._type && m_mustDisplayNextUpdate == m_mustDisplay)
		return;

	// Handle text display
	m_mustDisplay = m_mustDisplayNextUpdate;
	if (m_mustDisplay && !_actor->_Fighter->Dead)
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
	//pos = CalculateTextPosition();
	std::string text = std::to_string(m_observedDamage._value);
	std::string current = FontManager::GetInstance().GetFont(_damageFont)->_text;

	// Only update font when it changes
	if (text != current)
	{
		FontManager::GetInstance().SetText(_damageFont, text, Vector3f(), true);
		FontManager::GetInstance().GetFont(_damageFont)->UpdateModel(CalculateTextPosition());
		FontManager::GetInstance().GetFont(_damageFont)->SetOffset(CalculateTextPosition());
	}

	//FontManager::GetInstance().GetFont(_damageFont)->Update(0);
	//FontManager::GetInstance().GetFont(_damageFont)->UpdateModel(CalculateTextPosition());

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
	if (!m_mustDisplay || _actor->_Fighter->Dead)
	{
		if (_actor->_Fighter->Dead)
			FontManager::GetInstance().DisableFont(_damageFont);
		return;
	}

	if (m_mustDisplay && !_actor->_Fighter->Dead)
		FontManager::GetInstance().EnableFont(_damageFont);

	// FontManager handles setting the render for the label
	Renderer::GetInstance().Add(_background);
}

bool HudAttackPrediction::GetDisplay()
{
	return m_mustDisplay;
}