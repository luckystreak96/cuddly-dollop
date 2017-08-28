#ifndef PHYSICS_COMPONENT_H__
#define PHYSICS_COMPONENT_H__

#include "define.h"
#include "iComponent.h"
#include "vector3f.h"
#include <cassert>
#include "array2d.h"
#include "model.h"
#include "elapsedTime.h"

class PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(Vector3f pos = Vector3f(), std::string modelName = "TILE");
	void ReceiveMessage(std::string message) {};
	virtual void Update();
	void SetDefaults(std::string name = "TILE");//cancer
	std::string GetName();
	void ActionMove(bool up, bool down, bool left, bool right);
	void Move();
	void MoveBB(Vector3f distance);
	void SetBoundingBox();
	void SetBoundingBoxSize(Vector3f size, Vector3f numSquare = Vector3f(1, 1, 1));
	Array2d<float> GetBoundingBox();
	Array2d<float> GetMoveBoundingBox();
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
	void SetPosition(Vector3f pos) { m_pos = pos; };
	Vector3f Velocity() { return m_velocity; };
	//static inline bool SortFunc(Drawable* d, Drawable* d2) { return d->Position().z < d2->Position().z; }

	bool mustCollide = true;
	bool mustMove = true;
private:
	Vector3f m_pos;
	Vector3f m_velocity;
	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
	std::vector<Vertex> m_translatedVertices;
	std::vector<Vertex> m_originalVertices;
	std::vector<Vector3f> m_models;
	Vector3f m_size = Vector3f(-1, -1, -1);
	Vector3f m_normalSize = Vector3f(-1, -1, -1);;
	//Texture m_texture;
	std::string m_texture;
	std::string m_modelName = "NONE"; //cancer
	Vector3f m_rot = Vector3f(0, 0, 0);
	Array2d<float> m_boundingBox = Array2d<float>(6, 1);
	Array2d<float> m_moveBoundingBox = Array2d<float>(6, 1);
};

#endif
