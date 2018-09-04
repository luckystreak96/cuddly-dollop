#include "mathutils.h"
#include "transform.h"

Camera* Camera::_currentCam = NULL;

Camera::Camera() : Target(1), _followSpeed(0.005f), _scale(Vector3f(1)), _scaleTargetDad(Vector3f(1)), _style(CAMSTYLE_Follow),
_scaleTarget(Vector3f(1)), _paused(false), _3dTarget(0, 0.5f, 1), _3dUp(0, 1, 0)
{
	_transform = std::make_unique<Transformation>(Transformation());
}

Vector3f Camera::MapCenter()
{
	float left = OrthoProjInfo::GetRegularInstance().Left;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;
	float size = OrthoProjInfo::GetRegularInstance().Size;
	// (Maplength - viewWidth) / 2
	Vector3f result;
	// +1 otherwise it's not well centered
	result.x = ((_mapsize.x + 1) - ((abs(left) + abs(right)) / size)) / 2.f;
	// didn't do a +1 cause the map looks good without it (xd)
	result.y = ((_mapsize.y + 1) - ((top * 2.f) / size)) / 2.f;
	result.x += right / size;
	result.y += top / size;
	result.z = _transform->GetTranslation().z;

	return result;
}

void Camera::SetCameraFollowSpeed(CameraSpeeds cs)
{
	switch (cs)
	{
	case CAMSPEED_Slow:
		_lerperTrans.Acceleration = 0.000005f;
		break;
	case CAMSPEED_Normal:
		_lerperTrans.Acceleration = 0.0005f;
		break;
	case CAMSPEED_Fast:
		_lerperTrans.Acceleration = 0.005f;
		break;
	}
}

bool Camera::IsOnCamera(Vector3f& position, Vector3f& size)
{
	float targetRightLimit = position.x + 0.5f * size.x;
	float targetLeftLimit = position.x - 0.5f * size.x;
	float targetTopLimit = position.y + 0.5f * size.y;
	float targetBottomLimit = position.y - 0.5f * size.y;

	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;
	float orthoSize = OrthoProjInfo::GetRegularInstance().Size;

	float cameraRightLimit = -_translate.x + (right / orthoSize) / _scale.x;
	float cameraLeftLimit = -_translate.x - (right / orthoSize) / _scale.x;
	float cameraTopLimit = -_translate.y + (top / orthoSize) / _scale.y;
	float cameraBottomLimit = -_translate.y - (top / orthoSize) / _scale.y;

	// Needs to be in left-right limits AND top-down limits
	if (((targetRightLimit > cameraLeftLimit && targetRightLimit < cameraRightLimit) ||
		(targetLeftLimit < cameraRightLimit && targetLeftLimit > cameraLeftLimit)) &&
		((targetTopLimit > cameraBottomLimit && targetTopLimit < cameraTopLimit) ||
		(targetBottomLimit < cameraTopLimit && targetBottomLimit > cameraBottomLimit)))
		return true;

	return false;
}

void Camera::ForcePosition(Vector3f& pos)
{
	_translate = -pos;
}

void Camera::ForceScale(Vector3f& scale)
{
	_scale = scale;
}

void Camera::Update()
{
	if (_paused)
		return;
	switch (_style)
	{
	case CAMSTYLE_FollowDadNoScale:
	case CAMSTYLE_FollowDad:
		_dadCountdown++;

		// If the translate is rlly close to target, start targeting random nearby places
		// The lack of break will make the execute activate as well

		// Gotta make the followTarget negative cause otherwise it doesnt register in the method
		if (_dadCountdown > 60 * 10 &&
			((!_followingDad && _translate.NearXY(-_followTarget, 0.075f)) || _followingDad && _translate.NearXY(-_followTargetDad, 0.075f)) &&
			((!_followingDad && _scale.NearXY(_scaleTarget, 0.01f)) || _followingDad && _scale.NearXY(_scaleTargetDad, 0.01f)))
		{
			_followingDad = true;
			_lerperTrans.Amount = 0.010f;
			_lerperTrans.Acceleration = 0.0002f;

			_followTargetDad = _followTarget + Vector3f(RandomDad(), RandomDad(), 0); // set the random distance here
			if (_style != CAMSTYLE_FollowDadNoScale)
			{
				Vector3f scale = RandomDad() / 6.f;
				_scaleTargetDad = _scaleTarget + scale; // set the random distance here
			}
		}
	case CAMSTYLE_Follow:
		ExecuteScale();
		ExecuteFollow();
		break;
	default:
		break;
	}
}

float Camera::RandomDad()
{
	float result = fmod(rand(), 1.4f) - 0.7f;
	return result;
}

void Camera::ExecuteScale()
{
	Vector3f target = _followingDad ? _scaleTargetDad : _scaleTarget;

	// Set the scale slowly as the follow takes effect
	Vector3f lerp = _lerperScale.Lerp(_scale, target);

	_scale.x = lerp.x;
	_scale.y = lerp.y;

	_transform->SetScale(_scale);
}

void Camera::ExecuteFollow()
{
	Vector3f target = _followingDad ? _followTargetDad : _followTarget;

	// Sets how much the camera will move in this frame
	Vector3f lerp = _lerperTrans.Lerp(-_translate, target);

	_translate.x = -lerp.x;
	_translate.y = -lerp.y;

	float size = OrthoProjInfo::GetRegularInstance().Size;
	float left = OrthoProjInfo::GetRegularInstance().Left;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float bottom = OrthoProjInfo::GetRegularInstance().Bottom;
	float top = OrthoProjInfo::GetRegularInstance().Top;

	if (Transformation::perspectiveOrtho)
	{
		// If the pan would bring you left further than the left limit OR the map is too small to fit the screen width,
		//  just pan at the left limit
		if (_translate.x - ((left) / size) / _scale.x > 0 ||
			_mapsize.x < ((right) / size) * 2)
			_translate.x = ((left) / size) / _scale.x;
		// If the map is big enough for the screen to pan it right and you would normally pass the limits,
		//  set the pan to the exact right limit
		else if (abs(_translate.x - ((right) / size) / _scale.x) > _mapsize.x)
			_translate.x = -(_mapsize.x - ((right) / size) / _scale.x);

		// If the pan would bring you down further than the bottom OR the map isnt high enough to fill the screen,
		//  just stay at the bottom
		if (_translate.y - ((bottom) / size) / _scale.y > 0 ||
			_mapsize.y < ((top) / size) * 2)
			_translate.y = ((bottom) / size) / _scale.y;
		// If the map is big enough for the screen to pan it upwards and you would normally pass the limits,
		//  set the pan to the exact top
		else if (abs(_translate.y - ((top) / size) / _scale.y) > _mapsize.y)
			_translate.y = -(_mapsize.y - ((top) / size) / _scale.y);
	}

	_transform->SetTranslation(_translate + _extraTranslate);
}

void Camera::SetScale(Vector3f& scale)
{
	_scaleTarget = scale;
}

void Camera::SetFollow(Vector3f& target)
{
	if (target == _followTarget)
		return;
	_followTarget = target + Vector3f(0, Transformation::perspectiveOrtho ? 0 : -2.5, 0);

	EnableNormalCam();
}

void Camera::EnableNormalCam()
{
	// Reset the dad-following check
	_followingDad = false;
	_dadCountdown = 0;
	_lerperTrans.Amount = 0.025f;
	_lerperTrans.Acceleration = 0.02f;
}

void Camera::SetFollowCenteredY(Vector3f target)
{
	auto center = MapCenter();
	target.y = center.y + ((target.y - center.y) / 2);
	SetFollow(target);
}

void Camera::SetFollowCenteredXY(Vector3f target)
{
	auto center = MapCenter();
	target.x = center.x + ((target.x - center.x) / 2.f);
	target.y = center.y + ((target.y - center.y) / 1.5f);
	SetFollow(target);
}

Vector3f Camera::Get3dPos()
{
	Vector3f result = Vector3f();// _translate;
	result.z = 15;
	return result;
}

namespace MathUtils
{
	// ProgressPercent = between 0 and 1
	// The final result disregards the z value
	Vector3f FindPositionInParabola(float progressPercent, Vector3f startPoint, Vector3f endPoint)
	{
		// The x to be used in the final calculation
		float x = ((endPoint.x - startPoint.x) * progressPercent) + startPoint.x;
		Vector3f result;
		result.x = x;
		float a = -0.08f;
		float y1 = startPoint.y;
		float y2 = endPoint.y;
		float x1 = startPoint.x;
		float x2 = endPoint.x;
		float h = (y2 - a * pow(x2, 2) - y1 + a * pow(x1, 2)) / (2 * a * (x1 - x2));
		float k = y1 - a * pow(x1 - h, 2);

		result.y = a * pow(x - h, 2) + k;
		// Set the z here or it becomes 0 and the characters jump over the GUI
		result.z = startPoint.z;

		return result;
	}

	float HeightGivenLengthOfHypotenuseAndAngle(float length, float angleRadians)
	{
		return length * cosf(angleRadians);
	}

	//void CalcNormals(std::vector<GLuint>& indices, std::vector<Vertex>& verts)
	//{
	//	int vertCount = 0;
	//	for(Vertex& v : verts)
	//	{
	//		std::vector<Vector3f> norms = std::vector<Vector3f>();
	//		for (unsigned int i = 0; i < indices.size(); i += 3) {
	//			unsigned int Index0 = indices[i];
	//			unsigned int Index1 = indices[i + 1];
	//			unsigned int Index2 = indices[i + 2];
	//			if (Index0 == vertCount || Index1 == vertCount || Index2 == vertCount)
	//			{
	//				Vector3f v1 = verts[Index1].vertex - verts[Index0].vertex;
	//				Vector3f v2 = verts[Index2].vertex - verts[Index0].vertex;
	//				Vector3f Normal = v1.Cross(v2);
	//				Normal.Normalize();
	//				if (std::find(norms.begin(), norms.end(), Normal) != norms.end())
	//					continue;

	//				norms.push_back(Normal);

	//				v.normal += Normal;
	//			}
	//		}
	//		vertCount++;
	//	}

	//	for(Vertex& v : verts)
	//	{
	//		v.normal.Normalize();
	//	}
	//}
}
