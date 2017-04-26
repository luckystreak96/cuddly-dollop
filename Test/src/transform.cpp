#include "transform.h"

Transformation::Transformation()
{
}

Mat4f Transformation::GetWorldTrans()
{
    Mat4f scale, translate, rotate;

    scale.InitScaleMat(m_scale);
    translate.InitTranslateMat(m_translate);
    rotate.InitRotateMat(m_rotate);
    m_WTrans = translate * rotate * scale;

    return m_WTrans;
}

Mat4f Transformation::GetWOTrans()
{
	m_WTrans = GetWorldTrans();
	m_Proj.InitOrthoProj(*m_orthoProj);
	m_WPTrans = m_Proj * m_WTrans;

	return m_WPTrans;
}

Mat4f Transformation::GetWPTrans()
{
    m_WTrans = GetWorldTrans();
    m_Proj.InitProjPers(*m_persProjInfo);
    m_WPTrans = m_Proj * m_WTrans;

    return m_WPTrans;
}

Mat4f Transformation::GetTrans()
{
    Mat4f scale, translate, rotate, camTrans, camRot, persProjTrans;
    scale.InitScaleMat(m_scale);
    translate.InitTranslateMat(m_translate);
    rotate.InitRotateMat(m_rotate);
    camTrans.InitTranslateMat(-m_camera.Pos);
    camRot.InitCameraTransform(m_camera.Target, m_camera.Up);
    m_Proj.InitProjPers(*m_persProjInfo);

    m_transformation = m_Proj * (camRot * camTrans) * (translate * rotate * scale);
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

void Transformation::SetCamera(Camera cam)
{
    SetCamera(cam.Up, cam.Pos, cam.Target);
}

void Transformation::SetCamera(Vector3f up, Vector3f pos, Vector3f target)
{
    m_camera = Camera();
    m_camera.Up = up;
    m_camera.Pos = pos;
    m_camera.Target = target;
}

void Transformation::SetPersProjInfo(PersProjInfo* p)
{
    m_persProjInfo = p;
}

void Transformation::SetOrthoProj(OrthoProjInfo* o)
{
	m_orthoProj = o;
}


void Transformation::SetScale(Vector3f& vec)
{
    SetScale(vec.x, vec.y, vec.z);
}

void Transformation::SetTranslation(Vector3f& vec)
{
    SetTranslation(vec.x, vec.y, vec.z);
}

void Transformation::SetRotation(Vector3f& vec)
{
    SetRotation(vec.x, vec.y, vec.z);
}

Camera& Transformation::GetCamera()
{
	return m_camera;
}