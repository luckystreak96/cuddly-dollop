#include "map_handler.h"

#define COMPOSITION "TILE"

MapHandler::MapHandler() : m_mesh(Mesh()), m_id(1)
{
	srand(clock());

	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(4, 3, 4.0f), COMPOSITION, "res/pimkin.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(5, 3, 4.0f), COMPOSITION, "res/pumpkin.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(2, 4, 4), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(2, 3, 4.5f), COMPOSITION, "res/yellowGrass.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(3, 4, 3.5f), COMPOSITION, "res/bridge.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(4, 4, 3.0f), COMPOSITION, "res/bridge.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(5, 4, 3.0f), COMPOSITION, "res/bridge.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(6, 4, 3.5f), COMPOSITION, "res/bridge.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(7, 4, 4), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(8, 4, 3.5f), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(9, 4, 3.0f), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(10, 4, 2.5f), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(11, 4, 2.0f), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(12, 4, 1.5f), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(13, 4, 1.0f), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(14, 4, 0.5f), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(7, 5, 4), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(7, 6, 4), COMPOSITION, "res/platform.png")));
	m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f(7, 3, 4.5f), COMPOSITION, "res/yellowGrass.png")));

	//m_tiles.push_back(new Tile(Vector3f(2, 4, 4), COMPOSITION, "res/pattern.png", Vector3f(1.f, 0.5f, 1.f), Vector3f(1.f, 0.f, 1.f)));


	for (int x = 0; x < 32; x++)
		for (int y = 0; y < 18; y++)
		{
			//m_tiles.push_back(new Tile(Vector3f(x, y, 5), COMPOSITION, "res/sand.png"));
			int num = rand() % 35 + 1;
			num = num > 4 ? 1 : num;
			//m_tiles.push_back(new MapTile(Vector3f((float)x, (float)y, 5.f), COMPOSITION, "res/grass_0" + std::to_string(num) + ".png"));
			//if(num == 4) m_tiles.push_back(new MapTile(Vector3f((float)x, (float)y, 4.f), COMPOSITION, "res/pumpkin.png"));
			m_tiles.push_back(std::shared_ptr<MapTile>(new MapTile(Vector3f((float)x, (float)y, 5.f), COMPOSITION, "res/yellowGrass.png")));
		}

	FinalizeSetup();
}

MapHandler::MapHandler(unsigned int id, std::shared_ptr<JsonHandler> jh) : m_mesh(Mesh()), m_id(id), m_jsonHandler(jh)
{
	auto& map = m_jsonHandler->LoadMap(id);
	if (!map.HasMember("tiles"))
		return;
	for (auto& x : map["tiles"].GetArray())
	{
		Vector3f pos;
		std::string sprite;
		pos.x = x["x"].GetFloat();
		pos.y = x["y"].GetFloat();
		pos.z = x["z"].GetFloat();
		sprite = std::string(x["sprite"].GetString());
		std::shared_ptr<MapTile> tile = std::shared_ptr<MapTile>(new MapTile(pos, COMPOSITION, sprite));
		if (x.HasMember("walkOn") && x["walkOn"].GetBool() == false)
			tile->Physics()->walkOn = false;
		m_tiles.push_back(tile);
	}
	m_OrderedTiles = std::vector<std::shared_ptr<MapTile>>(m_tiles);
	FinalizeSetup();
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
		m_mesh.AddToMesh(t->Physics()->GetVertices(), t->Physics()->GetIndices(), t->Physics()->GetHighestIndex(), t->Physics()->Position(), t->GetTexture());

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
	if (m_graphics->GetMModels().size() == 0 || forced)
	{
		m_graphics->GetMModels().clear();
		for (auto x : m_tiles)
		{
			Vector3f pos = x->Physics()->Position();
			Transformation t;
			t.SetTranslation(pos);
			m_graphics->InsertMModels(t);
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

std::vector<std::shared_ptr<MapTile>>* MapHandler::OrderedTiles()
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
