#ifndef PLAYER_INPUT_COMPONENT
#define PLAYER_INPUT_COMPONENT

#include "inputComponent.h"
#include "physicsComponent.h"
#include "graphicsComponent.h"
#include "input_manager.h"
#include "observer.h"

class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent(std::shared_ptr<PhysicsComponent> phys);
	~PlayerInputComponent();
	void Update();
	void Update(Observable* obs);
	//void Interact(std::shared_ptr<Entity> interactor = NULL);
private:
	std::shared_ptr<PhysicsComponent> m_phys;
};

#endif // !PLAYER_INPUT_COMPONENT
