#include "font.h"

Font::Font(std::string path) : m_texture(path), m_mesh(Mesh(256)), m_phys(PhysicsComponent(Vector3f(), "TEXT"))
{
	int bitmapWidth = 16;
	m_width = 0.5f;
	m_height = 0.5f;

	m_phys.Update();

	CreateHash();
	ResourceManager::GetInstance().LoadTexture(path);
}

Font::~Font()
{
	delete m_graphics;
}

void Font::ChangeLetter(unsigned int index, char newChar)
{
	//The verts per letter are by groups of 4
	index *= 4;

	//The texture coords (they need the default values to register properly in the texAtlas func)
	std::vector<Vector2f> vecs = {
	Vector2f(0, 0),
	Vector2f(1, 0),
	Vector2f(0, 1),
	Vector2f(1, 1)
	};

	for (int i = 0; i < 4; i++)
	{
		//Let the texture atlas figure out what the tex coords should be
		m_mesh.GetAtlas()->TextureIndexToCoord(CharToCode(newChar), vecs.at(i).x, vecs.at(i).y);
		//Update them in the vertices for the mesh and graphics (cuz y not)
		m_mesh.GetMeshVertices()->at(index + i).tex = vecs.at(i);
		m_graphics->GetVertices()->at(index + i).tex = vecs.at(i);
	}
	//Make sure the component updates its stuff
	m_graphics->ResetVBO();
}


void Font::SetupMesh()
{
	m_mesh.Reset();

	for (int i = 0; i < m_message.size(); i++)
	{
		char c = m_message.at(i);
		unsigned int index = CharToCode(c);
		Transformation trans = Transformation();
		//trans.SetScale(Vector3f(m_width, m_height, 1.0f));
		trans.SetTranslation(Vector3f());

		//Change position, and then change texture coords
		Vector3f pos = Vector3f();
		pos.x += m_basePosition.x + i;
		m_mesh.AddToMesh(m_phys.GetVertices(), m_phys.GetIndices(), m_phys.GetHighestIndex(), pos, m_texture, &trans, index);
	}

	delete m_graphics;
	m_graphics = new GraphicsComponent(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices(), m_texture);
	m_graphics->SetPhysics(m_phys.Position(), Vector3f());

	if (m_graphics->GetModels()->size() == 0)
	{
		for (auto x : m_message)
		{
			Vector3f pos = Vector3f();
			m_graphics->GetModels()->insert(m_graphics->GetModels()->end(), 4, pos);
		}
	}
}

void Font::CreateHash() {
	std::string charList =
		"\n !\"#$%&'()*+,-."
		"/0123456789:;<=>"
		"?@ABCDEFGHIJKLMN"
		"OPQRSTUVWXYZ[\\]^"
		"_`abcdefghijklmn"
		"opqrstuvwxyz{|}~";

	for (int i = 0; i < charList.length(); i++)
		m_letters.emplace(charList.at(i), i);
}

void Font::SetText(std::string text, Vector3f location, bool centered)
{
	m_message = text;

	m_basePosition.x = location.x;
	m_basePosition.y = location.y;

	m_phys.SetPosition(location);

	SetupMesh();
}

void Font::Draw()
{
	m_graphics->Draw();
}


//Returns the index of the letter
unsigned int Font::CharToCode(char c)
{
	return m_letters.at(c/* - 32*/);
}
