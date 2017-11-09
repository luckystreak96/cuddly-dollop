#include "dialogueBox.h"

DialogueBox::DialogueBox(unsigned int entity_id, DialogueGraph* dg) : m_box(NULL), Font()
{
	m_dialogueGraph = dg;
	m_target = entity_id;

	m_box = new FontGraphicsComponent("DIALOGUE_BOX", "res/dialogue.png");
	m_box->SetPhysics(Vector3f(0, 0, 0.5f), Vector3f(0, 0, 0));
	m_box->Update();
	m_box->SetStatic(true);

	m_phys.SetPosition(Vector3f());
	m_static = true;
	m_lockLevel = 1;
	m_mode = BLOCKING;

	SetScale(0.5f, 0.5f);
}

DialogueBox::DialogueBox(unsigned int entity_id, std::vector<Dialogue> d, std::vector<DialogueChoice> dc)
{
	m_dialogueGraph = new DialogueGraph(d, dc);
	m_target = entity_id;

	m_box = new FontGraphicsComponent("DIALOGUE_BOX", "res/dialogue.png");
	m_box->SetPhysics(Vector3f(0, 0, 0.5f), Vector3f(0, 0, 0));
	m_box->Update();
	m_box->SetStatic(true);

	m_phys.SetPosition(Vector3f());
	m_static = true;
	m_lockLevel = 1;
	m_mode = BLOCKING;

	SetScale(0.5f, 0.5f);
}


DialogueBox::~DialogueBox()
{
	if (m_dialogueGraph)
		delete m_dialogueGraph;
	for (auto c : m_choices)
		delete c;
	if (m_box)
		delete m_box;
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
	Font::SetText(text, Vector3f(0.5f, 4.0f, 0.0f), false, m_maxWidth);
	for (auto c : m_choices)
		delete c;
	m_choices.clear();
	if (m_dialogueGraph && m_dialogueGraph->ChoiceAvailable())
	{
		m_y -= m_yScale / 2.0f;
		for (auto x : m_dialogueGraph->GetChoices())
		{
			m_y -= m_yScale * 1.25f;
			Font* temp = new Font(true);
			temp->SetScale(m_xScale, m_yScale);
			temp->SetTextSpeed(1);
			temp->SetText(x, Vector3f(0.75f, 4.0f + m_y, 0), false, m_maxWidth);
			m_choices.push_back(temp);
		}
	}
}

EventUpdateResponse DialogueBox::UpdateEvent(double elapsedTime, std::map<unsigned int, Entity*>* ents)
{
	EventUpdateResponse eur = EventUpdateResponse();
	eur.IsDone = true;
	eur.Queue = EventQueue();

	// Don't update it if its completed
	if (m_completed)
		return eur;
	else
		if (m_target <= ents->size())
			ents->at(m_target)->Physics()->SetConversationLock(true);

	// Update the font
	Font::Update(elapsedTime);

	for (int i = 0; i < m_choices.size(); i++)
	{
		if (m_dialogueGraph->SelectedChoice() == i)
			m_choices.at(i)->GetGraphics()->SetTexture("res/fonts/selected.png");
		else
			m_choices.at(i)->GetGraphics()->SetTexture("res/fonts/basic.png");

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
				return eur;
			}
		}
		else if (m_dialogueGraph != NULL && m_dialogueGraph->ChoiceAvailable() && InputManager::GetInstance().FrameKeyStatus(GLUT_KEY_DOWN + InputManager::SpecialKeyValue, KeyStatus::KeyPressed, 1))
		{
			m_dialogueGraph->SendInput(IT_Down);
		}
		else if (m_dialogueGraph != NULL && InputManager::GetInstance().FrameKeyStatus(GLUT_KEY_UP + InputManager::SpecialKeyValue, KeyStatus::KeyPressed, 1))
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

void DialogueBox::Update(double elapsedTime)
{
	Font::Update(elapsedTime);

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

	m_maxWidth = 29.0f/* / xScale*/;
	m_maxHeight = 4.0f/* / yScale*/;

	if (m_dialogueGraph != NULL)
		SetText(m_dialogueGraph->GetCurrentText());
}

void DialogueBox::ResetEvent()
{
	m_completed = false;
	m_dialogueGraph->SetToStart();
	SetText(m_dialogueGraph->GetCurrentText());
}