#include "font.h"
#include "utils.h"
#include "gameData.h"

Font::Font(bool sTatic, bool temporary, bool lightSpeed, std::string path) : m_texture(path), m_phys(PhysicsComponent(Vector3f(), "TEXT")),
m_elapsedTime(0), m_textSpeed(1.0), m_timePerLetter(0.03), m_static(sTatic), m_temporary(temporary), m_lifetime(5.0), _letterSpacing(1.0f), MaxTime(30000),
m_lettersPerRow(16), m_lettersPerColumn(16), m_xScale(1.0f), m_yScale(1.0f), m_lightSpeed(lightSpeed), _enabled(true), m_centered(false), m_xBndry(-1), m_x(0), m_y(0)
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
}

bool Font::IsDead()
{
	return m_temporary && m_elapsedTime > m_lifetime;
}

void Font::Update(double elapsedTime)
{
	// Lightspeed has been moved to the following for loop so elasedTime doesnt fuck up lifetime
	//For those who hate to wait for the text to draw
	//if (m_lightSpeed)
	//	elapsedTime = MaxTime;
	if (OrthoProjInfo::GetRegularInstance().changed)
		UpdateModel();

	if (m_elapsedTime < MaxTime)
		m_elapsedTime += elapsedTime * m_textSpeed;

	for (unsigned int i = 0; i < m_message.size(); i++)
	{
		if (m_message.at(i) != m_messageProgress.at(i) && (m_lightSpeed || m_elapsedTime > m_timePerLetter * i))
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


void Font::ChangeLetter(unsigned int index, uint32_t newChar)
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

void Font::SetupMesh(float xBndry, float yBndry)
{
	m_mesh.Reset();

	// Split the text into words for op word wrap
	std::vector<std::string> temp = Utils::Split(_text, ' ');
	std::vector<std::string> words = std::vector<std::string>();//split(m_message, ' ');

	// Iterate through the words
	for (auto x : temp)
	{
		// If there's any \n's in the text
		if (x.find('\n') != std::string::npos)
		{
			// Number of backslash ens
			int numBSN = std::count(x.begin(), x.end(), '\n');

			// Little hack to find out how many \n's and where they go
			auto s = Utils::Split(x, '\n');
			for (auto b : s)
			{
				// An empty string indicates \n at the start of the word
				if (b == "")
					b = "\n";

				// Add the word itself
				words.push_back(b);

				// If the string isn't \n, then it had a \n at the end, so add it here
				if (b != "\n")
				{
					// If the size is 2, then 2 words are seperated only by a \n and the second word shouldnt get a \n after it
					if (s.size() - 1 == numBSN && b == s.at(s.size() - 1))
						break;

					words.push_back("\n");
				}
			}

			// If the \n count was larger than the
			if ((size_t)numBSN > s.size())
				words.push_back("\n");
		}
		else
		{
			// No \n in the word, so just add the word
			words.push_back(x);
		}
	}

	// Letter position
	float offset;
	if (xBndry == -1)
		offset = 0.f;
	else
		offset = m_yScale;
	m_x = offset;
	m_y = -offset;

	unsigned int progress = 0;
	// This string is message but without \n's in it because
	//  for display purposes, the \n is fucking with the letter count
	std::string newmessage = "";

	m_letterPositions.clear();

	// Look at a word
	for (auto w : words)
	{
		// If the word is a \n, just start a new line
		if (w == "\n")
		{
			m_y -= m_yScale;
			m_x = offset;
		}
		// Is the word too big to fit in the first place?
		else if (w.size() * _letterSpacing > xBndry)
		{
			// TODO: Letter me wrap plz
			AddWordToMesh(w + " ", m_x, m_y);
			newmessage += w + " ";
			m_x += (w.size() + 1) * _letterSpacing * m_xScale;
		}
		else if (w.size() * _letterSpacing * m_xScale + m_x > xBndry)
		{
			// Increment y and start on new line
			m_y -= m_yScale;
			m_x = offset;

			AddWordToMesh(w + " ", m_x, m_y);
			newmessage += w + " ";
			m_x += (w.size() + 1) * _letterSpacing * m_xScale;
		}
		else
		{
			AddWordToMesh(w + " ", m_x, m_y);
			newmessage += w + " ";
			m_x += (w.size() + 1) * _letterSpacing * m_xScale;
		}
	}

	// To accomadate choices that might be drawn after
	//m_y -= m_yScale;


	// Set message to the non-\n version
	_text = newmessage;
	m_message = Utils::ConvertUTF8(newmessage);

	// Reset messageprogress so we dont have \n's
	for (auto x : m_message)
		m_messageProgress.push_back(' ');

	//delete m_graphics;
	if (m_graphics == NULL)
		m_graphics = std::shared_ptr<FontGraphicsComponent>(new FontGraphicsComponent(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices(), m_texture));
	//m_graphics->FullReset(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices());
	m_graphics->SetNewBuffers(m_mesh.GetMeshVertices(), m_mesh.GetMeshIndices());
	m_graphics->SetPhysics(m_phys.Position(), Vector3f());
	m_graphics->SetStatic(m_static);

	UpdateModel();
}

void Font::AddWordToMesh(std::string word, float x, float y)
{
	for (unsigned int i = 0; i < (int)word.size(); i++)
	{
		uint32_t c = i >= m_messageProgress.size() ? ' ' : m_messageProgress.at(i);
		unsigned int index = CharToCode(c);

		//Change position, and then change texture coords
		Vector3f pos = Vector3f();

		pos.x = i * _letterSpacing * m_xScale + x;
		pos.y = y;

		m_letterPositions.push_back(pos + m_phys.Position());

		m_mesh.AddToMesh(m_phys.GetVerticesRef(), m_phys.GetIndicesRef(), m_phys.GetHighestIndex(), pos, m_texture, index);
	}
}


void Font::CreateHash() {
	std::string charList =
		"\n !\"#$%&'()*+,-."
		"/0123456789:;<=>"
		"?@ABCDEFGHIJKLMN"
		"OPQRSTUVWXYZ[\\]^"
		"_`abcdefghijklmn"
		"opqrstuvwxyz{|}~"
		u8"\u001b�";

	std::vector<uint32_t> list = Utils::ConvertUTF8(charList);

	for (unsigned int i = 0; i < list.size(); i++)
		m_letters.emplace(list.at(i), i);
}

void Font::Reset()
{
	//m_graphics = NULL;
	SetText(_text, m_basePosition, m_centered, m_xBndry);
}

void Font::SetText(std::string text, Vector3f location, bool centered, float xBoundry)
{
	if (text == _text/* && m_graphics*/)
		return;
	m_xBndry = xBoundry;
	m_centered = centered;
	m_elapsedTime = 0;
	_text = text;

	// Fill in variables if necessary
	SetTextVariables();

	m_totalTime = text.size() * m_timePerLetter;
	//m_lifetime = 2 + text.size() * 0.1;
	m_messageProgress.clear();
	for (auto x : m_message)
		m_messageProgress.push_back(' ');

	float value = 0;
	if (centered)
		value = (((float)_text.size() / 2.0f) * _letterSpacing * m_xScale);

	location.x -= value;

	m_basePosition.x = location.x/* - value*/;
	m_basePosition.y = location.y;

	m_phys.SetPosition(location);

	SetupMesh(xBoundry);
}

void Font::SetTextVariables()
{
	int counter = 0;
	int location;
	int beginning;
	int end;
	std::string temp = _text;
	while ((location = temp.find("%")) != std::string::npos)
	{
		counter++;
		// replace these contents
		if (counter % 2 == 0)
		{
			std::string result = temp.substr(0, location);
			end = location + beginning + 2;
			std::string var_name = result;//m_message.substr(beginning + 1, (end - 1) - beginning);
			// If theres no illegal character, replace...
			if (!(var_name.find(' ') != std::string::npos || var_name.find('\n') != std::string::npos))
				_text.replace(beginning, end - beginning, GameData::Get(var_name));

			temp = _text;
			// ...else do nothing, it was probably a regular % in the text
		}
		else
		{
			temp = temp.substr(location + 1, _text.size() - location);
			beginning = location;
		}
	}
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
inline unsigned int Font::CharToCode(uint32_t c)
{
	if (m_letters.count(c))
		return m_letters.at(c);
	else
		return 96;
}

void Font::SetRender()
{
	if (m_graphics != NULL)
		Renderer::GetInstance().Add(m_graphics);
}

void Font::SetScale(float xScale, float yScale)
{
	m_xScale = xScale;
	m_yScale = yScale;

	UpdateModel();
}

void Font::UpdateModel()
{
	if (m_graphics)
	{
		m_graphics->ClearMModels();
		for (auto x : m_letterPositions)
		{
			Transformation t;
			t.SetTranslation(x);
			t.SetScale(Vector3f(m_xScale, m_yScale, 1));
			m_graphics->InsertMModels(t);
		}
	}
}
