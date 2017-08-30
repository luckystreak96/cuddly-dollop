#include "map_handler.h"

#define COMPOSITION "TILE"

MapHandler::MapHandler() : m_mesh(Mesh())
{
	m_tiles.push_back(new MapTile(Vector3f(10, 10, 4.5f), COMPOSITION, "res/pattern.png"));
	m_tiles.push_back(new MapTile(Vector3f(2, 4, 4), COMPOSITION, "res/pattern.png"));
	m_tiles.push_back(new MapTile(Vector3f(2, 3, 4.5f), COMPOSITION, "res/grass_01.png"));
	m_tiles.push_back(new MapTile(Vector3f(3, 4, 3.5), COMPOSITION, "res/bridge.png"));
	m_tiles.push_back(new MapTile(Vector3f(4, 4, 3.5), COMPOSITION, "res/bridge.png"));
	m_tiles.push_back(new MapTile(Vector3f(5, 4, 3.5), COMPOSITION, "res/bridge.png"));
	m_tiles.push_back(new MapTile(Vector3f(6, 4, 3.5), COMPOSITION, "res/bridge.png"));
	m_tiles.push_back(new MapTile(Vector3f(7, 4, 4), COMPOSITION, "res/pattern.png"));
	m_tiles.push_back(new MapTile(Vector3f(7, 5, 4), COMPOSITION, "res/pattern.png"));
	m_tiles.push_back(new MapTile(Vector3f(7, 6, 4), COMPOSITION, "res/pattern.png"));
	m_tiles.push_back(new MapTile(Vector3f(7, 3, 4.5f), COMPOSITION, "res/grass_01.png"));

	//m_tiles.push_back(new Tile(Vector3f(2, 4, 4), COMPOSITION, "res/pattern.png", Vector3f(1.f, 0.5f, 1.f), Vector3f(1.f, 0.f, 1.f)));


	for (int x = 0; x < 32; x++)
		for (int y = 0; y < 18; y++)
		{
			//m_tiles.push_back(new Tile(Vector3f(x, y, 5), COMPOSITION, "res/sand.png"));
			int num = rand() % 20 + 1;
			num = num > 4 ? 1 : num;
			m_tiles.push_back(new MapTile(Vector3f((float)x, (float)y, 5.f), COMPOSITION, "res/grass_0" + std::to_string(num) + ".png"));
		}

	SetupMesh();
	for (auto x : m_tiles)
		x->Physics()->Update();
}

MapHandler::~MapHandler()
{
	for (auto x : m_tiles)
		delete x;
}

MapHandler::MapHandler(const std::string& filePath)
{
	//implement me!
}

void MapHandler::SetupMesh()
{
	m_mesh.Reset();
	std::sort(m_tiles.begin(), m_tiles.end(), MapTile::SortFunc);
	for (auto t : m_tiles)
	{
		MapTile* temp = t;
		m_mesh.AddToMesh(temp->Physics()->GetVertices(), temp->Physics()->GetIndices(), temp->Physics()->GetHighestIndex(), temp->Physics()->Position(), temp->GetTexture(), temp->GetTransformation());
	}

	m_MBO_instances = m_tiles.size();

	m_texture = "res/map01.png";
	m_mesh.Finalize(m_texture);
	m_graphics = new GraphicsComponent(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices(), m_texture);
}

void MapHandler::Update()
{
	//This shit doesnt need an update
	//for (auto d : m_tiles)
	//	d->Update();

	//Drawable::Update();

	//INEFFICIENT BECAUSE THE MATRIX NEEDS TO BE SENT 4 TIMES - 1 PER VERTEX
	//THE PURPOSE OF THIS IS SO THAT THE VERTEX SHADER CAN KNOW THE Y COORD OF THE BITCH AND CHANGE
	//	THE Z ACCORDINGLY. THIS IS DONE THIS WAY BECAUSE OF MESHES, THEY CALCULATE POS AHEAD OF
	//	TIME AND CHANGE THE VERTEX POSITIONS -- BUT NOT ANY OTHER OBJECT. TO KEEP THE Z CONSISTENT,
	//	I USE THE MATRIX INSTEAD OF MULTIPLYING THE OTHER OBJECTS VERTEX POS BY THEIR POS.
	if (m_graphics->GetModels()->size() == 0)
	{
		//m_graphics->GetModels() = new std::vector<Vector3f>();

		for (auto x : m_tiles)
		{
			m_graphics->GetModels()->insert(m_graphics->GetModels()->end(), 4, x->Physics()->Position());
		}
	}

	m_graphics->Update();

	//if (m_models.size() == 0)
	//{
	//	m_models = std::vector<Mat4f>();

	//	for (auto x : m_tiles)
	//		m_models.insert(m_models.end(), 4, x->GetModelMat()->GetWorldTrans());

	//	m_MBO_instances = m_models.size();
	//}
}

void MapHandler::Draw()
{
	//Set to 0 so the mesh doesnt move all the objects as well.
	//This should only be modified if the whole mesh is moving, 
	//otherwise each individual object inside it has its own coords set in the vertices.
	m_graphics->GetModelMat()->SetTranslation(Vector3f(0, 0, 0));

	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&m_graphics->GetModelMat()->GetWorldTrans().m[0][0]);

	m_graphics->Draw();
}

std::vector<MapTile*>* MapHandler::Tiles()
{
	return &m_tiles;
}

unsigned int MapHandler::Size()
{
	return (unsigned int)m_tiles.size();
}