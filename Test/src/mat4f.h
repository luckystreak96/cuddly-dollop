#ifndef MAT4F_H__
#define MAT4F_H__
#include <math.h>
#include <iostream>
#include "vector3f.h"
#include "mathutils.h"

class Mat4f
{
    public:
        Mat4f();
        Mat4f(float a00, float a01, float a02, float a03,
                float a10, float a11, float a12, float a13,
                float a20, float a21, float a22, float a23,
                float a30, float a31, float a32, float a33);

		static Mat4f GetIdentity() { static Mat4f instance; return instance; };

        Mat4f operator*(const Mat4f &mat) const;
		Vector3f Multiply(Vector3f &vec, float pretendVec4) const;

        void SetScale(Vector3f scale);
        void SetRotation(Vector3f rotation);
        void SetTranslation(Vector3f translation);

        void ToIdentityMatrix(float (&matrix)[4][4]);

        void InitScaleMat(Vector3f &scale);
        void InitTranslateMat(Vector3f &translation);
        void InitRotateMat(Vector3f &rot);

        void InitCameraTransform(Vector3f& Target, Vector3f& Up);

        void InitProjPers(PersProjInfo p);
        void InitOrthoProj(OrthoProjInfo o);

        void Copy(const float (&mat)[4][4], float (&target)[4][4]);
        void Multiply(float (&first)[4][4], const float (&mat)[4][4]);

    public:
        //Camera camera;
        float m[4][4];

        //Vector3f m_scale;
        //Vector3f m_rotation;
        //Vector3f m_translation;
};

#endif
