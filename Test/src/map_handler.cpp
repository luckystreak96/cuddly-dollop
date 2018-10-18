#include "map_handler.h"

#define COMPOSITION "TILE"

int MapHandler::m_chunkSize = 3;

MapHandler::MapHandler(unsigned int id, std::shared_ptr<JsonHandler> jh) : m_mesh(Mesh()), m_id(id), m_jsonHandler(jh)
{
	auto map = m_jsonHandler->LoadMap(id);

	if (!map.HasMember("tiles"))
		return;

	for (auto& x : map["tiles"].GetArray())
	{
		Vector3f pos;
		pos.x = x["x"].GetFloat();
		pos.y = x["y"].GetFloat();
		pos.z = x["z"].GetFloat();
		std::string sprite;
		sprite = std::string(x["sprite"].GetString());

		std::shared_ptr<MapTile> tile = std::shared_ptr<MapTile>(new MapTile(pos, COMPOSITION, sprite));
		if (x.HasMember("walkOn") && x["walkOn"].GetBool() == false)
			tile->Physics()->walkOn = false;
		if (x.HasMember("deco") && x["deco"].GetBool() == true)
			tile->Physics()->SetEthereal(true);

		tile->PhysicsRaw()->_unmoving = true;

		m_tiles.push_back(tile);
	}

	FinalizeSetup();
	OrderTiles();
}

void MapHandler::OrderTiles()
{
	int maxX = m_mapSize.x / m_chunkSize + 1;
	int maxY = m_mapSize.y / m_chunkSize + 1;
	for (int i = 0; i < maxX; i++)
	{
		m_OrderedTiles.push_back(std::vector<std::vector<MapTile*>>());
		for (int y = 0; y < maxY; y++)
		{
			m_OrderedTiles[i].push_back(std::vector<MapTile*>());
		}
	}

	for (auto& t : m_tiles)
	{
		int x = t->PhysicsRaw()->PositionRef().x / m_chunkSize;
		int y = t->PhysicsRaw()->PositionRef().y / m_chunkSize;
		m_OrderedTiles[x][y].push_back(&*t);
	}
}

void MapHandler::FinalizeSetup()
{
	std::sort(m_tiles.begin(), m_tiles.end(), &TileSort);

	SetupMesh();

	int x = 0;
	int y = 0;
	int z = 0;
	for (auto t : m_tiles)
	{
		t->Physics()->Update();
		int tx = (int)t->Physics()->Position().x;
		if (tx > x)
			x = tx;
		int ty = (int)t->Physics()->Position().y;
		if (ty > y)
			y = ty;
		int tz = (int)t->Physics()->Position().z;
		if (tz > z)
			z = tz;
	}

	// +1 to add the size of the tile as well
	m_mapSize = Vector3f((float)x + 1.f, (float)y + 1.f, (float)z);
}

MapHandler::~MapHandler()
{
}

MapHandler::MapHandler(const std::string& filePath)
{
	//implement me!
}

void MapHandler::SetupMesh()
{
	m_mesh.Reset();
	//std::sort(m_tiles.begin(), m_tiles.end(), MapTile::SortFunc);
	for (auto t : m_tiles)
	{
		std::vector<Vertex> verts = t->Physics()->GetVerticesRef();
		m_mesh.AddToMesh(verts, t->Physics()->GetIndices(), t->Physics()->GetHighestIndex(), t->Physics()->Position(), t->GetTexture());
	}

	m_MBO_instances = m_tiles.size();

	m_texture = "res/tiles.png";
	//m_mesh.Finalize(m_texture);
	m_graphics = GraphComp_ptr(new GraphicsComponent(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices(), m_texture));
	m_graphics->SetPhysics(Vector3f(0, 0, 20.0f), Vector3f());
	//m_graphics->_instancedDraw = true;
}

void MapHandler::Update(bool forced)
{
	//INEFFICIENT BECAUSE THE MATRIX NEEDS TO BE SENT 4 TIMES - 1 PER VERTEX
	//THE PURPOSE OF THIS IS SO THAT THE VERTEX SHADER CAN KNOW THE Y COORD OF THE BITCH AND CHANGE
	//	THE Z ACCORDINGLY. THIS IS DONE THIS WAY BECAUSE OF MESHES, THEY CALCULATE POS AHEAD OF
	//	TIME AND CHANGE THE VERTEX POSITIONS -- BUT NOT ANY OTHER OBJECT. TO KEEP THE Z CONSISTENT,
	//	I USE THE MATRIX INSTEAD OF MULTIPLYING THE OTHER OBJECTS VERTEX POS BY THEIR POS.
	bool firstTime = m_graphics->GetMModels().size() == 0;
	if (m_graphics->GetMModels().size() == 0 || forced)
	{
		int i = 0;
		m_graphics->ClearMModels();
		for (auto x : m_tiles)
		{
			Vector3f pos = x->Physics()->Position();
			if (!Transformation::perspectiveOrtho)
			{
				// increase z by 1 to make people touch the tiles
				pos.z -= 1;
			}
			Transformation t;
			t.SetTranslation(pos);
			std::string texture = x->GetTexture();
			AdjustSprite(texture, t, i, firstTime);
			m_graphics->InsertMModels(t);
			i += 4;
		}
		m_graphics->ResetVBO();
	}
}

void MapHandler::AdjustSprite(std::string sprite, Transformation& t, int index, bool firstTime)
{
	if (sprite == "sapin_b.png")
	{
		t.SetRotation(GraphicsComponent::GetProjectionRotation(), 0, 0);
		t.SetScale(Vector3f(1, 2, 1));
		if (firstTime)
		{
			float y = m_graphics->GetVertices()->at(index + 2).tex.y;
			float increase = TextureAtlas::m_textureAtlas.GetTexCoordWH().y;
			m_graphics->GetVertices()->at(index + 2).tex.y += TextureAtlas::m_textureAtlas.GetTexCoordWH().y;
			m_graphics->GetVertices()->at(index + 3).tex.y += TextureAtlas::m_textureAtlas.GetTexCoordWH().y;
		}
	}
}

void MapHandler::SetRender()
{
	Renderer::GetInstance().Add(m_graphics);
}

void MapHandler::Draw()
{
	//Set to 0 so the mesh doesnt move all the objects as well.
	//This should only be modified if the whole mesh is moving, 
	//otherwise each individual object inside it has its own coords set in the vertices.
	m_graphics->GetModelMat()->SetTranslation(Vector3f(0, 0, 0));

	m_graphics->Draw();
}

std::vector<std::shared_ptr<MapTile>>* MapHandler::Tiles()
{
	return &m_tiles;
}

std::vector<std::vector<std::vector<MapTile*>>>* MapHandler::OrderedTiles()
{
	return &m_OrderedTiles;
}

unsigned int MapHandler::Size()
{
	return (unsigned int)m_tiles.size();
}

Vector3f MapHandler::GetMapSize()
{
	return m_mapSize;
}

bool MapHandler::TileSort(std::shared_ptr<MapTile> i, std::shared_ptr<MapTile> j)
{
	return (i->Physics()->Position() < j->Physics()->Position());
}

bool MapHandler::TileSortX(std::shared_ptr<MapTile> i, std::shared_ptr<MapTile> j)
{
	Vector3f r = j->Physics()->Position();
	float x = i->Physics()->Position().x;
	float y = i->Physics()->Position().y;
	float z = i->Physics()->Position().z;
	if (x == r.x)
	{
		if (y == r.y)
		{
			if (z == r.z)
			{

			}
			else
			{
				return z < r.z;
			}
		}
		else
		{
			return y < r.y;
		}
	}
	else
	{
		return x < r.x;
	}

	return false;
}
