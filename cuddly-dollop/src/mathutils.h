#ifndef MATHUTILS_H__
#define MATHUTILS_H__

#include "vector3f.h"
#include "vertex.h"
//#include "GL/glew.h"
#include "lerper.h"
#include "mat4f.h"
#include "projection.h"

#include <cassert>
#include <vector>
#include <memory>

class Transformation;

enum CameraSpeeds { CAMSPEED_Slow, CAMSPEED_Normal, CAMSPEED_Fast };
enum CameraStyle { CAMSTYLE_Follow, CAMSTYLE_FollowDad, CAMSTYLE_FollowDadNoScale };

class Camera
{
public:
	Camera();
	void Update();
	// Set the position to lerp towards
	void SetFollow(Vector3f pos);
	// get_position is pulled toward MapCenter.y
	void SetFollowCenteredY(Vector3f pos);
	// get_position is pulled toward MapCenter.xy
	void SetFollowCenteredXY(Vector3f pos);
	// Set the scale to lerp toward
	void SetScale(Vector3f scale);
	// Update follow - lerp to _followTarget
	void ExecuteFollow();
	// Update scale - lerp to _scaleTarget
	void ExecuteScale();
	// Return the center of the map as Vec3
	Vector3f MapCenter();
	// Set the speed the camera should lerp
	void SetCameraFollowSpeed(CameraSpeeds cs);
	// Forcefully set the position - no lerp
	void ForcePosition(Vector3f pos);
	// Forcefully set the scale - no lerp
	void ForceScale(Vector3f scale);
	// Method assumes a CENTERED_TILE - returns if the object is on the screen
	bool IsOnCamera(Vector3f position, Vector3f size);
	// Resets dad cam as if we set a new target
	void EnableNormalCam();

	Vector3f Get3dPos();

private:
	// Returns the random values for DadCam
	float RandomDad();
	float RandomDadScale();

public:
	bool _paused;

	// The camera currently in use
	static Camera* _currentCam;
	// default is 0.005f
	float _followSpeed;
	// The id of the person to follow if relevant
	int Target;
	// Used to set cam boundries
	Vector3f _mapsize;
	// Camera follow style - ex: DadFollow
	CameraStyle _style;
	// the lerp class for translation
	Lerper _lerperTrans;
	// the lerp class for scaling
	Lerper _lerperScale;
	// the extra position to add (for stuff like screen shaking that wants to ignore lerp)
	Vector3f _extraTranslate;
	// the transformation matrix handler
	std::unique_ptr<Transformation> _transform;
	// the current position
	Vector3f _translate;
private:
	// used to track how long to wait till dadCam
	int _dadCountdown;
	// used to track how long to wait in between dad locations
	int _dadSwitch;
	// decides if dadcam is in progress
	bool _followingDad;
	// the current scale
	Vector3f _scale;
	// the target to translate to
	Vector3f _followTarget;
	// the temporary target for dadCam
	Vector3f _followTargetDad;
	// the temporary target for dadCam
	Vector3f _scaleTargetDad;
	// the scale to scale towards
	Vector3f _scaleTarget;

public:
	Vector3f _3dTarget;
	Vector3f _3dUp;
	float _z;
};


namespace MathUtils
{
	//void CalcNormals(std::vector<GLuint>& indices, std::vector<Vertex>& verts);
	Vector3f FindPositionInParabola(float progressPercent, Vector3f startPoint, Vector3f endPoint);

	float HeightGivenLengthOfHypotenuseAndAngle(float length, float angleRadians);
}

#endif
