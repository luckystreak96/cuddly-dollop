#include "mat4f.h"

#define M_PI 3.14159265358979323
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

/*TIPS AND TRICKS
 * --------------
 *
 *  Matrix Transformation order:
 *      1- Scale
 *      2- Rotate
 *      3- Translate
 *      4- Camera
 */

Mat4f::Mat4f()
{
	ToIdentityMatrix(m);
}

Mat4f::Mat4f(float a00, float a01, float a02, float a03,
	float a10, float a11, float a12, float a13,
	float a20, float a21, float a22, float a23,
	float a30, float a31, float a32, float a33)
{
	m[0][0] = a00; m[0][1] = a01; m[0][2] = a02; m[0][3] = a03;
	m[1][0] = a10; m[1][1] = a11; m[1][2] = a12; m[1][3] = a13;
	m[2][0] = a20; m[2][1] = a21; m[2][2] = a22; m[2][3] = a23;
	m[3][0] = a30; m[3][1] = a31; m[3][2] = a32; m[3][3] = a33;
}

void Mat4f::ToIdentityMatrix(float(&matrix)[4][4])
{
	matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = 0; matrix[0][3] = 0;
	matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = 0; matrix[1][3] = 0;
	matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1; matrix[2][3] = 0;
	matrix[3][0] = 0; matrix[3][1] = 0; matrix[3][2] = 0; matrix[3][3] = 1;
}

void Mat4f::InitTranslateMat(Vector3f &translation)
{
	ToIdentityMatrix(m);
	m[0][3] = translation.x;
	m[1][3] = translation.y;
	m[2][3] = translation.z;
}

void Mat4f::InitRotateMat(Vector3f &rot)
{
	Mat4f rotMatX, rotMatY, rotMatZ;

	//0, rotates on the X axis
	rotMatX.m[1][1] = cos(rot.x); rotMatX.m[1][2] = -sin(rot.x);
	rotMatX.m[2][1] = sin(rot.x); rotMatX.m[2][2] = cos(rot.x);
	//rotates on the Y axis
	rotMatY.m[0][0] = cos(rot.y); rotMatY.m[0][2] = -sin(rot.y);
	rotMatY.m[2][0] = sin(rot.y); rotMatY.m[2][2] = cos(rot.y);
	//rotates on the Z axis
	rotMatZ.m[0][0] = cos(rot.z); rotMatZ.m[0][1] = -sin(rot.z);
	rotMatZ.m[1][0] = sin(rot.z); rotMatZ.m[1][1] = cos(rot.z);

	*this = rotMatZ * rotMatY * rotMatX;
}

void Mat4f::InitScaleMat(Vector3f &scale)
{
	m[0][0] = scale.x;
	m[1][1] = scale.y;
	m[2][2] = scale.z;
}

void Mat4f::Copy(const float(&mat)[4][4], float(&target)[4][4])
{
	target[0][0] = mat[0][0]; target[0][1] = mat[0][1]; target[0][2] = mat[0][2]; target[0][3] = mat[0][3];
	target[1][0] = mat[1][0]; target[1][1] = mat[1][1]; target[1][2] = mat[1][2]; target[1][3] = mat[1][3];
	target[2][0] = mat[2][0]; target[2][1] = mat[2][1]; target[2][2] = mat[2][2]; target[2][3] = mat[2][3];
	target[3][0] = mat[3][0]; target[3][1] = mat[3][1]; target[3][2] = mat[3][2]; target[3][3] = mat[3][3];
	std::cout << "Y U STILL COPY WITH ME" << std::endl;
}

Mat4f Mat4f::operator*(const Mat4f &right) const
{
	Mat4f mat;

	float result;
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result = 0;
			for (int value = 0; value < 4; value++) {
				result += m[row][value] * right.m[value][column];
			}
			mat.m[row][column] = result;
		}
	}
	return mat;
}

Vector3f Mat4f::Multiply(Vector3f &right, float pretendVec4) const
{
	Vector3f vector = Vector3f();

	float result;
	for (int row = 0; row < 4; row++)
	{
		result = 0;
		for (int column = 0; column < 4; column++) {
			if (column != 3)
				result += m[row][column] * right[column];
			else
				result += m[row][column] * pretendVec4;
		}
		if (row != 3)
			vector[row] = result;
	}
	return vector;
}

void Mat4f::Multiply(float(&first)[4][4], const float(&second)[4][4])
{
	float copy[4][4];
	Copy(first, copy);

	float result;
	for (int row = 0; row < 4; row++) {
		for (int column = 0; column < 4; column++) {
			result = 0;
			for (int value = 0; value < 4; value++) {
				result += copy[row][value] * second[value][column];
			}
			m[row][column] = result;
		}
	}
	std::cout << "Y U STILL USE ME (PS I M MAT4F.MULTIPLY)" << std::endl;
}

void Mat4f::InitProjPers(PersProjInfo p)
{
	const float ar = p.Width / p.Height;
	const float zRange = p.zNear - p.zFar;
	const float tanHalfFOV = tanf(ToRadian(p.FOV / 2.0));

	m[0][0] = 1.0f / (tanHalfFOV * ar);
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f / tanHalfFOV;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = (-p.zNear - p.zFar) / zRange;
	m[2][3] = 2.0f * p.zFar * p.zNear / zRange;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 1.0f;
	m[3][3] = 0.0f;
}

void Mat4f::InitOrthoProj(OrthoProjInfo o)
{
	const float top = o.Top;
	const float bottom = o.Bottom;
	const float right = o.Right;
	const float left = o.Left;
	const float close = o.zNear;
	const float away = o.zFar;

	m[0][0] = 2.0f / (right - left);
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 2.f / (top - bottom);
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 2 / (close - away);
	m[2][3] = 0.0f;

	m[3][0] = (right + left) / (left - right);
	m[3][1] = (top + bottom) / (bottom - top);
	m[3][2] = (close + away) / (away - close);
	m[3][3] = 1;

	//m[0][0] = 0.1f;
	//m[0][1] = 0.0f;
	//m[0][2] = 0.0f;
	//m[0][3] = 0.0f;

	//m[1][0] = 0.0f;
	//m[1][1] = 1;
	//m[1][2] = 0.0f;
	//m[1][3] = 0.0f;

	//m[2][0] = 0.0f;
	//m[2][1] = 0.0f;
	//m[2][2] = 1;
	//m[2][3] = 0.0f;

	//m[3][0] = 0;
	//m[3][1] = 0;
	//m[3][2] = 0;
	//m[3][3] = 1;
}

void Mat4f::SetScale(Vector3f scale)
{
	m_scale.x = scale.x;
	m_scale.y = scale.y;
	m_scale.z = scale.z;
}

void Mat4f::SetRotation(Vector3f rotation)
{
	m_rotation.x = rotation.x;
	m_rotation.y = rotation.y;
	m_rotation.z = rotation.z;
}

void Mat4f::SetTranslation(Vector3f translation)
{
	m_translation.x = translation.x;
	m_translation.y = translation.y;
	m_translation.z = translation.z;
}

void Mat4f::InitCameraTransform(Vector3f& Target, Vector3f& Up)
{
	Vector3f N = Target;
	N.Normalize();
	Vector3f U = Up;
	U.Normalize();
	U = U.Cross(N);
	Vector3f V = N.Cross(U);

	m[0][0] = U.x;   m[0][1] = U.y;   m[0][2] = U.z;   m[0][3] = 0.0f;
	m[1][0] = V.x;   m[1][1] = V.y;   m[1][2] = V.z;   m[1][3] = 0.0f;
	m[2][0] = N.x;   m[2][1] = N.y;   m[2][2] = N.z;   m[2][3] = 0.0f;
	m[3][0] = 0.0f;  m[3][1] = 0.0f;  m[3][2] = 0.0f;  m[3][3] = 1.0f;
}
