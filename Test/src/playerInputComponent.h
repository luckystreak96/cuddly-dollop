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
	PlayerInputComponent(std::shared_ptr<PhysicsComponent> phys, std::shared_ptr<GraphicsComponent> graph);
	~PlayerInputComponent();
	void Update();
	void Update(Observable* obs);
	//void Interact(std::shared_ptr<Entity> interactor = NULL);
private:
	std::shared_ptr<PhysicsComponent> m_phys;
	std::shared_ptr<GraphicsComponent> m_graph;
};

#endif // !PLAYER_INPUT_COMPONENT
