#include "dialogueBox.h"

DialogueBox::DialogueBox() : m_box(NULL), Font()
{
	m_box = new FontGraphicsComponent("DIALOGUE_BOX", "res/dialogue.png");
	m_box->SetPhysics(Vector3f(0, 0, 0.5f), Vector3f(0, 0, 0));
	m_box->Update();
	m_box->SetStatic(true);

	m_phys.SetPosition(Vector3f());
	m_static = true;
	m_lockLevel = 1;
	m_mode = BLOCKING;
	InputManager::GetInstance().SetLockLevel(1);
}

void DialogueBox::Draw()
{
	m_box->Draw();
	m_graphics->Draw();
}

void DialogueBox::SetText(std::string text)
{
	Font::SetText(text, Vector3f(0.5f, 4.0f, 0.0f), false, m_maxWidth);
}

bool DialogueBox::UpdateEvent(double elapsedTime)
{
	// Don't update it if its completed
	if (m_completed)
		return true;

	// Update the font
	Font::Update(elapsedTime);

	// When you press space, set up the textbox to be destroyed
	if (TextDisplayDone() && InputManager::GetInstance().FrameKeyStatus(' ', KeyStatus::KeyPressed, 1))
	{
		m_completed = true;
		return true;
	}
	else if (InputManager::GetInstance().FrameKeyStatus(' ', KeyStatus::AnyPress, 1))
	{
 		m_textSpeed = 7;
	}
	else
	{
		m_textSpeed = 1;
	}

	SetRender();

	// The dialogue is not done
	return false;
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

void DialogueBox::SetScale(float xScale, float yScale)
{
	Font::SetScale(xScale, yScale);

	m_maxWidth = 29.0f/* / xScale*/;
	m_maxHeight = 4.0f/* / yScale*/;
}
