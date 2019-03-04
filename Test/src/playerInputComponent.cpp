#include "playerInputComponent.h"
#include <GLFW/glfw3.h>

PlayerInputComponent::PlayerInputComponent(std::shared_ptr<PhysicsComponent> phys)
{
	m_phys = phys;
	
	//InputManager::GetInstance().AddObserver(Obs_ptr(this));
}

PlayerInputComponent::~PlayerInputComponent()
{
}

void PlayerInputComponent::Update()
{
	//std::list<std::pair<unsigned int, KeyStatus>>* keys = InputManager::GetInstance().GetKeys();
	//std::map<unsigned int, bool>* heldKeys = InputManager::GetInstance().GetHeldKeys();

	//std::map<std::pair<unsigned int, KeyStatus>, bool> keyMap = std::map<std::pair<unsigned int, KeyStatus>, bool>();
	//for (auto a : *keys)
	//	keyMap.emplace(a, false);

	if (false) {
		bool up = InputManager::GetInstance().FrameKeyStatus(A_Up);
		bool down = InputManager::GetInstance().FrameKeyStatus(A_Down);
		bool left = InputManager::GetInstance().FrameKeyStatus(A_Left);
		bool right = InputManager::GetInstance().FrameKeyStatus(A_Right);

		m_phys->ActionMove(up, down, left, right, InputManager::GetInstance().GetKeyPercent(A_Left), InputManager::GetInstance().GetKeyPercent(A_Down));
	}
}

void PlayerInputComponent::Update(Observable * obs)
{
	//InputManager* man = (InputManager*)obs;
	//bool up = man->FrameKeyStatus(A_Up);
	//bool down = man->FrameKeyStatus(A_Down);
	//bool left = man->FrameKeyStatus(A_Left);
	//bool right = man->FrameKeyStatus(A_Right);

	//m_phys->ActionMove(up, down, left, right, InputManager::GetInstance().GetKeyPercent(A_Left), InputManager::GetInstance().GetKeyPercent(A_Down));
	m_phys->handle_input(obs);
}
