#ifndef PHYSICS_RPG_H__
#define PHYSICS_RPG_H__

#include "physics_type.h"

class physics_rpg : public physics_type
{
public:
	physics_rpg();
	void calculate_collision();
	void calculate_position_adjustments();

private:
	bool m_initialized;
};

#endif