#ifndef MATHUTILS_H__
#define MATHUTILS_H__

#include "vector3f.h"
#include "vertex.h"
#include "GL/glew.h"
#include <cassert>
#include <vector>
#include <memory>

class Transformation;

class PersProjInfo
{
public:
	static PersProjInfo& GetRegularInstance()
	{
		static PersProjInfo regularInstance;
		return regularInstance;
	}

	static PersProjInfo& GetShadowInstance()
	{
		static PersProjInfo shadowInstance;
		return shadowInstance;
	}
	float FOV = 30.0f;
	float Height = 0;
	float Width = 0;
	float zNear = 1.0f;
	float zFar = 1000.0f;
private:
	PersProjInfo() {};
};

class OrthoProjInfo
{
public:
	static OrthoProjInfo& GetRegularInstance()
	{
		static OrthoProjInfo regularInstance;
		return regularInstance;
	}

	float Top;
	float Bottom;
	float Left;
	float Right;
	float zNear;
	float zFar;

	float Size;

	bool changed = true;
private:
	OrthoProjInfo() {};
};

//#include "transform.h"

class Camera
{
public:
	Camera();
	void Follow(Vector3f pos);
	void FollowScale(Vector3f& pos, Vector3f& zoomTarget);
	void MapCenter();
	// Method assumes a CENTERED_TILE
	bool IsOnCamera(Vector3f& position, Vector3f& size);
public:
	static Camera* _currentCam;
	int Target;
	Vector3f _mapsize;
	std::unique_ptr<Transformation> _transform;
	Vector3f _translate;
	Vector3f _scale;
};

namespace MathUtils
{
	//void CalcNormals(std::vector<GLuint>& indices, std::vector<Vertex>& verts);
	Vector3f FindPositionInParabola(float progressPercent, Vector3f startPoint, Vector3f endPoint);
}

#endif
