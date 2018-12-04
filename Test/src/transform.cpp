#include "transform.h"

#include <iostream>

bool Transformation::perspectiveOrtho = false;

Transformation::Transformation()
{
}

Mat4f& Transformation::GetWorldTrans()
{
	Mat4f scale, translate, rotate;

	scale.InitScaleMat(m_scale);
	rotate.InitRotateMat(m_rotate);
	translate.InitTranslateMat(m_translate);
	m_WTrans = translate * rotate * scale;

	return m_WTrans;
}

Mat4f& Transformation::GetWorldTransCentered()
{
	Mat4f scale, translate, rotate;

	scale.InitScaleMat(m_scale);
	rotate.InitRotateMat(m_rotate);
	translate.InitTranslateMat(m_translate);
	m_WTrans = rotate * scale * translate;

	return m_WTrans;
}

Mat4f& Transformation::GetWorldTransNoTranslate()
{
	Mat4f scale, translate, rotate;

	scale.InitScaleMat(m_scale);
	Vector3f trans = Vector3f(m_orthoProj->Left, m_orthoProj->Bottom, 0);
	translate.InitTranslateMat(trans);
	rotate.InitRotateMat(m_rotate);
	m_WTrans = translate * rotate/* * scale*/;

	return m_WTrans;
}

Mat4f& Transformation::GetWOTrans()
{
	Mat4f camtranslate, camrotate;
	camtranslate.InitTranslateMat(Camera::_currentCam->Get3dPos());
	camrotate.Init3dCameraTrans(Camera::_currentCam->_3dTarget, Camera::_currentCam->_3dUp);

	m_WTrans = GetWorldTrans();
	if (perspectiveOrtho)
	{
		m_Proj.InitOrthoProj(*m_orthoProj);
		m_WPTrans = m_Proj * m_WTrans;
	}
	else
	{
		m_Proj.InitProjPers(PersProjInfo::GetRegularInstance());
		m_WPTrans = m_Proj * (camrotate * camtranslate) * m_WTrans;
	}

	return m_WPTrans;
}

Mat4f& Transformation::GetWOTransCentered()
{
	Mat4f camtranslate, camrotate;
	camtranslate.InitTranslateMat(Camera::_currentCam->Get3dPos());
	camrotate.Init3dCameraTrans(Camera::_currentCam->_3dTarget, Camera::_currentCam->_3dUp);

	m_WTrans = GetWorldTransCentered();
	if (perspectiveOrtho)
	{
		m_Proj.InitOrthoProj(*m_orthoProj);
		m_WPTrans = m_Proj * m_WTrans;
	}
	else
	{
		m_Proj.InitProjPers(PersProjInfo::GetRegularInstance());
		m_WPTrans = m_Proj * (camrotate * camtranslate) * m_WTrans;
	}

	return m_WPTrans;
}

Mat4f& Transformation::GetWOTransNoTranslate()
{
	Mat4f camtranslate, camrotate;
	camtranslate.InitTranslateMat(Camera::_currentCam->Get3dPos());
	camrotate.Init3dCameraTrans(Camera::_currentCam->_3dTarget, Camera::_currentCam->_3dUp);

	m_WTrans = GetWorldTransNoTranslate();
	if (perspectiveOrtho)
	{
		m_Proj.InitOrthoProj(*m_orthoProj);
		m_WPTransNoTranslate = m_Proj * m_WTrans;
	}
	else
	{
		m_Proj.InitProjPers(PersProjInfo::GetRegularInstance());
		m_WPTransNoTranslate = m_Proj * (camrotate * camtranslate) * m_WTrans;
	}

	return m_WPTransNoTranslate;
}

Mat4f& Transformation::GetWPTrans()
{
	m_WTrans = GetWorldTrans();
	m_Proj.InitProjPers(*m_persProjInfo);
	m_WPTrans = m_Proj * m_WTrans;

	return m_WPTrans;
}

Mat4f& Transformation::GetTrans()
{
	Mat4f scale, translate, rotate, persProjTrans;
	scale.InitScaleMat(m_scale);
	translate.InitTranslateMat(m_translate);
	rotate.InitRotateMat(m_rotate);
	m_Proj.InitProjPers(*m_persProjInfo);

	m_transformation = m_Proj * (translate * rotate * scale);
	return m_transformation;
}

void Transformation::SetScale(float x, float y, float z)
{
	m_scale = Vector3f(x, y, z);
}

void Transformation::SetTranslation(float x, float y, float z)
{
	m_translate = Vector3f(x, y, z);
}

void Transformation::SetRotation(float x, float y, float z)
{
	m_rotate = Vector3f(x, y, z);
}

void Transformation::SetPersProjInfo(PersProjInfo* p)
{
	m_persProjInfo = p;
}

void Transformation::SetOrthoProj(OrthoProjInfo* o)
{
	m_orthoProj = o;
}


void Transformation::SetScale(const Vector3f vec)
{
	SetScale(vec.x, vec.y, vec.z);
}

void Transformation::SetTranslation(const Vector3f vec)
{
	SetTranslation(vec.x, vec.y, vec.z);
}

void Transformation::AddTranslation(float x, float y, float z)
{
	m_translate.x += x;
	m_translate.y += y;
	m_translate.z += z;
}

void Transformation::SetRotation(Vector3f vec)
{
	SetRotation(vec.x, vec.y, vec.z);
}

//Between 0 and 1
void Transformation::SetFollowSpeed(float percentSpeed)
{
	m_followSpeed = percentSpeed;
}

//Target is the object that the camera should pan towards
void Transformation::Follow(Vector3f target, Vector3f upperRightLimit)
{
	// Follow the center of the character
	target.x += 0.5f;
	target.y += 0.5f;

	float size = OrthoProjInfo::GetRegularInstance().Size;

	float distanceX = -(target.x * m_scale.x) - m_translate.x;//find the distance between the 2, -target so that the movement of the world will be proper
	float distanceY = -(target.y * m_scale.y) - m_translate.y;

	float percentagex = abs(distanceX) / 15.f / m_scale.x / 2.f;
	float percentagey = abs(distanceY) / 10.f / m_scale.x / 2.f;

	// Sets how much the camera will move in this frame
	m_translate.x += distanceX * (percentagex * m_followSpeed);
	m_translate.y += distanceY * (percentagey * m_followSpeed);

	// If the pan would bring you left further than the left limit OR the map is too small to fit the screen width,
	//  just pan at the left limit
	if (m_translate.x - ((OrthoProjInfo::GetRegularInstance().Left) / size) > 0 ||
		upperRightLimit.x < ((OrthoProjInfo::GetRegularInstance().Right) / size) * 2)
		m_translate.x = ((OrthoProjInfo::GetRegularInstance().Left) / size);
	// If the map is big enough for the screen to pan it right and you would normally pass the limits,
	//  set the pan to the exact right limit
	else if (abs(m_translate.x - ((OrthoProjInfo::GetRegularInstance().Right) / size)) > upperRightLimit.x * m_scale.x)
		m_translate.x = -(upperRightLimit.x * m_scale.x - ((OrthoProjInfo::GetRegularInstance().Right) / size));

	// If the pan would bring you down further than the bottom OR the map isnt high enough to fill the screen,
	//  just stay at the bottom
	if (m_translate.y - ((OrthoProjInfo::GetRegularInstance().Bottom) / size) > 0 ||
		upperRightLimit.y < ((OrthoProjInfo::GetRegularInstance().Top) / size) * 2)
		m_translate.y = ((OrthoProjInfo::GetRegularInstance().Bottom) / size);
	// If the map is big enough for the screen to pan it upwards and you would normally pass the limits,
	//  set the pan to the exact top
	else if (abs(m_translate.y - ((OrthoProjInfo::GetRegularInstance().Top) / size)) > upperRightLimit.y * m_scale.y)
		m_translate.y = -(upperRightLimit.y * m_scale.y - ((OrthoProjInfo::GetRegularInstance().Top) / size));
}

bool Transformation::operator!=(const Transformation& t)
{
	if (this->m_translate == t.m_translate)
		if (this->m_rotate == t.m_rotate)
			if (this->m_scale == t.m_scale)
				return false;

	return true;
}
