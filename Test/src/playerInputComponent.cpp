#include "playerInputComponent.h"
#include <GLFW\glfw3.h>

PlayerInputComponent::PlayerInputComponent(std::shared_ptr<PhysicsComponent> phys, std::shared_ptr<GraphicsComponent> graph)
{
	m_phys = phys;
	m_graph = graph;
}

//void PlayerInputComponent::Interact(std::shared_ptr<Entity> interactor = NULL)
//{
//
//}


void PlayerInputComponent::Update()
{
	//std::list<std::pair<unsigned int, KeyStatus>>* keys = InputManager::GetInstance().GetKeys();
	//std::map<unsigned int, bool>* heldKeys = InputManager::GetInstance().GetHeldKeys();

	//std::map<std::pair<unsigned int, KeyStatus>, bool> keyMap = std::map<std::pair<unsigned int, KeyStatus>, bool>();
	//for (auto a : *keys)
	//	keyMap.emplace(a, false);

	bool up = InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_UP) || InputManager::GetInstance().FrameKeyStatus('W');
	bool down = InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_DOWN) || InputManager::GetInstance().FrameKeyStatus('S');
	bool left = InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_LEFT) || InputManager::GetInstance().FrameKeyStatus('A');
	bool right = InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_RIGHT) || InputManager::GetInstance().FrameKeyStatus('D');

	m_phys->ActionMove(up, down, left, right);
	//if(InputManager::GetInstance().FrameKeyStatus(GLUT_KEY_UP + InputManager::SpecialKeyValue, ))
}