#ifndef PHYSICS_H__
#define PHYSICS_H__

#include <string>
#include <GL/glew.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "array2d.h"
#include "vector3f.h"
#include "vertex.h"
#include "elapsedTime.h"
#include <vector>
#include <array>

#define SMALL_NUM   0.00000001 // anything that avoids division overflow
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)

struct Ray
{
	Vector3f P0, P1;
};

struct Triangle
{
	Vector3f V0, V1, V2;
};

struct CollisionObject
{
	CollisionObject(std::vector<Vertex> v, std::vector<GLuint> i, Vector3f vt) : verts(v), inds(i), velocity(vt) {};
	std::vector<Vertex> verts;
	std::vector<GLuint> inds;
	Vector3f velocity;
};

namespace Physics
{
	int intersect3D_RayTriangle(Ray R, Triangle T, Vector3f* I);
	bool Intersect2D(const std::array<float, 6>& local, const std::array<float, 6>& other);
	bool Intersect2D(const std::array<float, 6>& local, Vector3f point);
	bool Intersect3D(std::array<float, 6>& local, std::array<float, 6>& other);
	void FindClosestCollision(CollisionObject co1, CollisionObject co2, bool* collisionConfirmed, float* percentMovement, 
		Vector3f* triangleNormal, bool* first, Triangle* tri, Vector3f* traject, int* vertIndex, Vector3f* poc);
	bool RespectsNormal(Vector3f vel, Vector3f normal);
}

#endif
