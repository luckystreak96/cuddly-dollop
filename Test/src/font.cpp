#include "font.h"

Font::Font(bool sTatic, bool temporary, std::string path) : m_texture(path), m_phys(PhysicsComponent(Vector3f(), "TEXT")),
m_elapsedTime(0), m_textSpeed(1.0), m_timePerLetter(0.03), m_static(sTatic), m_temporary(temporary), m_lifetime(5.0), LetterSpacing(1.0f), MaxTime(30000),
m_lettersPerRow(16), m_lettersPerColumn(16), m_xScale(1.0f)
{
	m_mesh = Mesh(m_lettersPerRow * m_lettersPerColumn);
	int bitmapWidth = 16;

	m_phys.Update();

	CreateHash();
	ResourceManager::GetInstance().LoadTexture(path);

	SetText(" ");
}

Font::~Font()
{
	delete m_graphics;
}

bool Font::IsDead()
{
	return m_temporary && m_elapsedTime > m_lifetime;
}

void Font::Update(double elapsedTime)
{
	if (m_elapsedTime < MaxTime)
		m_elapsedTime += elapsedTime * m_textSpeed;

	for (unsigned int i = 0; i < m_message.size(); i++)
	{
		if (m_message.at(i) != m_messageProgress.at(i) && m_elapsedTime > m_timePerLetter * i)
		{
			m_messageProgress.at(i) = m_message.at(i);
			ChangeLetter(i, m_message.at(i));
		}
	}
}

bool Font::TextDisplayDone()
{
	return m_elapsedTime > m_timePerLetter * (m_message.size() - 1);
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
		if (m_mesh.GetMeshVertices()->size() > index + i)
		{
			m_mesh.GetMeshVertices()->at(index + i).tex = vecs.at(i);
			m_graphics->GetVertices()->at(index + i).tex = vecs.at(i);
		}
	}
	//Make sure the component updates its stuff
	m_graphics->ResetVBO();
}



template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}


void Font::SetupMesh(float xBndry, float yBndry)
{
	m_mesh.Reset();

	// Split the text into words for op word wrap
	std::vector<std::string> temp = split(m_message, ' ');
	std::vector<std::string> words = std::vector<std::string>();//split(m_message, ' ');
	//std::vector<std::string> words = std::vector<std::string>();
	for (auto x : temp)
	{
		if (x.find('\n') != std::string::npos)
		{
			// Number of backslash ens
			int numBSN = std::count(x.begin(), x.end(), '\n');
			auto s = split(x, '\n');
			for (auto b : s)
			{
				if (b == "")
					b = "\n";

				words.push_back(b);

				if (b != "\n")
					words.push_back("\n");
			}
			if (numBSN >= s.size())
				words.push_back("\n");
		}
		else
		{
			words.push_back(x);
		}
	}

	// Letter position
	float x = 0;
	float y = 0;

	unsigned int progress = 0;
	std::string newmessage = "";
	// Look at a word
	for (auto w : words)
	{
		if (w == "\n")
		{
			y -= 1;
			x = 0;
		}
		// Is the word too big to fit in the first place?
		else if (w.size() * LetterSpacing > xBndry)
		{
			// TODO: Letter me wrap plz
			AddWordToMesh(w + " ", x, y);
			newmessage += w + " ";
		}
		else if (w.size() * LetterSpacing * m_xScale + x > xBndry / m_xScale)
		{
			// Increment y and start on new line
			y -= 1;
			x = 0;

			AddWordToMesh(w + " ", x, y);
			newmessage += w + " ";
			x += (w.size() + 1) * LetterSpacing * m_xScale;
		}
		else
		{
			AddWordToMesh(w + " ", x, y);
			newmessage += w + " ";
			x += (w.size() + 1) * LetterSpacing * m_xScale;
		}
	}

	m_message = newmessage;
	m_messageProgress = "";
	for (auto x : m_message)
		m_messageProgress += ' ';

	//delete m_graphics;
	if (m_graphics == NULL)
		m_graphics = new FontGraphicsComponent(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices(), m_texture);
	m_graphics->FullReset(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices());
	m_graphics->SetPhysics(m_phys.Position(), Vector3f());
	m_graphics->SetStatic(m_static);

	m_graphics->GetModels()->clear();
	for (auto x : m_message)
	{
		Vector3f pos = Vector3f();
		m_graphics->GetModels()->insert(m_graphics->GetModels()->end(), 4, pos);
	}
}

void Font::AddWordToMesh(std::string word, float x, float y)
{
	for (int i = 0; i < (int)word.size(); i++)
	{
		char c = i >= m_messageProgress.size() ? ' ' : m_messageProgress.at(i);
		unsigned int index = CharToCode(c);
		Transformation trans = Transformation();
		trans.SetTranslation(Vector3f());

		//Change position, and then change texture coords
		Vector3f pos = Vector3f();

		pos.x = i * LetterSpacing * m_xScale + x;
		pos.y = y;

		m_mesh.AddToMesh(m_phys.GetVertices(), m_phys.GetIndices(), m_phys.GetHighestIndex(), pos, m_texture, &trans, index);
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

	for (unsigned int i = 0; i < charList.length(); i++)
		m_letters.emplace(charList.at(i), i);
}

void Font::SetText(std::string text, Vector3f location, bool centered, float xBoundry)
{
	m_message = text;
	m_totalTime = text.size() * m_timePerLetter;
	m_lifetime = 2 + text.size() * 0.1;
	m_messageProgress = "";
	for (auto x : m_message)
		m_messageProgress += ' ';

	float value = 0;
	if (centered)
		value = (((float)m_message.size() / 2.0f) * LetterSpacing * m_xScale);

	location.x -= value;

	m_basePosition.x = location.x/* - value*/;
	m_basePosition.y = location.y;

	m_phys.SetPosition(location);

	SetupMesh(xBoundry);
}

void Font::Draw()
{
	m_graphics->Draw();
}

void Font::SetTextSpeed(double speed)
{
	m_textSpeed = speed;
}


//Returns the index of the letter
unsigned int Font::CharToCode(char c)
{
	return m_letters.at(c/* - 32*/);
}

void Font::SetRender()
{
	if (m_graphics != NULL)
		Renderer::GetInstance().Add(m_graphics);
}

void Font::SetScale(float xScale, float yScale)
{
	if (m_graphics != NULL)
		m_graphics->SetScale(Vector3f(xScale, yScale, 1));

	m_xScale = xScale;
	m_yScale = yScale;
}
