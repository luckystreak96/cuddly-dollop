#include "entity.h"
#include "define.h"

Entity::Entity(unsigned int id, std::string spritesheet, bool playerInput, bool fullSize) : m_id(id), m_hasEvents(false), m_fullSize(fullSize)
{
	m_graphicsComponent = !playerInput ? std::shared_ptr<GraphicsComponent>(new PlayerGraphicsComponent(spritesheet, "TILE")) : 
		std::shared_ptr<GraphicsComponent>(new PlayerGraphicsComponent(spritesheet, "TILE"));
	components.push_back(m_graphicsComponent);

	//m_physicsComponent = playerInput ? new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.9f, 0.4f, -0.45f), Vector3f(1.0f, 0, 0)) : new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.9f, 0.4f, -0.45f), Vector3f(1.0f, 0, 0));
	m_physicsComponent = m_fullSize ? std::shared_ptr<PhysicsComponent>(new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.95f, 0.95f, 0), Vector3f(1, 0, 0))) : std::shared_ptr<PhysicsComponent>(new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.8f, 0.4f, 0), Vector3f(1, 0, 0)));
	components.push_back(m_physicsComponent);

	m_inputComponent = playerInput ? std::shared_ptr<InputComponent>(new PlayerInputComponent(m_physicsComponent, m_graphicsComponent)) : 
		std::shared_ptr<InputComponent>(new InputComponent());
	components.push_back(m_inputComponent);

	//if (!playerInput)
	//{
	//	AudioComponent* audio = new AudioComponent();
	//	components.push_back(audio);
	//}
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

void Entity::Communicate(std::vector<std::string> msg)
{
	for (auto x : components)
		x->ReceiveMessage(msg);
}


void Entity::Update()
{
	m_physicsComponent->Update();
	std::vector<std::string> vec = std::vector<std::string>() = { "SET_POSITION", std::to_string(m_physicsComponent->Position().x), std::to_string(m_physicsComponent->Position().y), std::to_string(m_physicsComponent->Position().z) };
	for (auto x : components)
		x->ReceiveMessage(vec);

	m_inputComponent->Update();

	m_graphicsComponent->SetPhysics(m_physicsComponent->Position(), m_physicsComponent->Velocity());
	m_graphicsComponent->Update();
}

void Entity::Draw()
{
	Effect::SetModelPosition(&m_graphicsComponent->GetModelMat()->GetWorldTrans().m[0][0]);
	m_graphicsComponent->Draw();
}

void Entity::SetRender()
{
	Renderer::GetInstance().Add(m_graphicsComponent);
}

unsigned int Entity::GetID()
{
	return m_id;
}

