#ifndef DIALOGUE_BOX_H__
#define DIALOGUE_BOX_H__

#include "font.h"
#include "input_manager.h"
#include "iEvent.h"
#include "dialogueGraph.h"
#include "eventQueue.h"

class DialogueBox : public Font, public IEvent
{
public:
	DialogueBox(unsigned int entity_id, DialogueGraph* dg = NULL);
	DialogueBox(unsigned int entity_id, std::vector<Dialogue> d, std::vector<DialogueChoice> dc);
	~DialogueBox();
	void Update(double elapsedTime);
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, Entity*>* ents);
	void Draw();
	void SetRender();
	void SetText(std::string text);
	void SetScale(float xScale, float yScale);

private:
	FontGraphicsComponent* m_box;
	std::vector<Font*> m_choices;
	float m_maxWidth;
	float m_maxHeight;
	DialogueGraph* m_dialogueGraph;
};

#endif // !DIALOGUE_BOX_H__
