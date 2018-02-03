#include "fontGraphicsComponent.h"

Transformation FontGraphicsComponent::m_staticityTrans = Transformation();

FontGraphicsComponent::FontGraphicsComponent(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string texPath) : GraphicsComponent(verts, inds, texPath)
{
}

FontGraphicsComponent::FontGraphicsComponent(std::string model, std::string path) : GraphicsComponent(model, path)
{
}

void FontGraphicsComponent::Update()
{
	GraphicsComponent::Update();
}

void FontGraphicsComponent::Draw(bool withTex)
{
	// For drawing the font at a fixed location and not be affected by the moving screen
	if (m_static)
	{
		//This gets called every time, maybe should be changed?
		m_staticityTrans.SetOrthoProj(&OrthoProjInfo::GetRegularInstance());
		m_staticityTrans.SetTranslation(OrthoProjInfo::GetRegularInstance().Left, OrthoProjInfo::GetRegularInstance().Bottom, 0);

		//Return to previous world matrix so it doesnt effect the other non-static fonts
		float* prev_pos = Effect::GetPrevWorldPos();
		Effect::SetWorldPosition(&m_staticityTrans.GetWOTrans().m[0][0]);
		GraphicsComponent::Draw(withTex);
		Effect::SetWorldPosition(prev_pos);
	}
	else
		GraphicsComponent::Draw(withTex);
}

void FontGraphicsComponent::SetScale(Vector3f scale)
{
	m_modelMat.SetScale(scale);
}

Vector3f FontGraphicsComponent::GetScale()
{
	return m_modelMat.GetScale();
}


void FontGraphicsComponent::SetStatic(bool sttatic)
{
	m_static = sttatic;
}
