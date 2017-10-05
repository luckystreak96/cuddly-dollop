#ifndef DIALOGUE_BOX_H__
#define DIALOGUE_BOX_H__

#include "font.h"
#include "input_manager.h"
#include "iEvent.h"

class DialogueBox : public Font, public IEvent
{
public:
	DialogueBox();
	void Update(double elapsedTime);
	bool UpdateEvent(double elapsedTime);
	void Draw();
	void SetRender();
	void SetText(std::string text);

private:
	FontGraphicsComponent* m_box;
};

#endif // !DIALOGUE_BOX_H__
