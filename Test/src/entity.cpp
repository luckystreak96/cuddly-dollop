#include "entity.h"

Entity::Entity(bool playerInput)
{
	m_graphicsComponent = /*!playerInput ? new GraphicsComponent() :*/ new PlayerGraphicsComponent();
	components.push_back(m_graphicsComponent);

	m_physicsComponent = new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.9f, 0.4f, -0.45f), Vector3f(1, 0, 0));
	components.push_back(m_physicsComponent);

	m_inputComponent = playerInput ? new PlayerInputComponent(m_physicsComponent, m_graphicsComponent) : new InputComponent();
	components.push_back(m_inputComponent);
}

void Entity::Communicate(std::vector<std::string> msg)
{
	for (auto x : components)
		x->ReceiveMessage(msg);
}


void Entity::Update()
{
	m_physicsComponent->Update();

	m_inputComponent->Update();

	m_graphicsComponent->SetPhysics(m_physicsComponent->Position(), m_physicsComponent->Velocity());
	m_graphicsComponent->Update();
}

void Entity::Draw()
{
	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_graphicsComponent->GetModelMat()->GetWorldTrans().m[0][0]);
	m_graphicsComponent->Draw();
}