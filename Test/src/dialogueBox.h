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
	DialogueBox(unsigned int entity_id, std::vector<Dialogue> d, std::vector<DialogueChoice> dc);
	~DialogueBox();
	void Update(double elapsedTime);
	void UpdateBox();
	void ResetEvent();
	EventUpdateResponse UpdateEvent(double elapsedTime, std::map<unsigned int, std::shared_ptr<Entity>>* ents);
	void Draw();
	void SetRender();
	void SetText(std::string text);
	void SetScale(float xScale, float yScale);

private:
	std::shared_ptr<FontGraphicsComponent> m_box;
	std::vector<std::shared_ptr<Font>> m_choices;
	float m_maxWidth;
	float m_maxHeight;
	bool m_firstTime;
	DialogueGraph* m_dialogueGraph;
};

#endif // !DIALOGUE_BOX_H__
