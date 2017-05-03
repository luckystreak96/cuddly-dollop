#include "map.h"

#define COMPOSITION "TILE"

Map::Map() : m_mesh(Mesh()), Drawable(Vector3f(0, 0, 0), std::string(COMPOSITION))
{
	mustCollide = false;

	for (int x = 0; x < 32; x++)
		for (int y = 0; y < 18; y++)
		{
			m_tiles.push_back(new Tile(Vector3f(x, y, 5), COMPOSITION, "res/sand.png"));
			//int num = rand() % 20 + 1;
			//num = num > 4 ? 1 : num;
			//m_tiles.push_back(new Tile(Vector3f(x, y, 5), COMPOSITION, "res/grass_0" + std::to_string(num) + ".png"));
		}

	m_tiles.push_back(new Tile(Vector3f(2, 4, 4), COMPOSITION, "res/orb.png"));

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
	std::sort(m_tiles.begin(), m_tiles.end(), Drawable::SortFunc);
	for(auto t : m_tiles)
	{
		Drawable* temp = t;
		m_mesh.AddToMesh(temp->GetVertices(), temp->GetIndices(), temp->GetHighestIndex(), temp->Position(), temp->GetTexture());
		temp->mustDraw = false;
	}

	m_texture = "res/map01.png";
	m_mesh.Finalize(m_texture);
	m_vertices = m_mesh.GetMeshVertices();
	m_indices = m_mesh.GetMeshIndices();
}

void Map::Update(double elapsedTime)
{
	for(auto d : m_tiles)
		d->Update();

	Drawable::Update();
}

void Map::Draw()
{
	//Set to 0 so the mesh doesnt move all the objects as well.
	//This should only be modified if the whole mesh is moving, 
	//otherwise each individual object inside it has its own coords set in the vertices.
	m_modelMat.SetTranslation(Vector3f(0, 0, 0));
	
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

std::vector<Drawable*> Map::Tiles()
{
	return m_tiles;
}

unsigned int Map::Size()
{
	return m_tiles.size();
}