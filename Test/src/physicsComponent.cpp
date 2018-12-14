#include "physicsComponent.h"

#include "model.h"
#include "elapsedTime.h"

PhysicsComponent::PhysicsComponent() : m_conversationLock(false), walkOn(true), _collided_last_frame(-1), _unmoving(false)
{
	m_velocity = Vector3f();
}


PhysicsComponent::PhysicsComponent(Vector3f pos, std::string modelName, Vector3f size, Vector3f numTiles) : PhysicsComponent()
{
	m_size = (size);
	m_BBcenter = (numTiles);
	m_pos = pos;

	SetDefaults(modelName);
	Update();
}

void PhysicsComponent::Update()
{
	if (m_velocity != 0)
		PhysicsComponent::Move();

	SetMovedBB();
}

void PhysicsComponent::apply_natural_deceleration()
{
	//Slow down a bit

	// X
	if (m_velocity.x < 0)
		m_velocity.x += 0.1f;
	else
		m_velocity.x -= 0.1f;

	// Y
	if (m_velocity.y < 0)
		m_velocity.y += 0.1f;
	else
		m_velocity.y -= 0.1f;

	m_velocity.x *= 0.6f;
	m_velocity.y *= 0.6f;

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

	//Prepare the move
	SetMovedBB();
}

void PhysicsComponent::XCollide()
{
	m_velocity.x = 0;
}

void PhysicsComponent::YCollide()
{
	m_velocity.y = 0;
}

void PhysicsComponent::Move()
{
	//Vector3f distance = m_velocity * /*0.01667f;*/ (float)ElapsedTime::GetInstance().GetElapsedTime();
	RelativePosition(get_velocity_movement());
}

void PhysicsComponent::ActionMove(bool up, bool down, bool left, bool right, float xperc, float yperc)
{
	if (m_conversationLock)
		return;
	//apply_natural_deceleration();
	xperc = abs(xperc);
	yperc = abs(yperc);
	float total = sqrtf(pow(xperc, 2) + pow(yperc, 2));
	float maxTotal = 1.0f;
	if (total > maxTotal && ((up || down) && (left || right)))
	{
		xperc /= total;
		yperc /= total;
	}

	float speed = 1.5f * 64.0f;
	float speedXPerc = speed * xperc;
	float speedYPerc = speed * yperc;
	if (left)
		m_velocity.x -= speedXPerc;
	if (right)
		m_velocity.x += speedXPerc;
	if (up)
		m_velocity.y += speedYPerc;
	if (down)
		m_velocity.y -= speedYPerc;

	if (m_velocity.x > speedXPerc * 2)
		m_velocity.x = speedXPerc * 2;
	if (m_velocity.x < speedXPerc * -2)
		m_velocity.x = speedXPerc * -2;
	if (m_velocity.y > speedYPerc * 2)
		m_velocity.y = speedYPerc * 2;
	if (m_velocity.y < speedYPerc * -2)
		m_velocity.y = speedYPerc * -2;

	SetMovedBB();
}

void PhysicsComponent::SetDefaults(std::string name)
{
	_ethereal = true;

	Model::GetInstance().loadModel(name);

	m_vertices = std::vector<Vertex>(Model::GetInstance().getVertexVertices());

	m_indices = std::vector<GLuint>(Model::GetInstance().getIndices());
	assert(m_indices.size() % 3 == 0);

	//MathUtils::CalcNormals(m_indices, m_vertices);

	SetBoundingBox();
}


//Create the destination BB for collision testing
void PhysicsComponent::SetMovedBB()
{
	m_moveBoundingBox = std::array<float, 6>(m_boundingBox);
	m_moveBoundingBox[Right] += get_velocity_movement().x;//(float)ElapsedTime::GetInstance().GetElapsedTime();
	m_moveBoundingBox[Left] += get_velocity_movement().x;//(float)ElapsedTime::GetInstance().GetElapsedTime();
	m_moveBoundingBox[Up] += get_velocity_movement().y;//(float)ElapsedTime::GetInstance().GetElapsedTime();
	m_moveBoundingBox[Down] += get_velocity_movement().y;//(float)ElapsedTime::GetInstance().GetElapsedTime();
}

std::array<float, 6> PhysicsComponent::GetXMoveBoundingBox()
{
	std::array<float, 6> result = std::array<float, 6>(m_boundingBox);
	result[Right] += get_velocity_movement().x;//(float)ElapsedTime::GetInstance().GetElapsedTime();
	result[Left] += get_velocity_movement().x;//(float)ElapsedTime::GetInstance().GetElapsedTime();

	return result;
}

std::array<float, 6> PhysicsComponent::GetYMoveBoundingBox()
{
	std::array<float, 6> result = std::array<float, 6>(m_boundingBox);
	result[Up] += get_velocity_movement().y;//(float)ElapsedTime::GetInstance().GetElapsedTime();
	result[Down] += get_velocity_movement().y;//(float)ElapsedTime::GetInstance().GetElapsedTime();

	return result;
}

int PhysicsComponent::get_tile_expanse()
{
	return get_tile_expanse(m_moveBoundingBox);
}

int PhysicsComponent::get_tile_expanse(std::array<float, 6> bb)
{
	return ((int)bb[Up] - (int)bb[Down] + 1) * ((int)bb[Right] - (int)bb[Left] + 1);
}


std::array<float, 6>& PhysicsComponent::GetMoveBoundingBox()
{
	return m_moveBoundingBox;
}

Vector3f PhysicsComponent::get_velocity_movement()
{
	return m_velocity * 0.016f;
}


std::array<float, 6> PhysicsComponent::GetEtherealMoveBoundingBox()
{
	if (_ethereal)
	{
		auto result = std::array<float, 6>();
		result.fill(-99999999.f);
		return result;
	}
	else
		return m_moveBoundingBox;
}

void PhysicsComponent::SetBoundingBox()
{

	if (m_size != -1 && m_BBcenter != -1)
	{
		SetBoundingBoxSize(m_size, m_BBcenter);
	}
	else
	{
		m_boundingBox[Right] = m_vertices[0].vertex.x;
		m_boundingBox[Left] = m_vertices[0].vertex.x;
		m_boundingBox[Up] = m_vertices[0].vertex.y;
		m_boundingBox[Down] = m_vertices[0].vertex.y;
		m_boundingBox[Close] = m_vertices[0].vertex.z;
		m_boundingBox[Far] = m_vertices[0].vertex.z;

		for (auto v : m_vertices)
		{
			//Right
			if (v.vertex.x > m_boundingBox[Right])
				m_boundingBox[Right] = v.vertex.x;
			//Left
			else if (v.vertex.x < m_boundingBox[Left])
				m_boundingBox[Left] = v.vertex.x;
			//Up
			if (v.vertex.y > m_boundingBox[Up])
				m_boundingBox[Up] = v.vertex.y;
			//Down
			else if (v.vertex.y < m_boundingBox[Down])
				m_boundingBox[Down] = v.vertex.y;
			//Away
			if (v.vertex.z > m_boundingBox[Far])
				m_boundingBox[Far] = v.vertex.z;
			//Close
			else if (v.vertex.z < m_boundingBox[Close])
				m_boundingBox[Close] = v.vertex.z;
		}
		MoveBB(m_pos);
	}
}

//Size = Desired size
//numSquares = How big is the object in tiles (to determine how to centre the object)
void PhysicsComponent::SetBoundingBoxSize(Vector3f size, Vector3f numSquares)
{
	//Side-Space, if you put 0, itll just stick it to the left/bottom/close(ground)
	float sideX = numSquares.x > 0 ? (numSquares.x - size.x) / 2 : 0;
	float sideY = numSquares.y > 0 ? (numSquares.y - size.y) / 2 : 0;
	float sideZ = numSquares.z > 0 ? (numSquares.z - size.z) / 2 : 0;

	if (size.x >= 0)
	{
		m_boundingBox[Left] = m_vertices[0].vertex.x + sideX;
		m_boundingBox[Right] = size.x + sideX;
	}
	else
	{
		m_boundingBox[Right] = m_vertices[0].vertex.x + sideX;
		m_boundingBox[Left] = size.x + sideX;
	}

	//The close z-value is the one closest to infinity, so its the bottom of the object
	if (size.z >= 0)
	{
		m_boundingBox[Close] = m_vertices[0].vertex.z + sideZ;
		m_boundingBox[Far] = size.z + sideZ;
	}
	else
	{
		m_boundingBox[Far] = m_vertices[0].vertex.z + sideZ;
		m_boundingBox[Close] = size.z + sideZ;
	}

	if (size.y >= 0)
	{
		m_boundingBox[Down] = m_vertices[0].vertex.y + sideY;
		m_boundingBox[Up] = size.y + sideY;
	}
	else
	{
		m_boundingBox[Up] = m_vertices[0].vertex.y + sideY;
		m_boundingBox[Down] = size.y + sideY;
	}

	MoveBB(m_pos);
}

void PhysicsComponent::MoveBB(Vector3f distance)
{
	m_boundingBox[Right] += distance.x;
	m_boundingBox[Left] += distance.x;
	m_boundingBox[Up] += distance.y;
	m_boundingBox[Down] += distance.y;
	m_boundingBox[Close] += distance.z;
	m_boundingBox[Far] += distance.z;
}

std::array<float, 6> PhysicsComponent::GetBoundingBox()
{
	return m_boundingBox;
}

void PhysicsComponent::RelativePosition(Vector3f movementPos)
{
	m_pos += movementPos;
	MoveBB(movementPos);
	SetMovedBB();
}

void PhysicsComponent::AbsolutePosition(Vector3f absolutePos, Vector3f useAxis)
{
	absolutePos.x = useAxis.x == 0 ? m_pos.x : absolutePos.x;
	absolutePos.y = useAxis.y == 0 ? m_pos.y : absolutePos.y;
	absolutePos.z = useAxis.z == 0 ? m_pos.z : absolutePos.z;
	m_pos = absolutePos;
	SetBoundingBox();
	SetMovedBB();
}

std::vector<GLuint> PhysicsComponent::GetIndices()
{
	return m_indices;
}

std::vector<Vertex>& PhysicsComponent::GetVerticesRef()
{
	return m_vertices;
}

std::vector<GLuint>& PhysicsComponent::GetIndicesRef()
{
	return m_indices;
}

int PhysicsComponent::GetHighestIndex()
{
	return m_vertices.size() - 1;
	//GLuint highest = 0;
	//for (auto i : m_indices)
	//	highest = i > highest ? i : highest;

	//return highest;
}

void PhysicsComponent::DesiredMove()
{
	if (m_velocity != 0)
		SetMovedBB();
}

void PhysicsComponent::set_velocity_zero()
{
	m_velocity = Vector3f();
	SetMovedBB();
}

void PhysicsComponent::SetConversationLock(bool locked)
{
	m_conversationLock = locked;
	set_velocity_zero();
}

void PhysicsComponent::SetEthereal(bool ethereal)
{
	_ethereal = ethereal;
}



