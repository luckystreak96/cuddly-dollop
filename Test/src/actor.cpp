#include "actor.h"

Actor::Actor(Vector3f position, std::string modelName, std::string texPath) : Drawable(position, modelName, texPath), m_speed(4.f)
{
	m_modelMat.SetScale(1, 1, 1);
	m_size = Vector3f(0.9f, 0.4f, -0.45f);
	m_normalSize = Vector3f(1, 0, 0);
	SetBoundingBox();
	SetProperties();
}

void Actor::Update()
{
	int dir = m_direction;
	//Change the direction hes facing
	if (m_velocity != 0)
	{
		if (abs(m_velocity.x) > abs(m_velocity.y))
		{
			if (m_velocity.x > 0)
				m_direction = Direction::dir_Right;
			else
				m_direction = Direction::dir_Left;
		}
		else
		{
			if (m_velocity.y > 0)
				m_direction = Direction::dir_Up;
			else
				m_direction = Direction::dir_Down;
		}
	}

	//use the backup to reset the texture coordinates for proper analysis from the animator
	m_vertices = std::vector<Vertex>(m_originalVertices);

	//Change the sprite depending on direction
	m_animation = m_direction;

	if (SetTileModelTC(&m_vertices, dir != m_direction))
		ResetVBO();
	Drawable::Update();

	m_models = std::vector<Vector3f>();
	m_models.insert(m_models.end(), 4, m_pos);
}

void Actor::Jump(bool released)
{
	if (!released)
		m_speed = 8;
	else
		m_speed = 4;
}

void Actor::Move(bool up, bool down, bool left, bool right)
{
	if (left)
		m_velocity.x -= m_speed;
	if (right)
		m_velocity.x += m_speed;
	if (up)
		m_velocity.y += m_speed;
	if (down)
		m_velocity.y -= m_speed;

	float hor = abs(m_velocity.x);
	float ver = abs(m_velocity.y);

}

void Actor::DesiredMove()
{
	ApplyGravity();
	Drawable::DesiredMove();
}

void Actor::ApplyGravity()
{
	//Slow down a bit
	m_velocity.x *= /*m_was_on_ground ? */0.6f/* : 0.95f*/;
	m_velocity.y *= /*m_was_on_ground ?*/ 0.6f/* : 0.95f*/;
	//You aren't a bird
	//m_velocity.z += m_speed * 8.f * ElapsedTime::GetInstance().GetElapsedTime();

	//Don't go too fast
	//m_velocity.x = m_velocity.x > m_speed ? m_speed : m_velocity.x;
	//m_velocity.x = m_velocity.x < -m_speed ? -m_speed : m_velocity.x;
	//m_velocity.y = m_velocity.y > m_speed ? m_speed : m_velocity.y;
	//m_velocity.y = m_velocity.y < -m_speed ? -m_speed : m_velocity.y;
	//m_velocity.z = m_velocity.z > m_speed * 2 ? m_speed * 2 : m_velocity.z;
	//m_velocity.z = m_velocity.z < -m_speed * 2 ? -m_speed * 2 : m_velocity.z;

	//Too slow = stop
	m_velocity.x = abs(m_velocity.x) < 0.1f ? 0 : m_velocity.x;
	m_velocity.y = abs(m_velocity.y) < 0.1f ? 0 : m_velocity.y;

	//Dont leave the world >:P
	//if (m_pos.z > 3.0f && m_velocity.z >= 0)
	//	RelativePosition(Vector3f(0, 0, -10));

	//Prepare the move
	SetMovedBB();
}

Transformation Actor::GetMlMatrix()
{
	return m_modelMat;
}

void Actor::DrawShadowMap(Transformation& p)
{

	Drawable::Draw();
}

void Actor::Draw()
{
	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);
	Drawable::Draw();
}


void Actor::SetProperties()
{
	AddProp(Ent_Props::P_Player);
}