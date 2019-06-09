//
// Created by yanik on 09/04/19.
//

#ifndef PROJECT_COLOR_GENERATOR_H
#define PROJECT_COLOR_GENERATOR_H

#include "vector3f.h"
#include <random>

class ColorGenerator
{
public:
    ColorGenerator();
    virtual Vector3f next();

protected:
    std::default_random_engine m_generator;
};

class UniformColorDistribution : public ColorGenerator
{
public:
    UniformColorDistribution(Vector3f interval0, Vector3f interval1);
    Vector3f next();

private:
    std::uniform_real_distribution<float> m_red;
    std::uniform_real_distribution<float> m_blue;
    std::uniform_real_distribution<float> m_green;
};


#endif //PROJECT_COLOR_GENERATOR_H
