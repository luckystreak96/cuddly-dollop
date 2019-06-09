//
// Created by yanik on 09/04/19.
//

#include "color_generator.h"

UniformColorDistribution::UniformColorDistribution(Vector3f interval0, Vector3f interval1) {
    float min_x = std::min(interval0.x, interval1.x);
    float max_x = std::max(interval0.x, interval1.x);
    float min_y = std::min(interval0.y, interval1.y);
    float max_y = std::max(interval0.y, interval1.y);
    float min_z = std::min(interval0.z, interval1.z);
    float max_z = std::max(interval0.z, interval1.z);

    m_red = std::uniform_real_distribution<float>(min_x, max_x);
    m_green = std::uniform_real_distribution<float>(min_y, max_y);
    m_blue = std::uniform_real_distribution<float>(min_z, max_z);
}

Vector3f UniformColorDistribution::next() {
    return Vector3f(m_red(m_generator), m_green(m_generator), m_blue(m_generator));
}

ColorGenerator::ColorGenerator() = default;

Vector3f ColorGenerator::next() {
    return Vector3f();
}
