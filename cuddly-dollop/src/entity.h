#ifndef ENTITY_H__
#define ENTITY_H__

#include <vector>
#include <memory>
#include "graphics_component.h"
#include "physicsComponent.h"
#include "inputComponent.h"
#include "audioComponent.h"

class EventQueue;

class Entity
{
public:
	Entity(unsigned int id, std::string spritesheet, bool playerInput = false, bool fullSize = false);
	~Entity();
	void SetRender();
	void Update();
	void Draw();
	unsigned int GetID();
	void AddEventQueue(std::shared_ptr<EventQueue> eq);
	std::vector<std::shared_ptr<EventQueue>>* GetQueues();
	std::shared_ptr<PhysicsComponent>& Physics() { return m_physicsComponent; }
	PhysicsComponent* PhysicsRaw() { return m_physicsComponent.get(); }
	std::shared_ptr<GraphicsComponent> Graphics() { return m_graphicsComponent; }
	std::vector<std::shared_ptr<IComponent>> components;

public:
	bool _justTouched;

private:
	unsigned int m_id;
	bool m_hasEvents;
	bool m_fullSize;
	std::vector<std::shared_ptr<EventQueue>> m_events;
	std::shared_ptr<PhysicsComponent> m_physicsComponent;
	std::shared_ptr<GraphicsComponent> m_graphicsComponent;
	std::shared_ptr<InputComponent> m_inputComponent;
	std::shared_ptr<AudioComponent> m_audioComponent;
};

#endif