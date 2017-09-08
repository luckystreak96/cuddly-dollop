#include "entity.h"

Entity::Entity(bool playerInput)
{
	m_graphicsComponent = /*!playerInput ? new GraphicsComponent() :*/ new PlayerGraphicsComponent();
	components.push_back(m_graphicsComponent);

	m_physicsComponent = new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.9f, 0.4f, -0.45f), Vector3f(1, 0, 0));
	components.push_back(m_physicsComponent);

	m_inputComponent = playerInput ? new PlayerInputComponent(m_physicsComponent, m_graphicsComponent) : new InputComponent();
	components.push_back(m_inputComponent);

	if (!playerInput)
	{
		AudioComponent* audio = new AudioComponent();
		components.push_back(audio);
	}
}

Entity::~Entity()
{
	for (auto x : components)
	{
		delete x;
	}
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