#include "playerInputComponent.h"
#include <GLFW/glfw3.h>

PlayerInputComponent::PlayerInputComponent(std::shared_ptr<PhysicsComponent> phys, GraphComp_ptr graph)
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

	bool up = InputManager::GetInstance().FrameKeyStatus(A_Up);
	bool down = InputManager::GetInstance().FrameKeyStatus(A_Down);
	bool left = InputManager::GetInstance().FrameKeyStatus(A_Left);
	bool right = InputManager::GetInstance().FrameKeyStatus(A_Right);

	m_phys->ActionMove(up, down, left, right, InputManager::GetInstance().GetKeyPercent(A_Left), InputManager::GetInstance().GetKeyPercent(A_Down));
}
