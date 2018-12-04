#ifndef VECTOR_H__
#define VECTOR_H__

#include <math.h>
#include <iostream>
#include <string>

class Vector3f
{
public:
	Vector3f();
	Vector3f(float _x, float _y, float _z);
	Vector3f(float f);

	Vector3f Cross(const Vector3f& v);
	Vector3f& Normalize();
	Vector3f operator-();
	Vector3f& operator+=(const Vector3f& r);
	Vector3f OnlyXY();
	bool NearXY(Vector3f other, float distance) const;
	friend std::ostream& operator<<(std::ostream& output, const Vector3f& D);
	bool operator<(const Vector3f& r);
	bool operator>(const Vector3f& r);
	bool operator==(const Vector3f& r);
	bool operator!=(const Vector3f& r);
	bool operator!=(const float & r);
	bool operator==(const int& i);
	bool operator<(const float& i);
	bool operator>(const float& i);
	Vector3f& operator-=(const Vector3f& r);
	float& operator[](int i);
	Vector3f& operator*=(float c);
	Vector3f& operator*=(Vector3f c);
	Vector3f& operator/=(float c);
	Vector3f operator*(float r);
	Vector3f operator*(Vector3f v);
	Vector3f operator/(float r);
	Vector3f operator+(Vector3f v);
	Vector3f operator-(Vector3f v);
	Vector3f Abs();
	float AngleBetween(Vector3f v);
	float Length();
	float Dot(Vector3f v);
	float VectorProjection(Vector3f normal);
	float Distance2D(Vector3f& other);
	float PercentDistance(Vector3f& other);
	std::string Print();

	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
};

class Vector2f
{
public:
	Vector2f();
	Vector2f(float x, float y);
	bool operator==(const Vector2f& r);
	bool operator<(const Vector2f& i) const;
	Vector2f operator-();
public:
	float x = 0.0f;
	float y = 0.0f;
};

#endif
