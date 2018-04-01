#ifndef MATHUTILS_H__
#define MATHUTILS_H__

#include <cassert>
#include <vector>
#include <memory>
#include "vector3f.h"
#include "vertex.h"
#include "GL/glew.h"
#include "lerper.h"
#include "mat4f.h"
#include "projection.h"

class Transformation;

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
	void SetFollowCenteredXY(Vector3f pos);
	void SetScale(Vector3f& scale);
	void ExecuteFollow();
	void ExecuteScale();
	Vector3f MapCenter();
	void SetCameraFollowSpeed(CameraSpeeds cs);
	// Method assumes a CENTERED_TILE
	bool IsOnCamera(Vector3f& position, Vector3f& size);

private:
	float RandomDad();

public:
	static Camera* _currentCam;
	// default is 0.005f
	float _followSpeed;
	FollowType _followConfiguration;
	int Target;
	Vector3f _mapsize;
	// Camera follow style - ex: DadFollow
	CameraStyle _style;
	Lerper _lerperTrans;
	Lerper _lerperScale;
	std::unique_ptr<Transformation> _transform;
private:
	int _dadCountdown;
	bool _followingDad;
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
