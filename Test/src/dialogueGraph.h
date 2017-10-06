#ifndef DIALOGUE_GRAPH_H__
#define DIALOGUE_GRAPH_H__

#include <string>
#include <vector>
#include "iEvent.h"

enum DialogueType { Simple, Choice };

struct DialogueChoice
{
	std::string Text;
	unsigned int NextTextId;
	IEvent* Event;
};

struct Dialogue
{
	unsigned int TextId;
	std::string Text;
	DialogueType Type;
	std::vector<DialogueChoice> Choices;
};

class DialogueGraph
{
public:
	DialogueGraph(std::vector<Dialogue> ds);
	bool GetCurrentDialogue();
private:
	std::vector<Dialogue> m_dialogues;
	unsigned int m_currentDialogue;
};

#endif // !DIALOGUE_GRAPH_H__

