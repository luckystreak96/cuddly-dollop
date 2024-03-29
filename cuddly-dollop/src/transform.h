#ifndef TRANSFORM_H__
#define TRANSFORM_H__

#include "vector3f.h"
#include "mat4f.h"
#include "mathutils.h"

class Transformation
{
public:
	Transformation();

	void SetScale(float x, float y, float z);
	void SetScale(const Vector3f vec);
	void SetTranslation(float x, float y, float z);
	void SetTranslation(const Vector3f vec);
	void AddTranslation(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetRotation(Vector3f vec);
	void SetPersProjInfo(PersProjInfo* p);
	void SetOrthoProj(OrthoProjInfo* o);
	void SetFollowSpeed(float percentSpeed);
	void Follow(Vector3f target, Vector3f upperRightLimit);

	bool operator!=(const Transformation& t);

	Mat4f& GetWorldTrans();
	Mat4f& GetWorldTransCentered();
	Mat4f& GetWorldTransNoTranslate();
	Mat4f& GetWOTrans();
	Mat4f& GetWOTransCentered();
	Mat4f& GetWOTransNoTranslate();
	Mat4f& GetWPTrans();
	Mat4f& GetTrans();
	Vector3f GetTranslation() { return m_translate; };
	Vector3f GetScale() { return m_scale; };
	Vector3f GetRotation() { return m_rotate; };

public:
	static bool perspectiveOrtho;

private:
	float m_followSpeed = 1;
	Mat4f m_WTrans;
	Mat4f m_Proj;
	Mat4f m_WPTrans;
	Mat4f m_WPTransNoTranslate;
	Mat4f m_transformation;
	Vector3f m_scale = Vector3f(1, 1, 1);
	Vector3f m_translate = Vector3f(0, 0, 0);
	Vector3f m_rotate = Vector3f(0, 0, 0);
	PersProjInfo* m_persProjInfo = NULL;
	OrthoProjInfo* m_orthoProj = NULL;
};

#endif
