#include "entity.h"

Entity::Entity()
{
	m_graphicsComponent = new GraphicsComponent();
	components.push_back(m_graphicsComponent);

	m_physicsComponent = new PlayerPhysicsComponent();
	components.push_back(m_physicsComponent);

	m_inputComponent = new PlayerInputComponent(m_physicsComponent, m_graphicsComponent);
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