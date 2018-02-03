#include "dialogueBox.h"
#include <GLFW/glfw3.h>

DialogueBox* DialogueBox::m_owner = NULL;

DialogueBox::DialogueBox(unsigned int entity_id, std::vector<Dialogue> d, std::vector<DialogueChoice> dc) : m_firstTime(true)
{
	m_dialogueGraph = std::shared_ptr<DialogueGraph>(new DialogueGraph(d, dc));
	m_target = entity_id;

	Construct();
}

DialogueBox::DialogueBox(unsigned int entity_id, std::shared_ptr<DialogueGraph> dg) : m_firstTime(true)
{
	m_dialogueGraph = dg;
	m_target = entity_id;

	Construct();
}

void DialogueBox::Construct()
{
	m_xScale = 0.25f;
	m_yScale = 0.25f;

	m_box = std::shared_ptr<FontGraphicsComponent>(new FontGraphicsComponent("TILE", "res/sprites/special/dialogue.png"));
	m_box->SetPhysics(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0, 0, 0));
	m_box->SetScale(Vector3f(1.0f, 0.5f, 1.0f));
	m_box->Update();
	m_box->SetStatic(true);

	m_phys.SetPosition(Vector3f());
	m_static = true;
	m_lockLevel = 1;
	m_mode = BLOCKING;
}


DialogueBox::~DialogueBox()
{
}


void DialogueBox::Draw()
{
	m_box->Draw();
	m_graphics->Draw();
	for (auto x : m_choices)
		x->Draw();
}

void DialogueBox::SetText(std::string text)
{
	float x = 1.0f;
	m_y = 5.0f * m_box->GetScale().y - m_yScale;
	Font::SetText(text, Vector3f(x, m_y, 0.0f), false, m_maxWidth);
	m_choices.clear();
	if (m_dialogueGraph && m_dialogueGraph->ChoiceAvailable())
	{
		//m_y += m_yScale * 1.25f;
		for (auto x : m_dialogueGraph->GetChoices())
		{
			m_y -= m_yScale * 1.25f;
			Font_ptr temp = Font_ptr(new Font(true));
			temp->SetScale(m_xScale, m_yScale);
			temp->SetTextSpeed(1);
			temp->SetText(x, Vector3f(0.75f, m_yScale * 9.0f + m_y, 0), false, m_maxWidth);
			m_choices.push_back(temp);
		}
	}
}

EventUpdateResponse DialogueBox::UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;
	eur.Queue = std::shared_ptr<EventQueue>(new EventQueue());

	if (m_owner != this && m_owner != NULL)
	{
		eur.IsDone = false;
		return eur;
	}

	if (m_firstTime)
	{
		m_owner = this;
		SetScale(m_xScale, m_yScale);
		m_firstTime = false;
	}

	// Don't update it if its completed
	if (m_completed)
	{
		if (m_owner == this)
			m_owner = NULL;
		return eur;
	}
	else
		if (m_target <= ents->size() && m_target != 0)
			ents->at(m_target)->Physics()->SetConversationLock(true);

	// Update the font
	Font::Update(elapsedTime);
	UpdateBox();

	for (unsigned int i = 0; i < m_choices.size(); i++)
	{
		if (m_dialogueGraph->SelectedChoice() == i)
			m_choices.at(i)->GetGraphics()->SetTexture("res/fonts/lowercase_selected.png");
		else
			m_choices.at(i)->GetGraphics()->SetTexture("res/fonts/lowercase.png");

		if (TextDisplayDone())
			m_choices.at(i)->Update(elapsedTime);
	}

	bool choices_done = true;
	for (auto x : m_choices)
		if (!x->TextDisplayDone())
			choices_done = false;

	// When you press space, set up the textbox to be destroyed
	if (TextDisplayDone() && choices_done)
	{
		if (InputManager::GetInstance().FrameKeyStatus(' ', KeyStatus::KeyPressed, 1))
		{
			// SendInput returns false if its done
			if (m_dialogueGraph == NULL)
			{
				m_completed = true;
				if (m_target <= ents->size())
					ents->at(m_target)->Physics()->SetConversationLock(false);
				m_owner = NULL;
				return eur;
			}

			DialogueResponse dr = m_dialogueGraph->SendInput(IT_Action);
			eur.Queue = dr.Queue;

			if (dr.NotDone)
			{
				SetText(m_dialogueGraph->GetCurrentText());
			}
			else
			{
				m_completed = true;
				if (m_target <= ents->size())
					ents->at(m_target)->Physics()->SetConversationLock(false);
				m_owner = NULL;
				return eur;
			}
		}
		else if (m_dialogueGraph != NULL && m_dialogueGraph->ChoiceAvailable() && InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_DOWN, KeyStatus::KeyPressed, 1))
		{
			m_dialogueGraph->SendInput(IT_Down);
		}
		else if (m_dialogueGraph != NULL && InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_UP, KeyStatus::KeyPressed, 1))
		{
			m_dialogueGraph->SendInput(IT_Up);
		}
	}
	else if (InputManager::GetInstance().FrameKeyStatus(' ', KeyStatus::AnyPress, 1))
	{
		m_textSpeed = 7;
	}
	else
	{
		m_textSpeed = 1;
	}

	SetRender();

	// The dialogue is not done
	eur.IsDone = false;
	return eur;
}

void DialogueBox::UpdateBox()
{
	float width = OrthoProjInfo::GetRegularInstance().Right * 2.f / OrthoProjInfo::GetRegularInstance().Size - 1.f;
	m_box->SetScale(Vector3f(width, m_yScale * 10.0f, 0));
	m_box->Update();
}

// Deprecated method
void DialogueBox::Update(double elapsedTime)
{
	Font::Update(elapsedTime);
	m_box->Update();

	// When you press space, set up the textbox to be destroyed next frame
	if (TextDisplayDone() && InputManager::GetInstance().FrameKeyStatus(' ', KeyStatus::KeyPressed, 1))
	{
		m_temporary = true;
		m_lifetime = 0;
		InputManager::GetInstance().SetLockLevel(0);
	}
}

void DialogueBox::SetRender()
{
	if (m_box != NULL)
		Renderer::GetInstance().Add(m_box);
	if (m_graphics != NULL)
		Renderer::GetInstance().Add(m_graphics);
	for (auto x : m_choices)
		x->SetRender();
}

void DialogueBox::SetScale(float xScale, float yScale)
{
	Font::SetScale(xScale, yScale);

	m_maxWidth = (OrthoProjInfo::GetRegularInstance().Right * 2.f / OrthoProjInfo::GetRegularInstance().Size) - 1.5f/* / xScale*/;
	m_maxHeight = 4.5f;

	if (m_dialogueGraph != NULL)
		SetText(m_dialogueGraph->GetCurrentText());
}

void DialogueBox::ResetEvent()
{
	m_firstTime = true;
	m_completed = false;
	m_dialogueGraph->SetToStart();
	SetText(m_dialogueGraph->GetCurrentText());
}

std::shared_ptr<IEvent> DialogueBox::Clone()
{
	std::shared_ptr<DialogueGraph> dg = m_dialogueGraph->Clone();
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new DialogueBox(m_target, dg));
	SetCloneBaseAttributes(result);
	return result;
}
