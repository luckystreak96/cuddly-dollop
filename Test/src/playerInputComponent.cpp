#include "playerInputComponent.h"

PlayerInputComponent::PlayerInputComponent(PhysicsComponent* phys, GraphicsComponent* graph)
{
	PhysicsComponent* m_phys = phys;
	GraphicsComponent* m_graph = graph;
}

void PlayerInputComponent::Update()
{
	std::list<std::pair<unsigned int, KeyStatus>>* keys = InputManager::GetInstance().GetKeys();
	std::map<unsigned int, bool>* heldKeys = InputManager::GetInstance().GetHeldKeys();

	std::map<std::pair<unsigned int, KeyStatus>, bool> keyMap = std::map<std::pair<unsigned int, KeyStatus>, bool>();
	for (auto a : *keys)
		keyMap.emplace(a, false);

	if(heldKeys->count(GLUT_KEY_UP + InputManager::SpecialKeyValue))
		m_phys.
	//if(InputManager::GetInstance().FrameKeyStatus(GLUT_KEY_UP + InputManager::SpecialKeyValue, ))
}