#ifndef PHYSICS_COMPONENT_H__
#define PHYSICS_COMPONENT_H__

#include "define.h"
#include "iComponent.h"
#include "vector3f.h"
#include "vertex.h"
#include "observable.h"
#include "../res/include/GL/glew.h"

#include <cassert>
#include <memory>
#include <array>
#include <vector>

class PhysicsComponent;
typedef std::shared_ptr<PhysicsComponent> Physics_ptr;

class PhysicsComponent : public IComponent
{
public:
	PhysicsComponent(Vector3f pos, std::string modelName = "TILE", Vector3f size = Vector3f(), Vector3f numTilesSize = Vector3f(-1, -1, -1));
	virtual void init();
	virtual void Update();
	virtual void handle_input(Observable*) {}
	void SetDefaults(std::string name = "TILE");
	void ActionMove(bool up, bool down, bool left, bool right, float xperc, float yperc);
	void SetEthereal(bool ethereal);
	void Move();
	void MoveBB(Vector3f distance);
	void SetBoundingBox();
	void SetBoundingBoxSize(Vector3f size, Vector3f numSquare = Vector3f(1, 1, 1));
	std::array<float, 6> GetBoundingBox();
	std::array<float, 6>& GetMoveBoundingBox();
	std::array<float, 6> GetXMoveBoundingBox();
	std::array<float, 6> GetYMoveBoundingBox();
	std::array<float, 6> GetEtherealMoveBoundingBox();
	void RelativePosition(Vector3f movementPos);
	void AbsolutePosition(Vector3f absolutePos, Vector3f useAxis = Vector3f(1, 1, 1));
	std::vector<Vertex>& GetVerticesRef();
	std::vector<GLuint> GetIndices();
	std::vector<GLuint>& GetIndicesRef();
	int GetHighestIndex();
	void XCollide();
	void YCollide();
	virtual void DesiredMove();
	void SetMovedBB();
	Vector3f Position() { return m_pos; };
	Vector3f& PositionRef() { return m_pos; };
	Vector3f GetCenter() { return Vector3f(m_boundingBox[Left] + m_size.x / 2, m_boundingBox[Down] + m_size.y / 2, m_boundingBox[Far] + m_size.z / 2);/*return m_pos + (m_size / 2);*/ }
	void SetPosition(Vector3f pos) { m_pos = pos; };
	Vector3f Velocity() { return m_velocity; };
	void PhysicsComponent::set_velocity(Vector3f vel);
	Vector3f GetSize() { return m_size; }
	Vector3f get_velocity_movement();
	int get_tile_expanse();
	int get_tile_expanse(std::array<float, 6> bb);
	void set_velocity_zero();
	void apply_natural_deceleration();
	void SetConversationLock(bool locked);
	//static inline bool SortFunc(Drawable* d, Drawable* d2) { return d->Position().z < d2->Position().z; }

private:
	PhysicsComponent();

public:
	bool walkOn;
	bool _ethereal;
	bool _unmoving;

	// Contains id of component that it collided with
	int _collided_last_frame;


protected:
	Vector3f m_pos;
	Vector3f m_velocity;
	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
	Vector3f m_size = Vector3f(-1, -1, -1);
	Vector3f m_BBcenter = Vector3f(-1, -1, -1);
	bool m_conversationLock;
	Vector3f m_rot = Vector3f(0, 0, 0);
	std::array<float, 6> m_boundingBox = std::array<float, 6>();
	std::array<float, 6> m_moveBoundingBox = std::array<float, 6>();
};

#endif
