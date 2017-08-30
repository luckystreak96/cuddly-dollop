#include "entity.h"

Entity::Entity(bool playerInput)
{
	m_graphicsComponent = new GraphicsComponent();
	components.push_back(m_graphicsComponent);

	m_physicsComponent = new PlayerPhysicsComponent(Vector3f(), "TILE", Vector3f(0.9f, 0.4f, -0.45f), Vector3f(0, 0, 1));
	components.push_back(m_physicsComponent);

	m_inputComponent = playerInput ? new PlayerInputComponent(m_physicsComponent, m_graphicsComponent) : new InputComponent();
	components.push_back(m_inputComponent);
}

void Entity::Update()
{
	m_physicsComponent->Update();

	m_graphicsComponent->SetPosition(m_physicsComponent->Position());
	m_graphicsComponent->Update();
	m_inputComponent->Update();
}

void Entity::Draw()
{
	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_graphicsComponent->GetModelMat()->GetWorldTrans().m[0][0]);
	m_graphicsComponent->Draw();
}