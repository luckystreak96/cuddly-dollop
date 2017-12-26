#include "dialogueGraph.h"

DialogueGraph::DialogueGraph()
{
	std::shared_ptr<EventQueue> ev = std::shared_ptr<EventQueue>(new EventQueue(2));

	DialogueChoice dc1 = { 0, "Know anything cool?", 1, std::shared_ptr<EventQueue>(new EventQueue()) };

	m_choices.push_back(DialogueChoice{ 0, "Know anything cool?", 1, std::shared_ptr<EventQueue>(new EventQueue()) });

	std::shared_ptr<IEvent> down = std::shared_ptr<IEvent>(new EventMove(2, 3.0, 2));
	std::shared_ptr<IEvent> up = std::shared_ptr<IEvent>(new EventMove(2, 3.0, 0));
	down->SetExecutionMode(EventExecutionMode::BLOCKING);
	up->SetExecutionMode(EventExecutionMode::BLOCKING);

	ev->SetRepeating(true);
	ev->PushBack(down);
	ev->PushBack(up);

	DialogueChoice dc2 = { 0, "Can you walk down please?", 2, ev };
	DialogueChoice dc3 = { 0, "I gotta go.", -1, std::shared_ptr<EventQueue>(new EventQueue()) };

	m_choices.push_back(DialogueChoice{ 0, "Can you walk down please?", 2, ev });
	m_choices.push_back(DialogueChoice{ 0, "I gotta go.", -1, std::shared_ptr<EventQueue>(new EventQueue()) });

	std::vector<DialogueChoice> vdc = std::vector<DialogueChoice>();
	vdc.push_back(dc1);
	vdc.push_back(dc2);
	vdc.push_back(dc3);

	Dialogue d0 = { 0, 1, "Hey man, what's up?", Choice };
	Dialogue d1 = { 1, 0, "Nope! Sorry!", Simple };
	Dialogue d2 = { 2, 0, "Sure man, just move away after we're done talking!", Simple };
	m_dialogues.emplace(d0.Id, d0);
	m_dialogues.emplace(d1.Id, d1);
	m_dialogues.emplace(d2.Id, d2);

	m_currentDialogue = m_dialogues.at(0).Id;
	m_selectedChoice = m_dialogues.at(m_currentDialogue).Type == Choice ? 0 : -1;
}

DialogueGraph::DialogueGraph(std::vector<Dialogue> d, std::vector<DialogueChoice> dc)
{
	m_choices = dc;
	for (auto x : d)
		m_dialogues.emplace(x.Id, x);

	m_currentDialogue = m_dialogues.at(0).Id;
	m_selectedChoice = m_dialogues.at(m_currentDialogue).Type == Choice ? 0 : -1;
}

DialogueGraph::DialogueGraph(std::map<int, Dialogue> ds)
{
	m_dialogues = ds;
}

DialogueGraph::~DialogueGraph()
{

}


std::vector<std::string> DialogueGraph::GetChoices()
{
	std::vector<std::string> result = std::vector<std::string>();
	for (auto x : m_choices)
	{
		if (x.DialogueId == m_currentDialogue)
			result.push_back(x.Text);
	}
	return result;
}

std::vector<DialogueChoice> DialogueGraph::GetDialogueChoices(int current_dialogue_id)
{
	std::vector<DialogueChoice> result = std::vector<DialogueChoice>();
	for (auto x : m_choices)
	{
		if (x.DialogueId == current_dialogue_id)
			result.push_back(x);
	}
	return result;
}

bool DialogueGraph::ChoiceAvailable()
{
	return m_dialogues.at(m_currentDialogue).Type == Choice;
}


std::string DialogueGraph::GetCurrentText()
{
	return m_dialogues.at(m_currentDialogue).Text;
}

// Returns false if the dialogue is done
DialogueResponse DialogueGraph::SendInput(InputType it)
{
	Dialogue& d = m_dialogues.at(m_currentDialogue);

	std::vector<DialogueChoice> choices = GetDialogueChoices(m_currentDialogue);

	DialogueResponse result = DialogueResponse();

	// Set result queue
	if (m_selectedChoice == -1)
		result.Queue = d.Queue;//std::shared_ptr<EventQueue>(new EventQueue());
	else
		result.Queue = choices.at(m_selectedChoice).Queue;

	result.NotDone = false;

	unsigned int length = choices.size();

	if (it == IT_Action)
	{
		// The dialogue is over
		if (d.Type == End || d.NextTextId == -1)
			return result;

		if (choices.size() > 0)
		{
			// If theres no future text, end the conversation
			if (choices.at(m_selectedChoice).NextTextId == -1)
				return result;

			// Set the next dialogue
			SetNextDialogue(choices.at(m_selectedChoice).NextTextId);
		}
		else if (d.Type == Simple)
		{
			SetNextDialogue(d.NextTextId);
		}
	}
	else if (it == IT_Up)
	{
		if (m_selectedChoice > 0)
			m_selectedChoice--;
	}
	else if (it == IT_Down)
	{
		if (m_selectedChoice != -1 && m_selectedChoice + 1 < (int)choices.size())
			m_selectedChoice++;
	}

	result.NotDone = true;
	return result;
}

void DialogueGraph::SetNextDialogue(int id)
{
	m_currentDialogue = id;
	if (m_dialogues.at(id).Type == Choice)
		m_selectedChoice = 0;
	else
		m_selectedChoice = -1;
}

int DialogueGraph::SelectedChoice()
{
	return m_selectedChoice;
}

DialogueType DialogueGraph::StringToDialogueType(std::string s)
{
	switch (s[0])
	{
	case 's':
		return Simple;
		break;
	case 'e':
		return End;
		break;
	case 'c':
		return Choice;
		break;
	default:
		return Simple;
		break;
	}
}

void DialogueGraph::SetToStart()
{
	for (auto x : m_dialogues)
		if (x.first == 0)
			SetNextDialogue(0);
}



//std::vector<std::string> DialogueGraph::GetChoices()
//{
//	std::vector<std::string> result = std::vector<std::string>();
//	for (auto x : m_dialogues.at(m_currentDialogue).Choices)
//	{
//		result.push_back(x.Text);
//	}
//	return result;
//}
