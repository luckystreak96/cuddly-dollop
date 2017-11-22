#include "vector3f.h"

Vector3f::Vector3f() : x(0), y(0), z(0) {}
Vector3f::Vector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
Vector3f::Vector3f(float f) : x(f), y(f), z(f) {}

Vector3f Vector3f::Abs()
{
	Vector3f result = Vector3f();
	result.x = abs(x);
	result.y = abs(y);
	result.z = abs(z);

	return result;
}

float Vector3f::AngleBetween(Vector3f v)
{
	float dot = Dot(v);
	float lenA = Length();
	float lenB = v.Length();
	float cos = dot / lenA * lenB;
	return acos(cos);
}

float Vector3f::Length()
{
	return sqrtf(x*x + y*y + z*z);
}

float Vector3f::PercentDistance(Vector3f& other)
{
	float a, b, c;
	a = other.x != 0 ? (x / other.x) : 0;
	if (a > 1)
		a = (other.x / x);
	b = other.y != 0 ? (y / other.y) : 0;
	if (b > 1)
		b = (other.y / y);
	c = other.z != 0 ? (z / other.z) : 0;
	if (c > 1)
		c = (other.z / z);

	float result = a;
	if (result < b)
		result = b;
	if (result < c)
		result = c;

	return result;
}

Vector3f Vector3f::Cross(const Vector3f& v)
{
	const float _x = y * v.z - z * v.y;
	const float _y = z * v.x - x * v.z;
	const float _z = x * v.y - y * v.x;

	return Vector3f(_x, _y, _z);
}

float Vector3f::Dot(Vector3f v)
{
	return x * v.x + y * v.y + z * v.z;
}

float Vector3f::VectorProjection(Vector3f normal)
{
	normal = normal.Normalize();
	return Dot(normal);
}

float& Vector3f::operator[](int i)
{
	if (i == 0)
		return x;
	if (i == 1)
		return y;
	if (i == 2)
		return z;
	std::cout << "ACCESSING IMPOSSIBLE VECTOR [] THINGY" << std::endl;
	//system("PAUSE");
	exit(1);
}

Vector3f Vector3f::operator-()
{
	Vector3f result = Vector3f();
	result.x = -this->x;
	result.y = -this->y;
	result.z = -this->z;
	return result;
}

Vector3f& Vector3f::operator+=(const Vector3f& r)
{
	this->x += r.x;
	this->y += r.y;
	this->z += r.z;

	return *this;
}

bool Vector3f::operator==(const Vector3f& r)
{
	return (this->x == r.x && this->y == r.y && this->z == r.z);
}

bool Vector3f::operator<(const Vector3f& r)
{
	if (x == r.x)
	{
		if (y == r.y)
		{
			if (z == r.z)
			{

			}
			else
			{
				return z < r.z;
			}
		}
		else
		{
			return y < r.y;
		}
	}
	else
	{
		return x < r.x;
	}

	return false;
}

bool Vector3f::operator>(const Vector3f& r)
{
	if (z == r.z)
	{
		if (y == r.y)
		{
			if (x == r.x)
			{

			}
			else
			{
				return x > r.x;
			}
		}
		else
		{
			return y > r.y;
		}
	}
	else
	{
		return z > r.z;
	}

	return false;
}

bool Vector3f::operator!=(const Vector3f& r)
{
	return !(this->x == r.x && this->y == r.y && this->z == r.z);
}

bool Vector3f::operator!=(const float& r)
{
	return !(this->x == r && this->y == r && this->z == r);
}

bool Vector3f::operator==(const int& i)
{
	return (x == i && y == i && z == i);
}

bool Vector3f::operator<(const float& i)
{
	return (x < i && y < i && z < i);
}

bool Vector3f::operator>(const float& i)
{
	return (x > i && y > i && z > i);
}

Vector3f& Vector3f::operator-=(const Vector3f& r)
{
	this->x -= r.x;
	this->y -= r.y;
	this->z -= r.z;

	return *this;
}

Vector3f& Vector3f::operator*=(float c)
{
	this->x *= c;
	this->y *= c;
	this->z *= c;

	return *this;
}

Vector3f Vector3f::operator*(float r)
{
	Vector3f temp(this->x * r,
		this->y * r,
		this->z * r);

	return temp;
}

Vector3f Vector3f::operator/(float r)
{
	Vector3f temp(this->x / r,
		this->y / r,
		this->z / r);

	return temp;
}

Vector3f Vector3f::operator+(Vector3f vec)
{
	Vector3f temp(this->x + vec.x,
		this->y + vec.y,
		this->z + vec.z);

	return temp;
}

Vector3f Vector3f::operator-(Vector3f vec)
{
	Vector3f temp(this->x - vec.x,
		this->y - vec.y,
		this->z - vec.z);

	return temp;
}

Vector3f& Vector3f::Normalize()
{
	const float Length = sqrtf(x * x + y * y + z * z);

	x /= Length;
	y /= Length;
	z /= Length;

	return *this;
}

std::ostream& operator<<(std::ostream& output, const Vector3f& v)
{
	output << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
	return output;
}

float Vector3f::Distance2D(Vector3f& other)
{
	float a = other.x - x;
	float b = other.y - y;
	return sqrtf(a*a + b*b);
}

Vector2f::Vector2f(float x, float y) : x(x), y(y) {}
