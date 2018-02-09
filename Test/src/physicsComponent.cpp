#include "physicsComponent.h"

PhysicsComponent::PhysicsComponent(Vector3f pos, std::string modelName, Vector3f size, Vector3f numTiles) : m_size(size), 
m_BBcenter(numTiles), m_conversationLock(false), walkOn(true)
{
	m_pos = pos;
	m_velocity = Vector3f();

	SetDefaults(modelName);
}

// Sets the color of all vertices to change the color of the sprite
void PhysicsComponent::SetColorAll(Vector3f color, float alpha)
{
	for (auto& v : m_vertices)
	{
		v.color = color;
		v.alpha = alpha;
	}
}

void PhysicsComponent::Update()
{
	if (m_velocity != 0)
		PhysicsComponent::Move();

	SetMovedBB();
}

void PhysicsComponent::ApplyGravity()
{
	//Slow down a bit
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
	Vector3f distance = m_velocity * 0.01667f;// (float)ElapsedTime::GetInstance().GetElapsedTime();
	RelativePosition(distance);
}

void PhysicsComponent::ActionMove(bool up, bool down, bool left, bool right)
{
	if (m_conversationLock)
		return;
	//ApplyGravity();

	float speed = 1.5f;
	if (left)
		m_velocity.x -= speed;
	if (right)
		m_velocity.x += speed;
	if (up)
		m_velocity.y += speed;
	if (down)
		m_velocity.y -= speed;

	if (m_velocity.x > speed * 2)
		m_velocity.x = speed * 2;
	if (m_velocity.x < speed * -2)
		m_velocity.x = speed * -2;
	if (m_velocity.y > speed * 2)
		m_velocity.y = speed * 2;
	if (m_velocity.y < speed * -2)
		m_velocity.y = speed * -2;

	SetMovedBB();
}

void PhysicsComponent::SetDefaults(std::string name)
{
	_ethereal = false;
	m_modelName = name;

	Model::GetInstance().loadModel(name);
	m_modelName = Model::GetInstance().GetName();
	std::vector<float> verts = Model::GetInstance().getVertices();
	std::vector<float> tex = Model::GetInstance().GetTex();

	assert(verts.size() % 3 == 0);//full vertices only plz
								  //assert(tex.size() == (verts.size() % 3) * 2);//2 texcoords per vertex

	int numVertex = (int)verts.size() / 3;

	m_originalVertices = std::vector<Vertex>();

	for (int x = 0; x < numVertex; x++)
	{
		m_originalVertices.push_back(
			Vertex(Vector3f(verts[x * 3], verts[x * 3 + 1], verts[x * 3 + 2]),
				Vector2f(x * 2 < tex.size() ? tex[x * 2] : 0, x * 2 + 1 < tex.size() ? tex[x * 2 + 1] : 0)));//TEXCOORD MOD HERE PLZ
	}

	m_vertices = std::vector<Vertex>(m_originalVertices);

	{
		std::vector<GLuint> indices = Model::GetInstance().getIndices();
		assert(indices.size() % 3 == 0);

		m_indices = std::vector<GLuint>();

		for (auto i : indices)
			m_indices.push_back(i);
	}

	//MathUtils::CalcNormals(m_indices, m_vertices);

	SetBoundingBox();
}

void PhysicsComponent::SetTranslatedVertices()
{
	m_translatedVertices = std::vector<Vertex>(m_vertices);
	for (Vertex& v : m_translatedVertices)
	{
		v.vertex.x += m_pos.x + m_velocity.x * (float)ElapsedTime::GetInstance().GetElapsedTime();
		v.vertex.y += m_pos.y + m_velocity.y * (float)ElapsedTime::GetInstance().GetElapsedTime();
		v.vertex.z += m_pos.z + m_velocity.z * (float)ElapsedTime::GetInstance().GetElapsedTime();
	}
}

std::vector<Vertex> PhysicsComponent::GetTranslatedVertices()
{
	return m_translatedVertices;
}

//Create the destination BB for collision testing
void PhysicsComponent::SetMovedBB()
{
	m_moveBoundingBox = std::array<float, 6>(m_boundingBox);
	m_moveBoundingBox[Right] += m_velocity.x * (float)ElapsedTime::GetInstance().GetElapsedTime();
	m_moveBoundingBox[Left] += m_velocity.x * (float)ElapsedTime::GetInstance().GetElapsedTime();
	m_moveBoundingBox[Up] += m_velocity.y * (float)ElapsedTime::GetInstance().GetElapsedTime();
	m_moveBoundingBox[Down] += m_velocity.y * (float)ElapsedTime::GetInstance().GetElapsedTime();
}


std::array<float, 6>& PhysicsComponent::GetMoveBoundingBox()
{
	return m_moveBoundingBox;
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

Vector3f PhysicsComponent::BBSize()
{
	float w = m_boundingBox[Right] - m_boundingBox[Left];
	float h = m_boundingBox[Up] - m_boundingBox[Down];
	float d = m_boundingBox[Far] - m_boundingBox[Close];
	return Vector3f(w, h, d);
}

std::string PhysicsComponent::GetName()
{
	return m_modelName;
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

std::vector<Vertex> PhysicsComponent::GetVertices()
{
	return m_vertices;
}

std::vector<GLuint> PhysicsComponent::GetIndices()
{
	return m_indices;
}

int PhysicsComponent::GetHighestIndex()
{
	GLuint highest = 0;
	for (auto i : m_indices)
		highest = i > highest ? i : highest;

	return highest;
}

void PhysicsComponent::DesiredMove()
{
	if (m_velocity != 0)
		SetMovedBB();
}

void PhysicsComponent::RemoveVelocity()
{
	m_velocity = Vector3f();
	SetMovedBB();
}

void PhysicsComponent::SetConversationLock(bool locked)
{
	m_conversationLock = locked;
	RemoveVelocity();
}

void PhysicsComponent::SetEthereal(bool ethereal)
{
	_ethereal = ethereal;
}



