#include "tile.h"

Tile::Tile(Vector3f pos, std::string modelName, std::string texPath, Vector3f bbsize, Vector3f centerSize) : Drawable(pos, modelName, texPath) 
{
	SetProperties();
	m_size = bbsize;
	m_normalSize = centerSize;
	SetBoundingBox();
}

void Tile::Draw()
{
	if (m_modelName == "NONE")
		return;

	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);

	Drawable::Draw();
}

void Tile::Update()
{
	Drawable::Update();

	if (m_velocity != 0 || m_models.size() == 0)
	{
		m_models = std::vector<Vector3f>();
		m_models.insert(m_models.end(), 4, m_pos);
	}
}

void Tile::DrawShadowMap(Transformation& p)
{
	if (m_modelName == "NONE")
		return;

	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);
	//ShadowMapEffect::GetInstance().Enable();
	//p.SetTranslation(Position());
	//ShadowMapEffect::GetInstance().SetWorld(p.GetTrans());
	//ShadowMapEffect::GetInstance().SetModel(m_modelMat.GetWorldTrans());

	Drawable::Draw();
}

void Tile::SetProperties()
{
	std::string name = m_modelName;
	if (name.find("SLOPE_E") < 150)//returns the first character of the substring, returns largest possible size_t if not, so gotta make sure its not too big
		AddProp(Ent_Props::P_Slope_High_E);
}