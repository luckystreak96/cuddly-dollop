#include "hudTurnOrder.h"

#include "fontManager.h"
#include "localizationData.h"
#include "renderer.h"
#include "actor.h"
#include "lerper.h"
#include "skill.h"

HudTurnOrder::HudTurnOrder(Actor* owner)
{
	_actor = owner;

	m_prevActorPos = _actor->_Graphics->GetPosRef();

	m_observedOrder = _actor->_Fighter->_OrderPosition;

	m_prevOrder = -43893893;// set this to a fucked up number so itll do it's first update

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

void HudTurnOrder::SetBoxPosition()
{
	_background->SetPhysics(m_prevActorPos.OnlyXY() - Vector3f(0.5f, -0.25f, -0.1f), Vector3f());
}

Vector3f HudTurnOrder::CalculateTextPosition()
{
	return _background->GetPosRef().OnlyXY() + Vector3f(0.5f, 0.375f, 0);
}

void HudTurnOrder::Destroy()
{
	// Make sure to destroy the fonts
	FontManager::GetInstance().RemoveFont(_damageFont);

	HudComponent::Destroy();
}

void HudTurnOrder::Update()
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

	m_observedOrder = _actor->_Fighter->_OrderPosition;

	// Ensure that the value we're following actually changed to do something
	if (m_prevOrder == m_observedOrder && m_mustDisplayNextUpdate == m_mustDisplay)
		return;

	// Handle text display
	m_mustDisplay = m_mustDisplayNextUpdate;
	if (m_mustDisplay && !_actor->_Fighter->Dead)
		FontManager::GetInstance().EnableFont(_damageFont);
	else
		FontManager::GetInstance().DisableFont(_damageFont);

	m_prevOrder = m_observedOrder;

	// Show damage type
	Vector3f color = Vector3f(0.5f, 0.10f, 0.025f);
	//if (m_observedOrder._type == SkillType::ST_Physical)
	//	color = Vector3f(0.5f, 0.10f, 0.025f);
	//else if (m_observedOrder._type == SkillType::ST_Magical)
	//	color = Vector3f(0.3f, 0.05f, 0.5f);
	//else if (m_observedOrder._type == SkillType::ST_Healing)
	//	color = Vector3f(0.05f, 0.5f, 0.1f);

	_background->SetColorAll(color, 0.7f);

	// Update the font
	//pos = CalculateTextPosition();
	std::string text = std::to_string(m_observedOrder);
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

void HudTurnOrder::ToggleDisplay(bool display)
{
	m_mustDisplayNextUpdate = display;
}

void HudTurnOrder::AdjustPosition()
{
	// Handle moving all components if the screen is resized
}

void HudTurnOrder::SetRender()
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

bool HudTurnOrder::GetDisplay()
{
	return m_mustDisplay;
}