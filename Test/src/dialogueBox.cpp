#include "dialogueBox.h"

DialogueBox::DialogueBox() : m_box(NULL), Font()
{
	m_box = new FontGraphicsComponent("DIALOGUE_BOX", "res/dialogue.png");
	m_box->SetPhysics(Vector3f(0, 0, 0.5f), Vector3f(0, 0, 0));
	m_box->Update();
	m_box->SetStatic(true);

	m_phys.SetPosition(Vector3f());
	m_static = true;
	InputManager::GetInstance().SetLockLevel(1);
}

void DialogueBox::Draw()
{
	m_box->Draw();
	m_graphics->Draw();
}

void DialogueBox::SetText(std::string text, Vector3f location, bool centered)
{
	Font::SetText(text, Vector3f(0.5f, 4.0f, 0.0f), false);
}

void DialogueBox::Update(double elapsedTime)
{
	Font::Update(elapsedTime);

	// When you press space, set up the textbox to be destroyed next frame
	if (TextDisplayDone() && InputManager::GetInstance().FrameKeyStatus(' ', KeyStatus::KeyPressed, 1))
	{
		m_temporary = true;
		m_lifetime = 0;
		InputManager::GetInstance().SetLockLevel(0);
	}
}

void DialogueBox::SetRender()
{
	if (m_box != NULL)
		Renderer::GetInstance().Add(m_box);
	if (m_graphics != NULL)
		Renderer::GetInstance().Add(m_graphics);
}