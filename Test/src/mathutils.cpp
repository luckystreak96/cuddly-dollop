#include "mathutils.h"
#include "transform.h"

static const float StepSize = 0.2f;
int Camera::Target = 1;
Vector3f Camera::Mapsize = Vector3f();

void Camera::MapCenter(Transformation* t)
{
	float left = OrthoProjInfo::GetRegularInstance().Left;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;
	float size = OrthoProjInfo::GetRegularInstance().Size;
	// (Maplength - viewWidth) / 2
	Vector3f result;
	result.x = (Mapsize.x - ((abs(left) + abs(right)) / size)) / 2.f;
	result.y = (Mapsize.y - ((top * 2.f) / size)) / 2.f;
	result.x += right / size;
	result.y += top / size;
	result.z = t->GetTranslation().z;
	t->SetTranslation(-result);
}

void Camera::Follow(Vector3f target, Transformation* t)
{
	Vector3f translate, scale;
	translate = t->GetTranslation();
	scale = t->GetScale();

	// Follow the center of the character
	target.x += 0.5f;
	target.y += 0.5f;

	float size = OrthoProjInfo::GetRegularInstance().Size;

	float distanceX = -(target.x * scale.x) - translate.x;//find the distance between the 2, -target so that the movement of the world will be proper
	float distanceY = -(target.y * scale.y) - translate.y;

	//float percentagex = abs(distanceX) / 15.f / scale.x / 2.f;
	//float percentagey = abs(distanceY) / 10.f / scale.x / 2.f;

	float percentagex = (0.005f * pow(distanceX, 2)) * scale.x + 0.02f;
	float percentagey = (0.005f * pow(distanceY, 2)) * scale.y + 0.02f;

	percentagex = fmin(percentagex, 1.0f);
	percentagey = fmin(percentagey, 1.0f);

	// Sets how much the camera will move in this frame
	translate.x += distanceX * percentagex;
	translate.y += distanceY * percentagey;

	// If the pan would bring you left further than the left limit OR the map is too small to fit the screen width,
	//  just pan at the left limit
	if (translate.x - ((OrthoProjInfo::GetRegularInstance().Left) / size) > 0 ||
		Mapsize.x < ((OrthoProjInfo::GetRegularInstance().Right) / size) * 2)
		translate.x = ((OrthoProjInfo::GetRegularInstance().Left) / size);
	// If the map is big enough for the screen to pan it right and you would normally pass the limits,
	//  set the pan to the exact right limit
	else if (abs(translate.x - ((OrthoProjInfo::GetRegularInstance().Right) / size)) > Mapsize.x * scale.x)
		translate.x = -(Mapsize.x * scale.x - ((OrthoProjInfo::GetRegularInstance().Right) / size));

	// If the pan would bring you down further than the bottom OR the map isnt high enough to fill the screen,
	//  just stay at the bottom
	if (translate.y - ((OrthoProjInfo::GetRegularInstance().Bottom) / size) > 0 ||
		Mapsize.y < ((OrthoProjInfo::GetRegularInstance().Top) / size) * 2)
		translate.y = ((OrthoProjInfo::GetRegularInstance().Bottom) / size);
	// If the map is big enough for the screen to pan it upwards and you would normally pass the limits,
	//  set the pan to the exact top
	else if (abs(translate.y - ((OrthoProjInfo::GetRegularInstance().Top) / size)) > Mapsize.y * scale.y)
		translate.y = -(Mapsize.y * scale.y - ((OrthoProjInfo::GetRegularInstance().Top) / size));

	t->SetTranslation(translate);
}



namespace MathUtils
{
	// ProgressPercent = between 0 and 1
	// The final result disregards the z value
	Vector3f FindPositionInParabola(float progressPercent, Vector3f startPoint, Vector3f endPoint)
	{
		// The x to be used in the final calculation
		float x = ((endPoint.x - startPoint.x) * progressPercent) + startPoint.x;
		Vector3f result;
		result.x = x;
		float a = -0.08f;
		float y1 = startPoint.y;
		float y2 = endPoint.y;
		float x1 = startPoint.x;
		float x2 = endPoint.x;
		float h = (y2 - a * pow(x2, 2) - y1 + a * pow(x1, 2)) / (2 * a * (x1 - x2));
		float k = y1 - a * pow(x1 - h, 2);
		
		result.y = a * pow(x - h, 2) + k;

		return result;
	}

	//void CalcNormals(std::vector<GLuint>& indices, std::vector<Vertex>& verts)
	//{
	//	int vertCount = 0;
	//	for(Vertex& v : verts)
	//	{
	//		std::vector<Vector3f> norms = std::vector<Vector3f>();
	//		for (unsigned int i = 0; i < indices.size(); i += 3) {
	//			unsigned int Index0 = indices[i];
	//			unsigned int Index1 = indices[i + 1];
	//			unsigned int Index2 = indices[i + 2];
	//			if (Index0 == vertCount || Index1 == vertCount || Index2 == vertCount)
	//			{
	//				Vector3f v1 = verts[Index1].vertex - verts[Index0].vertex;
	//				Vector3f v2 = verts[Index2].vertex - verts[Index0].vertex;
	//				Vector3f Normal = v1.Cross(v2);
	//				Normal.Normalize();
	//				if (std::find(norms.begin(), norms.end(), Normal) != norms.end())
	//					continue;

	//				norms.push_back(Normal);

	//				v.normal += Normal;
	//			}
	//		}
	//		vertCount++;
	//	}

	//	for(Vertex& v : verts)
	//	{
	//		v.normal.Normalize();
	//	}
	//}
}
