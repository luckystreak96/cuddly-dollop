#ifndef DIALOGUE_GRAPH_H__
#define DIALOGUE_GRAPH_H__

#include <string>
#include <vector>
#include <map>
#include "iEvent.h"
#include "eventQueue.h"
#include "eventMove.h"

enum DialogueType { Simple, Choice, End };
enum InputType { IT_Action, IT_Up, IT_Down };

struct DialogueResponse
{
	bool NotDone;
	std::shared_ptr<EventQueue> Queue;
};

struct DialogueChoice
{
	int DialogueId;
	std::string Text;
	int NextTextId;
	std::shared_ptr<EventQueue> Queue;
	DialogueChoice();
};

struct Dialogue
{
	int Id;
	int NextTextId;
	std::string Text;
	DialogueType Type;
	std::shared_ptr<EventQueue> Queue;
	Dialogue();
};

class DialogueGraph
{
public:
	DialogueGraph(bool DONT_USE_THIS = true);
	DialogueGraph(std::vector<Dialogue> d, std::vector<DialogueChoice> dc);
	~DialogueGraph();
	void SetToStart();
	std::string GetCurrentText();
	DialogueResponse SendInput(InputType it);
	bool ChoiceAvailable();
	int SelectedChoice();
	std::vector<std::string> GetChoices();
	std::vector<DialogueChoice> GetDialogueChoices(int current_dialogue_id);
	static DialogueType StringToDialogueType(std::string s);
	std::shared_ptr<DialogueGraph> Clone();
	//std::vector<std::string> GetChoices(int dialogueId);

public:
	std::vector<Dialogue> Dialogues;
	std::vector<DialogueChoice> DialogueChoices;

private:
	void SetNextDialogue(int id);
private:
	int m_selectedChoice;
	std::map<int, Dialogue> m_dialogues;
	std::vector<DialogueChoice> m_choices;
	int m_currentDialogue;
};

#endif // !DIALOGUE_GRAPH_H__

