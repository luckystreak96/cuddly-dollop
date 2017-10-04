#ifndef DIALOGUE_BOX_H__
#define DIALOGUE_BOX_H__

#include "font.h"
#include "input_manager.h"

class DialogueBox : public Font
{
public:
	DialogueBox();
	void Update(double elapsedTime);
	void Draw();
	void SetRender();
	void SetText(std::string text, Vector3f location = Vector3f(0, 0, 0), bool centered = false);

private:
	FontGraphicsComponent* m_box;
};

#endif // !DIALOGUE_BOX_H__
