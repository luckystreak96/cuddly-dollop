#include "entity.h"
#include "define.h"
#include "eventQueue.h"
#include "renderer.h"
#include "playerPhysicsComponent.h"
#include "playerGraphicsComponent.h"
#include "playerInputComponent.h"
#include "physicsComponent.h"
#include "graphicsComponent.h"

Entity::Entity(unsigned int id, std::string spritesheet, bool playerInput, bool fullSize)
	: m_id(id), m_hasEvents(false), m_fullSize(fullSize), _justTouched(false), m_graphicsComponent(nullptr)
{
	if (ResourceManager::GetInstance().LoadTexture(spritesheet))
	{
		m_graphicsComponent = !playerInput ? GraphComp_ptr(new PlayerGraphicsComponent(spritesheet, "CENTERED_TILE")) :
			GraphComp_ptr(new PlayerGraphicsComponent(spritesheet, "CENTERED_TILE"));
		m_graphicsComponent->_id = id;
		components.push_back(m_graphicsComponent);
	}

    float size = OrthoProjInfo::GetRegularInstance().Size;

	m_physicsComponent = m_fullSize ? Physics_ptr(new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.95f * size, 0.95f * size, 0), Vector3f(1, 0, 0))) :
		Physics_ptr(new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.8125f * size, 0.40625f * size, 0), Vector3f(1, 0, 0)));
	m_physicsComponent->_id = id;
	//m_physicsComponent->_unmoving = false;
	components.push_back(m_physicsComponent);

	m_inputComponent = playerInput ? std::shared_ptr<InputComponent>(new PlayerInputComponent(m_physicsComponent)) :
		std::shared_ptr<InputComponent>(new InputComponent());
	m_inputComponent->_id = id;
	components.push_back(m_inputComponent);

	InputManager::GetInstance().AddObserver(m_inputComponent);
}

Entity::~Entity()
{

}

void Entity::AddEventQueue(std::shared_ptr<EventQueue> eq)
{
	m_events.push_back(eq);
	m_hasEvents = true;
}

std::vector<std::shared_ptr<EventQueue>>* Entity::GetQueues()
{
	return &m_events;
}

void Entity::Update()
{
	m_inputComponent->Update();

	m_physicsComponent->Update();

	if (m_graphicsComponent)
	{
		m_graphicsComponent->SetPhysics(m_physicsComponent->Position(), m_physicsComponent->Velocity());
		m_graphicsComponent->Update();
	}
}

void Entity::Draw()
{
	if (m_graphicsComponent)
	{
		Effect::SetModelPosition(&m_graphicsComponent->GetModelMat()->GetWorldTrans().m[0][0]);
		m_graphicsComponent->Draw();
	}
}

void Entity::SetRender()
{
	if (m_graphicsComponent)
		Renderer::GetInstance().Add(m_graphicsComponent);
}

unsigned int Entity::GetID()
{
	return m_id;
}

