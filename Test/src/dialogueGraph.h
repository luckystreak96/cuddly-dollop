#ifndef DIALOGUE_GRAPH_H__
#define DIALOGUE_GRAPH_H__

#include <string>
#include <vector>
#include <map>
#include "iEvent.h"

enum DialogueType { Simple, Choice, End };
enum InputType { IT_Action, IT_Up, IT_Down };

struct DialogueChoice
{
	std::string Text;
	int NextTextId;
	IEvent* Event;
};

struct Dialogue
{
	int Id;
	int NextTextId;
	std::string Text;
	DialogueType Type;
	std::vector<DialogueChoice> Choices;
};

class DialogueGraph
{
public:
	DialogueGraph();
	DialogueGraph(std::map<int, Dialogue> ds);
	std::string GetCurrentText();
	bool SendInput(InputType it);
	bool ChoiceAvailable();
	int SelectedChoice();
	std::vector<std::string> GetChoices();
private:
	void SetNextDialogue(int id);
private:
	int m_selectedChoice;
	std::map<int, Dialogue> m_dialogues;
	int m_currentDialogue;
};

#endif // !DIALOGUE_GRAPH_H__

