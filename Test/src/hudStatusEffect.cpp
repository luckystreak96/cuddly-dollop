#include "hudStatusEffect.h"

#include "fontManager.h"
#include "localizationData.h"
#include "renderer.h"
#include "actor.h"
#include "lerper.h"
#include "skill.h"

HudStatusEffect::HudStatusEffect(BattleUnit u)
{
	m_mustDisplay = true;
	_unit = u;

	m_prevActorPos = *u.position;//_actor.second->get_position_ref();

	m_status_count = 0;//_actor.first->_OrderPosition;

	m_prevOrder = -43893893;// set this to a fucked up number so itll do it's first update

	// Box
	_background = std::shared_ptr<GraphicsComponent>(new GraphicsComponent("CENTERED_TILE", "res/sprites/special/blank.png"));
	//dynamic_cast<FontGraphicsComponent*>(ptr.get())->SetStatic(true);

	SetBoxPosition();
	_background->GetModelMat()->SetScale(Vector3f(0.25f, 0.25f, 1));

	_background->Update();

	// damage
	unsigned int font = FontManager::GetInstance().AddFont(false, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(font, 0.2f, 0.2f);
	//FontManager::GetInstance().GetFont(font)->_letterSpacing = 0.55f;

	// Set everything
	_turnFont = font;

	Update((StatUser*)_unit.stats);
}

void HudStatusEffect::SetBoxPosition()
{
	float size = 64.0f;
    _background->set_position(m_prevActorPos.OnlyXY() - Vector3f(0.5f * size, -0.25f * size, -0.1f));
}

Vector3f HudStatusEffect::CalculateTextPosition()
{
	float size = 64.0f;
	return _background->get_position_ref().OnlyXY() + Vector3f(0.5f * size, 0.375f * size, 0);
}

void HudStatusEffect::Destroy()
{
	// Make sure to destroy the fonts
	FontManager::GetInstance().RemoveFont(_turnFont);

	HudComponent::Destroy();
}

void HudStatusEffect::Update(Observable* obs)
{
	Fighter* fighter = dynamic_cast<Fighter*>(obs);


	Vector3f pos = *_unit.position;
	if (m_prevActorPos != pos)
	{
		m_prevActorPos = pos;

		// box
		SetBoxPosition();
		_background->Update();

		// font
		FontManager::GetInstance().GetFont(_turnFont)->UpdateModel(CalculateTextPosition());
		// Necessary for window size changes
		FontManager::GetInstance().GetFont(_turnFont)->SetOffset(CalculateTextPosition());
	}

	if (fighter != nullptr)
		m_status_count = (int)fighter->_Statuses.size();

	// Ensure that the value we're following actually changed to do something
	if (m_prevOrder == m_status_count && m_mustDisplayNextUpdate == m_mustDisplay)
		return;

	m_prevOrder = m_status_count;

	// Show status type
	Vector3f color = Vector3f(0.5f, 0.10f, 0.025f);

	_background->SetColorAll(color, 0.7f);

	// Update the font
	//pos = CalculateTextPosition();
	std::string text = std::to_string(m_status_count);
	std::string current = FontManager::GetInstance().GetFont(_turnFont)->_text;

	// Only update font when it changes
	if (text != current)
	{
		FontManager::GetInstance().SetText(_turnFont, text, Vector3f(), true);
		FontManager::GetInstance().GetFont(_turnFont)->UpdateModel(CalculateTextPosition());
		FontManager::GetInstance().GetFont(_turnFont)->SetOffset(CalculateTextPosition());
	}

	_background->Update();
}

void HudStatusEffect::ToggleDisplay(bool display)
{
	//m_mustDisplayNextUpdate = display;
	m_mustDisplay = !display;
}

void HudStatusEffect::AdjustPosition()
{
	// Handle moving all components if the screen is resized
}

void HudStatusEffect::SetRender()
{
	if (!m_mustDisplay)
	{
		//if (_actor->_Fighter->Dead)
		FontManager::GetInstance().DisableFont(_turnFont);
		return;// returning here allows us to not render the bg
	}

	if (m_mustDisplay)
		FontManager::GetInstance().EnableFont(_turnFont);

	// FontManager handles setting the render for the label
	Renderer::GetInstance().Add(_background);
}

bool HudStatusEffect::GetDisplay()
{
	return m_mustDisplay;
}