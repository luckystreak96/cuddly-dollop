#include "playerGraphicsComponent.h"

PlayerGraphicsComponent::PlayerGraphicsComponent(std::string tex, std::string model) : m_firstLoad(true)
{
	m_modelName = model;
	m_texture = tex;
	Construct();
	Update();
	m_firstLoad = false;
}

void PlayerGraphicsComponent::Update()
{
	int dir = m_direction;
	//Change the direction hes facing
	if (m_vel != 0)
	{
		if (abs(m_vel.x) > abs(m_vel.y))
		{
			if (m_vel.x > 0)
				m_direction = dir_Right;
			else
				m_direction = dir_Left;
		}
		else
		{
			if (m_vel.y > 0)
				m_direction = dir_Up;
			else
				m_direction = dir_Down;
		}
	}

	//use the backup to reset the texture coordinates for proper analysis from the animator
	m_vertices = std::vector<Vertex>(m_originalVertices);

	//Change the sprite depending on direction
	//m_animation = m_direction;

	if (SetTileModelTC(&m_vertices, m_direction, dir != m_direction || m_direction != m_lastInteraction || m_firstLoad))
		ResetVBO();

	GraphicsComponent::Update();
}