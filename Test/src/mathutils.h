#ifndef MATHUTILS_H__
#define MATHUTILS_H__

#include <cassert>
#include <vector>
#include <memory>
#include "vector3f.h"
#include "vertex.h"
#include "GL/glew.h"
#include "lerper.h"

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
enum CameraSpeeds { CAMSPEED_Slow, CAMSPEED_Normal, CAMSPEED_Fast };
enum FollowType { FT_Exponential, FT_Stable };
enum CameraStyle { CAMSTYLE_Follow, CAMSTYLE_FollowDad };

class Camera
{
public:
	Camera();
	void Update();
	void SetFollow(Vector3f& pos);
	void SetFollowCenteredY(Vector3f pos);
	void SetScale(Vector3f& scale);
	void ExecuteFollow(bool useDadPos = false);
	void ExecuteScale();
	Vector3f MapCenter();
	void SetCameraFollowSpeed(CameraSpeeds cs);
	// Method assumes a CENTERED_TILE
	bool IsOnCamera(Vector3f& position, Vector3f& size);
public:
	static Camera* _currentCam;
	// default is 0.005f
	float _followSpeed;
	FollowType _followConfiguration;
	int Target;
	Vector3f _mapsize;
	CameraStyle _style;
	std::unique_ptr<Transformation> _transform;
	Lerper _lerper;
private:
	Vector3f _translate;
	Vector3f _scale;
	Vector3f _followTarget;
	Vector3f _followTargetDad;
	Vector3f _scaleTarget;
};


namespace MathUtils
{
	//void CalcNormals(std::vector<GLuint>& indices, std::vector<Vertex>& verts);
	Vector3f FindPositionInParabola(float progressPercent, Vector3f startPoint, Vector3f endPoint);
}

#endif
