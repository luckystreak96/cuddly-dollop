#ifndef PLAYER_INPUT_COMPONENT
#define PLAYER_INPUT_COMPONENT

#include "inputComponent.h"
#include "physicsComponent.h"
#include "graphicsComponent.h"
#include "input_manager.h"

class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent(PhysicsComponent* phys, GraphicsComponent* graph);
	void Update();
private:
	PhysicsComponent* m_phys;
	GraphicsComponent* m_graph;
};

#endif // !PLAYER_INPUT_COMPONENT
