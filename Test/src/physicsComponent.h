#ifndef PHYSICS_COMPONENT_H__
#define PHYSICS_COMPONENT_H__

#include "define.h"
#include "iComponent.h"
#include "vector3f.h"
#include <cassert>
#include "array2d.h"
#include "model.h"
#include "elapsedTime.h"
#include <array>

class PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(Vector3f pos = Vector3f(), std::string modelName = "TILE", Vector3f size = Vector3f(), Vector3f numTilesSize = Vector3f(-1, -1, -1));
	void ReceiveMessage(std::vector<std::string> message);
	virtual void Update();
	void SetDefaults(std::string name = "TILE");//cancer
	std::string GetName();
	void ActionMove(bool up, bool down, bool left, bool right);
	void Move();
	void MoveBB(Vector3f distance);
	void SetBoundingBox();
	void SetBoundingBoxSize(Vector3f size, Vector3f numSquare = Vector3f(1, 1, 1));
	std::array<float, 6> GetBoundingBox();
	std::array<float, 6> GetMoveBoundingBox();
	Vector3f BBSize();
	void RelativePosition(Vector3f movementPos);
	void AbsolutePosition(Vector3f absolutePos, Vector3f useAxis = Vector3f(1, 1, 1));
	std::vector<Vertex> GetVertices();
	std::vector<Vertex> GetTranslatedVertices();
	std::vector<GLuint> GetIndices();
	int GetHighestIndex();
	void XCollide();
	void YCollide();
	virtual void DesiredMove();
	void SetMovedBB();
	void SetTranslatedVertices();
	Vector3f Position() { return m_pos; };
	Vector3f GetCenter() { return Vector3f(m_boundingBox[Left] + m_size.x / 2, m_boundingBox[Down] + m_size.y / 2, m_boundingBox[Far] + m_size.z / 2);/*return m_pos + (m_size / 2);*/ }
	void SetPosition(Vector3f pos) { m_pos = pos; };
	Vector3f Velocity() { return m_velocity; };
	Vector3f GetSize() { return m_size; }
	void RemoveVelocity();
	void ApplyGravity();
	void SetConversationLock(bool locked);
	//static inline bool SortFunc(Drawable* d, Drawable* d2) { return d->Position().z < d2->Position().z; }

	bool mustCollide = true;
protected:
	Vector3f m_pos;
	Vector3f m_velocity;
	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
	std::vector<Vertex> m_translatedVertices;
	std::vector<Vertex> m_originalVertices;
	std::vector<Vector3f> m_models;
	Vector3f m_size = Vector3f(-1, -1, -1);
	Vector3f m_BBcenter = Vector3f(-1, -1, -1);
	bool m_conversationLock;
	//Texture m_texture;
	std::string m_texture;
	std::string m_modelName = "NONE"; //cancer
	Vector3f m_rot = Vector3f(0, 0, 0);
	std::array<float, 6> m_boundingBox = std::array<float, 6>();
	std::array<float, 6> m_moveBoundingBox = std::array<float, 6>();
	//Array2d<float> m_boundingBox = Array2d<float>(6, 1);
	//Array2d<float> m_moveBoundingBox = Array2d<float>(6, 1);
};

#endif
