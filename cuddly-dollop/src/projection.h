#ifndef PROJECTION_H__
#define PROJECTION_H__

class PersProjInfo
{
public:
	static PersProjInfo& GetRegularInstance()
	{
		static PersProjInfo regularInstance;
		return regularInstance;
	}

	static PersProjInfo& GetShadowInstance()
	{
		static PersProjInfo shadowInstance;
		return shadowInstance;
	}
	float FOV = 30.0f;
	float Height = 0;
	float Width = 0;
	float zNear = 1.0f;
	float zFar = 1000.0f;
private:
	PersProjInfo() {};
};

class OrthoProjInfo
{
public:
	static OrthoProjInfo& GetRegularInstance()
	{
		static OrthoProjInfo regularInstance;
		return regularInstance;
	}

	float Top;
	float Bottom;
	float Left;
	float Right;
	float zNear;
	float zFar;

	float Size;

	bool changed = true;
private:
	OrthoProjInfo() {};
};

#endif // !PROJECTION_H__
