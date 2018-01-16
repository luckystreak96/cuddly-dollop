#ifndef PLAYER_INPUT_COMPONENT
#define PLAYER_INPUT_COMPONENT

#include "inputComponent.h"
#include "physicsComponent.h"
#include "graphicsComponent.h"
#include "input_manager.h"

class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent(std::shared_ptr<PhysicsComponent> phys, GraphComp_ptr graph);
	void Update();
	//void Interact(std::shared_ptr<Entity> interactor = NULL);
private:
	std::shared_ptr<PhysicsComponent> m_phys;
	GraphComp_ptr m_graph;
};

#endif // !PLAYER_INPUT_COMPONENT
