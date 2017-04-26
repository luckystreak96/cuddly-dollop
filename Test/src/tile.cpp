#include "tile.h"

Tile::Tile(Vector3f pos, std::string modelName, std::string texPath) : Drawable(pos, modelName, texPath) 
{
	SetProperties();
}

void Tile::Draw()
{
	if (m_modelName == "NONE")
		return;

	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);
	//PointLightShadowEffect::GetInstance().Enable();
	//PointLightShadowEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);

	Drawable::Draw();
}

void Tile::DrawShadowMap(Transformation& p)
{
	if (m_modelName == "NONE")
		return;

	ShadowMapEffect::GetInstance().Enable();
	p.SetTranslation(Position());
	ShadowMapEffect::GetInstance().SetWorld(p.GetTrans());
	ShadowMapEffect::GetInstance().SetModel(m_modelMat.GetWorldTrans());

	Drawable::Draw();
}

void Tile::SetProperties()
{
	std::string name = m_modelName;
	if (name.find("SLOPE_E") < 150)//returns the first character of the substring, returns largest possible size_t if not, so gotta make sure its not too big
		AddProp(Ent_Props::P_Slope_High_E);
}