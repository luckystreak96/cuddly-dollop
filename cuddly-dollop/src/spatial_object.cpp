//
// Created by yanik on 24/03/19.
//

#include "spatial_object.h"

Vector3f &SpatialObject::get_position_ref() {
    return m_position;
}

Vector3f SpatialObject::get_position() {
    return m_position;
}

void SpatialObject::set_position(Vector3f pos) {
    m_position = pos;
}

void SpatialObject::set_velocity(Vector3f vel) {
    m_velocity = vel;
}

Vector3f SpatialObject::get_velocity() {
    return m_velocity;
}
