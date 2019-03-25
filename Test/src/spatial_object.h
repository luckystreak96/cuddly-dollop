//
// Created by yanik on 24/03/19.
//

#ifndef PROJECT_SPATIAL_OBJECT_H
#define PROJECT_SPATIAL_OBJECT_H

#include "vector3f.h"

class SpatialObject
{
public:
    void set_position(Vector3f pos);
    Vector3f& get_position_ref();
    Vector3f get_position();

    void set_velocity(Vector3f vel);
    Vector3f get_velocity();
protected:
    Vector3f m_position;
    Vector3f m_velocity;
};

#endif //PROJECT_SPATIAL_OBJECT_H
