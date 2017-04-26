#include "actor.h"

Actor::Actor(Vector3f position, std::string modelName, std::string texPath) : Drawable(position, modelName, texPath), m_speed(4.f)
{
	SetProperties();
}

void Actor::Update()
{
	Drawable::Update();
}

void Actor::Jump(bool released)
{
	if (!released && m_was_on_ground)
		m_velocity.z = -10.f;
	else if (released && m_velocity.z < 0)
		m_velocity.z = 0;
}

void Actor::Move(bool up, bool down, bool left, bool right)
{
	if (left)
		m_velocity.x -= m_speed;
		//m_velocity.x -= m_was_on_ground ? m_speed : SlowSpeed();
	if (right)
		m_velocity.x += m_speed;
	if (up)
		m_velocity.y += m_speed;
	if (down)
		m_velocity.y -= m_speed;
}

void Actor::DesiredMove()
{
	ApplyGravity();
	Drawable::DesiredMove();
}

void Actor::ApplyGravity()
{
	//Slow down a bit
	m_velocity.x *= /*m_was_on_ground ? */0.85f/* : 0.95f*/;
	m_velocity.y *= /*m_was_on_ground ?*/ 0.85f/* : 0.95f*/;
	//You aren't a bird
	//m_velocity.z += m_speed * 8.f * ElapsedTime::GetInstance().GetElapsedTime();

	//Don't go too fast
	m_velocity.x = m_velocity.x > m_speed ? m_speed : m_velocity.x;
	m_velocity.x = m_velocity.x < -m_speed ? -m_speed : m_velocity.x;
	m_velocity.y = m_velocity.y > m_speed ? m_speed : m_velocity.y;
	m_velocity.y = m_velocity.y < -m_speed ? -m_speed : m_velocity.y;
	m_velocity.z = m_velocity.z > m_speed * 2 ? m_speed * 2 : m_velocity.z;
	m_velocity.z = m_velocity.z < -m_speed * 2 ? -m_speed * 2 : m_velocity.z;

	//Too slow = stop
	m_velocity.x = abs(m_velocity.x) < 0.1f ? 0 : m_velocity.x;
	m_velocity.y = abs(m_velocity.y) < 0.1f ? 0 : m_velocity.y;

	//Dont leave the world >:P
	//if (m_pos.z > 3.0f && m_velocity.z >= 0)
	//	RelativePosition(Vector3f(0, 0, -10));

	//Prepare the move
	SetMovementBB();
}

Transformation Actor::GetMlMatrix()
{
	return m_modelMat;
}

void Actor::DrawShadowMap(Transformation& p)
{
	ShadowMapEffect::GetInstance().Enable();
	p.SetTranslation(Position());
	ShadowMapEffect::GetInstance().SetWorld(p.GetTrans());
	ShadowMapEffect::GetInstance().SetModel(m_modelMat.GetWorldTrans());

	Drawable::Draw();
}

void Actor::Draw()
{
	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);

	//PointLightShadowEffect::GetInstance().Enable();
	//PointLightShadowEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);

	Drawable::Draw();
}
void Actor::SetProperties()
{
	AddProp(Ent_Props::P_Player);
}