#include "fontGraphicsComponent.h"

Transformation FontGraphicsComponent::m_staticityTrans = Transformation();

FontGraphicsComponent::FontGraphicsComponent(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string texPath) : GraphicsComponent(verts, inds, texPath)
{
	
}

void FontGraphicsComponent::Draw(bool withTex)
{
	if (m_static)
	{
		//This gets called every time, maybe should be changed?
		m_staticityTrans.SetOrthoProj(&OrthoProjInfo::GetRegularInstance());
		m_staticityTrans.SetTranslation(OrthoProjInfo::GetRegularInstance().Left, OrthoProjInfo::GetRegularInstance().Bottom, 0);
		Effect::SetWorldPosition(&m_staticityTrans.GetWOTrans().m[0][0]);
	}
	GraphicsComponent::Draw(withTex);
}

void FontGraphicsComponent::SetScale(Vector3f scale)
{
	m_modelMat.SetScale(scale);
}


void FontGraphicsComponent::SetStatic(bool sttatic)
{
	m_static = sttatic;
}