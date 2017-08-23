#ifndef ENTITY2_H__
#define ENTITY2_H__

#include "vector3f.h"
#include <vector>
#include "define.h"

class Entity2
{
public:
	Vector3f Position();
	Vector3f Velocity();
	std::vector<Ent_Props>& GetProps();
protected:
	Vector3f Emulate(float elapsedTime);
	virtual void Move(float elapsedTime);
	void AddProp(Ent_Props prop);
	bool ContainsProp(Ent_Props prop);
protected:
	std::vector<Ent_Props> m_props;
	Vector3f m_pos;
	Vector3f m_velocity;
};

#endif