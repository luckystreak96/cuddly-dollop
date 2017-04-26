#include "mathutils.h"

static const float StepSize = 0.2f;

Camera::Camera()
{
	InitCam();
}

Camera::Camera(float Window_Width, float Window_Height) : m_windowWidth(Window_Width), m_windowHeight(Window_Height)
{
	InitCam();
}

void Camera::InitCam()
{
	Pos = Vector3f(0.0f, 0.0f, -z_distance);
	Target = Pos - Vector3f(0.0f, 0.0f, 1.0f);
	Up = Vector3f(0.0f, 1.0f, 0.0f);
}

void Camera::Update(Vector3f player_pos)
{
	player_pos.z -= 0.8f;
	Vector3f movement = ((player_pos - Vector3f(0, 0, z_distance)) - Vector3f(Pos.x, Pos.y, Pos.z));
	movement.x /= 60;
	movement.y /= 60;
	movement.z /= 60;

	Pos += movement;
}

void Camera::RotateHorizontal(bool right)
{
	Target.x += right ? 0.1f : -0.1f;
}

void Camera::RotateVertical(bool up)
{
	Target.z += up ? 0.1f : -0.1f;
}

void Camera::RotateSpiral(bool clockwise)
{
	Target.y += clockwise ? 0.1f : -0.1f;
}

namespace MathUtils
{
	void CalcNormals(std::vector<GLuint>& indices, std::vector<Vertex>& verts)
	{
		int vertCount = 0;
		for(Vertex& v : verts)
		{
			std::vector<Vector3f> norms = std::vector<Vector3f>();
			for (unsigned int i = 0; i < indices.size(); i += 3) {
				unsigned int Index0 = indices[i];
				unsigned int Index1 = indices[i + 1];
				unsigned int Index2 = indices[i + 2];
				if (Index0 == vertCount || Index1 == vertCount || Index2 == vertCount)
				{
					Vector3f v1 = verts[Index1].vertex - verts[Index0].vertex;
					Vector3f v2 = verts[Index2].vertex - verts[Index0].vertex;
					Vector3f Normal = v1.Cross(v2);
					Normal.Normalize();
					if (std::find(norms.begin(), norms.end(), Normal) != norms.end())
						continue;

					norms.push_back(Normal);

					v.normal += Normal;
				}
			}
			vertCount++;
		}

		for(Vertex& v : verts)
		{
			v.normal.Normalize();
		}
	}
}
