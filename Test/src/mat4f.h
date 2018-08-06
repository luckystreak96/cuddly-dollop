#ifndef MAT4F_H__
#define MAT4F_H__
#include <math.h>
#include <iostream>
#include "vector3f.h"
#include "projection.h"

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

        void ToIdentityMatrix(float (&matrix)[4][4]);

        void InitScaleMat(Vector3f &scale);
        void InitTranslateMat(Vector3f &translation);
        void InitRotateMat(Vector3f &rot);

		void SetTranslation(Vector3f& trans);

        void InitProjPers(PersProjInfo p);
        void InitOrthoProj(OrthoProjInfo o);
		void Init3dCameraTrans(const Vector3f& target, const Vector3f& up);

        void Copy(const float (&mat)[4][4], float (&target)[4][4]);
        void Multiply(float (&first)[4][4], const float (&mat)[4][4]);

    public:
        float m[4][4];
};

#endif
