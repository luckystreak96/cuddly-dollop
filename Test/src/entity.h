#ifndef ENTITY_H__
#define ENTITY_H__

#include <vector>
#include "define.h"
#include "iComponent.h"
#include "physicsComponent.h"
#include "graphicsComponent.h"
#include "inputComponent.h"

class Entity
{
public:
	Entity();
	void Update();
	void Draw();
	PhysicsComponent* Physics() { return m_physicsComponent; }
	GraphicsComponent* Graphics() { return m_graphicsComponent; }
	InputComponent* Input() { return m_inputComponent; }
	std::vector<IComponent*> components;
private:
	PhysicsComponent* m_physicsComponent;
	GraphicsComponent* m_graphicsComponent;
	InputComponent* m_inputComponent;
};

#endif