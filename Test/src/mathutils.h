#ifndef MATHUTILS_H__
#define MATHUTILS_H__

#include "vector3f.h"
#include "vertex.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cassert>
#include <vector>

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

	float Top = 0;
	float Bottom = 0;
	float Left = 0;
	float Right = 0;
	float zNear = 0.0f;
	float zFar = 1000.0f;
	bool changed = true;
private:
	OrthoProjInfo() {};
};

class Camera
{
    public:
        Camera();
        Camera(float Window_Width, float Window_Height);
		void Update(Vector3f player_pos = Vector3f());
		void RotateHorizontal(bool right);
		void RotateVertical(bool up);
		void RotateSpiral(bool clockwise);
    public:
        Vector3f Up;
        Vector3f Pos;
        Vector3f Target;
    private:
        void InitCam();
    private:
		float z_distance = 15.0f;
        float m_windowWidth = 800;
        float m_windowHeight = 600;
};

namespace MathUtils 
{
	void CalcNormals(std::vector<GLuint>& indices, std::vector<Vertex>& verts);
}

#endif
