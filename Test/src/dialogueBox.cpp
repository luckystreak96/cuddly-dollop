#include "dialogueBox.h"
#include <GLFW/glfw3.h>
#include <cassert>

#include "localizationData.h"

DialogueBox* DialogueBox::m_owner = NULL;

DialogueBox::DialogueBox(unsigned int entity_id, std::vector<Dialogue> d, std::vector<DialogueChoice> dc) : m_firstTime(true)
{
	auto graph = std::shared_ptr<DialogueGraph>(new DialogueGraph(d, dc));
	m_dialogueGraphs.emplace("english", graph);
	m_target = entity_id;

	Construct();
}

DialogueBox::DialogueBox(unsigned int entity_id, std::shared_ptr<DialogueGraph> dg) : m_firstTime(true)
{
	m_dialogueGraphs.emplace("english", dg);
	m_target = entity_id;

	Construct();
}

DialogueBox::DialogueBox(unsigned int entity_id, std::map<std::string, std::shared_ptr<DialogueGraph>> dg) : m_firstTime(true)
{
	m_dialogueGraphs = dg;
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
	UpdateBox();
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
	auto graph = GetLocalizedGraph();

	float x = 0.5f;
	float y = m_box->GetScale().y + m_box->GetPosRef().y - m_yScale;
	// LOCALIZATION HERE
	Font::SetText(_(text), Vector3f(x, y, 0.0f), false, m_maxWidth);
	m_choices.clear();
	if (graph && graph->ChoiceAvailable())
	{
		//m_y += m_yScale * 1.25f;
		for (auto x : graph->GetChoices())
		{
			// LOCALIZATION HERE
			x = _(x);
			m_y -= m_yScale * 1.25f;
			Font_ptr temp = Font_ptr(new Font(true));
			temp->SetScale(m_xScale, m_yScale);
			temp->SetTextSpeed(1);
			temp->SetText(x, Vector3f(0.75f, y + m_y, 0), false, m_maxWidth);
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

	// Get the right dialogue graphs
	auto graph = GetLocalizedGraph();

	for (unsigned int i = 0; i < m_choices.size(); i++)
	{
		if (graph->SelectedChoice() == i)
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
		if (InputManager::GetInstance().FrameKeyStatus(A_Accept, KeyStatus::KeyPressed, 1))
		{
			// SendInput returns false if its done
			if (graph == NULL)
			{
				m_completed = true;
				if (m_target <= ents->size())
					ents->at(m_target)->Physics()->SetConversationLock(false);
				m_owner = NULL;
				return eur;
			}

			DialogueResponse dr = graph->SendInput(IT_Action);
			eur.Queue = dr.Queue;

			if (dr.NotDone)
			{
				SetText(graph->GetCurrentText());
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
		else if (graph != NULL && graph->ChoiceAvailable() && InputManager::GetInstance().FrameKeyStatus(A_Down, KeyStatus::KeyPressed, 1))
		{
			graph->SendInput(IT_Down);
		}
		else if (graph != NULL && InputManager::GetInstance().FrameKeyStatus(A_Up, KeyStatus::KeyPressed, 1))
		{
			graph->SendInput(IT_Up);
		}
	}
	else if (InputManager::GetInstance().FrameKeyStatus(A_Accept, KeyStatus::AnyPress, 1))
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

std::shared_ptr<DialogueGraph> DialogueBox::GetLocalizedGraph()
{
	std::shared_ptr<DialogueGraph> result;

	assert(m_dialogueGraphs.size() > 0);

	std::string language = LocalizationData::Language;

	// Try to give the correct language
	if (m_dialogueGraphs.count(language))
		result = m_dialogueGraphs[language];

	// if the language isnt avaiable for this text box, try to give english
	else if (m_dialogueGraphs.count("english"))
		result = m_dialogueGraphs["english"];

	// if english isnt avaiable, just give the first one.
	else if (m_dialogueGraphs.size() > 0)
		result = (*m_dialogueGraphs.begin()).second;

	// if there is no dialogue graph, then wtf
	else
		result = std::shared_ptr<DialogueGraph>(new DialogueGraph());

	return result;
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
	if (TextDisplayDone() && InputManager::GetInstance().FrameKeyStatus(A_Accept, KeyStatus::KeyPressed, 1))
	{
		m_temporary = true;
		m_lifetime = 0;
		InputManager::GetInstance().PopLockLevel();
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
	// Get the right dialogue graphs
	auto graph = GetLocalizedGraph();

	Font::SetScale(xScale, yScale);

	m_maxWidth = (OrthoProjInfo::GetRegularInstance().Right * 2.f / OrthoProjInfo::GetRegularInstance().Size) - 1.5f/* / xScale*/;
	m_maxHeight = 4.5f;

	if (graph != NULL)
		SetText(graph->GetCurrentText());
}

void DialogueBox::ResetEvent()
{
	// Get the right dialogue graphs
	auto graph = GetLocalizedGraph();

	m_firstTime = true;
	m_completed = false;
	graph->SetToStart();
	SetText(graph->GetCurrentText());
}

std::shared_ptr<IEvent> DialogueBox::Clone()
{
	// Get the right dialogue graphs
	auto graph = GetLocalizedGraph();

	std::shared_ptr<DialogueGraph> dg = graph->Clone();
	std::shared_ptr<IEvent> result = std::shared_ptr<IEvent>(new DialogueBox(m_target, dg));
	SetCloneBaseAttributes(result);
	return result;
}
