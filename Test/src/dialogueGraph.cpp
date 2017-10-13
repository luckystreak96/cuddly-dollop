#include "dialogueGraph.h"

DialogueGraph::DialogueGraph()
{
	m_dialogues = std::map<int, Dialogue>();
	DialogueChoice dc1 = { "Know anything cool?", 1, EventQueue() };
	EventQueue ev = EventQueue();
	ev.PushBack(new EventMove(2, 3.0f, 2));
	DialogueChoice dc2 = { "Can you walk down please?", 1, ev };
	DialogueChoice dc3 = { "I gotta go.", -1, EventQueue() };
	std::vector<DialogueChoice> vdc = std::vector<DialogueChoice>();
	vdc.push_back(dc1);
	vdc.push_back(dc2);
	vdc.push_back(dc3);
	Dialogue d0 = { 0, 1, "Hey man, what's up?", Choice, vdc };
	Dialogue d1 = { 1, 0, "Did you know that nurses and doctors wear white to make it easier to spot any bleeding? Pretty cool, huh?", Simple, std::vector<DialogueChoice>() };
	m_dialogues.emplace(d0.Id, d0);
	m_dialogues.emplace(d1.Id, d1);

	m_currentDialogue = m_dialogues.at(0).Id;
	m_selectedChoice = m_dialogues.at(m_currentDialogue).Choices.size() > 0 ? 0 : -1;
}

DialogueGraph::DialogueGraph(std::map<int, Dialogue> ds)
{
	m_dialogues = ds;
}

bool DialogueGraph::ChoiceAvailable()
{
	return m_dialogues.at(m_currentDialogue).Choices.size() > 0;
}


std::string DialogueGraph::GetCurrentText()
{
	return m_dialogues.at(m_currentDialogue).Text;
}

// Returns false if the dialogue is done
DialogueResponse DialogueGraph::SendInput(InputType it)
{
	Dialogue& d = m_dialogues.at(m_currentDialogue);

	DialogueResponse result = DialogueResponse();
	m_selectedChoice != -1 ? result.Queue = d.Choices.at(m_selectedChoice).Queue : EventQueue();
	result.NotDone = false;

	unsigned int length = d.Choices.size();

	if (it == IT_Action)
	{
		// The dialogue is over
		if (d.Type == End || d.NextTextId == -1)
			return result;

		if (d.Choices.size() > 0)
		{
			// If theres no future text, end the conversation
			if (d.Choices.at(m_selectedChoice).NextTextId == -1)
				return result;

			// Set the next dialogue
			SetNextDialogue(d.Choices.at(m_selectedChoice).NextTextId);
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
		if (m_selectedChoice != -1 && m_selectedChoice + 1 < d.Choices.size())
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

std::vector<std::string> DialogueGraph::GetChoices()
{
	std::vector<std::string> result = std::vector<std::string>();
	for (auto x : m_dialogues.at(m_currentDialogue).Choices)
	{
		result.push_back(x.Text);
	}
	return result;
}
