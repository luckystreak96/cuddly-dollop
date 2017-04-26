#include "map.h"

#define COMPOSITION "TILE_CUBE"

Map::Map() : m_mesh(Mesh()), Drawable(Vector3f(0, 0, 5), std::string(COMPOSITION))
{
	mustCollide = false;

	for (int x = 0; x < m_tiles.CountX(); x++)
		for (int y = 0; y < m_tiles.CountY(); y++)
		{
			m_tiles.Set(x, y, new Tile(Vector3f(x, y, 5.f), COMPOSITION));
		}

	SetupMesh();
}

Map::~Map()
{
}

Map::Map(const std::string& filePath)
{
}

void Map::SetupMesh()
{
	m_mesh.Reset();
	for (int x = 0; x < m_tiles.CountX(); x++)
		for (int y = 0; y < m_tiles.CountY(); y++)
		{
			Tile* temp = m_tiles.Get(x, y);
			if (temp->GetName() != "NONE")
			{
				m_mesh.AddToMesh(temp->GetVertices(), temp->GetIndices(), temp->GetHighestIndex(), temp->Position(), temp->GetTexture());
				temp->mustDraw = false;
			}
		}

	m_texture = "res/map01.png";
	m_mesh.Finalize(m_texture);
	m_vertices = m_mesh.GetMeshVertices();
	m_indices = m_mesh.GetMeshIndices();
	//SetBuffers();
}

void Map::Update(double elapsedTime)
{
	for (int x = 0; x < m_tiles.CountX(); x++)
		for (int y = 0; y < m_tiles.CountY(); y++)
			m_tiles.Get(x, y)->Update();

	Drawable::Update();
}

void Map::Draw()
{
	
	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);

	Drawable::Draw();
}

void Map::DrawShadowMap(Transformation& p)
{
	ShadowMapEffect::GetInstance().Enable();
	p.SetTranslation(Position());
	ShadowMapEffect::GetInstance().SetWorld(p.GetTrans());
	ShadowMapEffect::GetInstance().SetModel(m_modelMat.GetWorldTrans());

	Drawable::Draw();
}

Array2d<Tile*> Map::Tiles()
{
	return m_tiles;
}

unsigned int Map::Size()
{
	return m_tiles.Count();
}