#include "map.h"

#define COMPOSITION "TILE_CUBE"

Map::Map() : m_mesh(Mesh()), Drawable(Vector3f(0, 0 ,0), std::string(COMPOSITION))
{
	mustCollide = false;

	for (int x = 0; x < m_tiles.GetXCount(); x++)
		for (int y = 0; y < m_tiles.GetYCount(); y++)
			for (int z = 0; z < m_tiles.GetZCount(); z++)
			{
				if (z == 0)
				{
					m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), COMPOSITION));
					if (x < 2)
						m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), COMPOSITION));
					//else if(x == 2)
					//	m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), "NONE"));
					else
						m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), COMPOSITION));
					//if(x == 1 && y == 3)
					//	m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), "NONE"));
				}
				else if (z == 1)
				{
					if (x > 2)
					{
						m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), COMPOSITION));
					}
					else if (x == 2)
					{
						m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), COMPOSITION));
					}
					else
					{
						m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), "NONE"));
					}

				}
				else if (z == 2 && x == 4  && y == 3)
					m_tiles.Set(x, y, z, new Tile(Vector3f(6, 0, -1), "TILE_TEST"));
				else
					m_tiles.Set(x, y, z, new Tile(Vector3f(x, y, -z), "NONE"));
			}
	for (int i = 1; i < 5; i++)
	{
		//if(i != 4)
		//	m_tiles.Set(3, i, 0, new Tile(Vector3f(0.2f * i + 1, 1, -0.2f * i), "TILE_THICK"));
		//else
			m_tiles.Set(3, i, 0, new Tile(Vector3f(i, 3, -i), "TILE_SLOPE_E", "res/pattern.png"));
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
	for (int x = 0; x < m_tiles.GetXCount(); x++)
		for (int y = 0; y < m_tiles.GetYCount(); y++)
			for (int z = 0; z < m_tiles.GetZCount(); z++)
			{
				Tile* temp = m_tiles.Get(x, y, z);
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
	for (int x = 0; x < m_tiles.GetXCount(); x++)
		for (int y = 0; y < m_tiles.GetYCount(); y++)
			for (int z = 0; z < m_tiles.GetZCount(); z++)
				m_tiles.Get(x, y, z)->Update();

	Drawable::Update();
}

void Map::Draw()
{
	PointLightShadowEffect::GetInstance().Enable();
	PointLightShadowEffect::GetInstance().SetModelPosition(&m_modelMat.GetWorldTrans().m[0][0]);

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

Array3d<Tile*> Map::Tiles()
{
	return m_tiles;
}

unsigned int Map::Size()
{
	return m_tiles.Count();
}