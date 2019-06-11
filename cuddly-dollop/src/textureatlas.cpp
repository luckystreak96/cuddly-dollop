#include "textureatlas.h"

#include <GL/glew.h>

TextureAtlas TextureAtlas::m_textureAtlas = TextureAtlas(128);

TextureAtlas::TextureAtlas(unsigned int nbTexture) : m_isValid(false), m_currentTextureIndex(0)
{
	if (nbTexture < 4)
		nbTexture = 4;

	// Arrondir sur la puissance de 2 superieure
	m_nbTexturePerSide = (int)sqrt((float)nbTexture);
	if (m_nbTexturePerSide * m_nbTexturePerSide < nbTexture)
		m_nbTexturePerSide++;
	while (!IsPowerOfTwo(m_nbTexturePerSide))
		m_nbTexturePerSide++;
}

TextureAtlas::~TextureAtlas()
{
	if (IsValid())
		glDeleteTextures(1, &m_textureId);
}

std::vector<Vector2f> TextureAtlas::get_placeholder_uv_offset()
{
	std::vector<Vector2f> result;
	for (int v = 0; v < 2; v++)
	{
		for (int u = 0; u < 2; u++)
		{
			float u2 = (float)u;
			float v2 = (float)v;
			TextureIndexToCoord(m_nbTexturePerSide * (m_nbTexturePerSide - 1), u2, v2);
			result.emplace_back(u2, v2);
		}
	}
	return result;
}

TextureAtlas::TextureList& TextureAtlas::GetTextureList()
{
	return m_textureList;
}

void TextureAtlas::ShortenTextureList()
{
	TextureList temp;
	for (auto x : m_textureList)
		temp.emplace(Utils::Split(x.first, '/').back(), x.second);
	m_textureList = temp;
}



GLuint TextureAtlas::GetID()
{
	return m_textureId;
}

TextureAtlas::TextureIndex TextureAtlas::AddTexture(const std::string& fname)
{
	TextureList::iterator it = m_textureList.find(fname);

	if (it != m_textureList.end())
		return it->second.texIdx;

	TextureIndex id = m_currentTextureIndex++;

	Vector2f pos = Vector2f(-1, -1);
	if (fname == "res/sprites/tiles/sapin_b.png")
		pos = Vector2f(0, 0);
	else if (fname == "res/sprites/tiles/sapin_t.png")
		pos = Vector2f(0, 1);

	m_textureList.insert(std::make_pair(fname, TextureInfo((ILuint)-1, id, pos.x, pos.y)));
	return id;
}

bool TextureAtlas::Generate(int textureSize, bool mipmap, std::string filename)
{
	// TODO mipmap pas encore 100% parfait...

	if (!IsPowerOfTwo(textureSize))
		return false;

	//Initialize Devil only once:
	static bool alreadyInitialized = false;
	if (!alreadyInitialized)
	{
		ilInit();
		iluInit();
		alreadyInitialized = true;
	}

	for (TextureList::iterator it = m_textureList.begin(); it != m_textureList.end(); ++it)
	{
		ILuint texid = it->second.texId;
		if (texid == (ILuint)-1)
		{
			//std::cout << "Loading " << it->first << " (id=" << it->second.texIdx << ")..." << std::endl;
			ilGenImages(1, &texid);
			ilBindImage(texid);

			ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
			ilEnable(IL_ORIGIN_SET);

			if (!ilLoadImage((const ILstring)it->first.c_str()))
			{
				std::cout << "Load Image failed." << std::endl;
				return false;
			}

			if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
			{
				std::cout << "Convert Image failed." << std::endl;
				return false;
			}

			iluScale(textureSize, textureSize, 1);

			it->second.texId = texid;
		}
	}


	//std::cout << ilGetInteger(IL_IMAGE_BPP) << std::endl;
	//std::cout << ilGetInteger(IL_IMAGE_FORMAT) << std::endl;
	//std::cout << ilGetInteger(IL_IMAGE_DEPTH) << std::endl;
	//std::cout << ilGetInteger(IL_IMAGE_TYPE) << std::endl;
	//std::cout << ilGetInteger(IL_IMAGE_WIDTH) << std::endl;
	//std::cout << ilGetInteger(IL_IMAGE_HEIGHT) << std::endl;



	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
	if (mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_NEAREST_MIPMAP_LINEAR );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	int level = textureSize;
	int oglLevel = 0;
	int mipmapSize = textureSize * m_nbTexturePerSide;
	while (mipmapSize != 0)
	{
		ILuint atlasTex;
		ilGenImages(1, &atlasTex);
		ilBindImage(atlasTex);
		ilTexImage(mipmapSize, mipmapSize, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, 0);
		ilClearColour(1, 0, 0, 1);
		ilClearImage();

		for (TextureList::iterator it = m_textureList.begin(); it != m_textureList.end(); ++it)
		{
			ILuint tmpImg;
			ilGenImages(1, &tmpImg);
			ilBindImage(tmpImg);
			ilCopyImage(it->second.texId);

			iluImageParameter(ILU_FILTER, ILU_NEAREST);
			//iluImageParameter(ILU_FILTER, ILU_BILINEAR);
			if (level != textureSize)
				iluScale(level, level, 1);

			char* data = new char[level * level * 4];
			ilCopyPixels(0, 0, 0, level, level, 1, IL_RGBA, IL_UNSIGNED_BYTE, data);


			int imgIdx = it->second.texIdx;

			//int x = imgIdx % m_nbTexturePerSide;
			//int y = m_nbTexturePerSide - 1 - imgIdx / m_nbTexturePerSide;
			int x = 0;
			int y = 0;
			// Not defined, gotta set the location
			if (it->second.x == -1)
			{
				bool exit = false;
				while (!exit)
				{
					exit = true;
					x = imgIdx % m_nbTexturePerSide;
					y = m_nbTexturePerSide - 1 - imgIdx / m_nbTexturePerSide;
					imgIdx++;
					for (auto iter = m_textureList.begin(); iter != m_textureList.end(); ++iter)
						if (iter->second.x == x && iter->second.y == y)
							exit = false;
				}

				it->second.x = x;
				it->second.y = y;
			}
			else
			{
				x = it->second.x;
				y = it->second.y;
			}
			ilBindImage(atlasTex);
			ilSetPixels(x * level, y * level, 0, level, level, 1, IL_RGBA, IL_UNSIGNED_BYTE, data);
			//ilOverlayImage(tmpImg, x * level, y * level, 0);

			delete[] data;
			ilDeleteImages(1, &tmpImg);
		}

		// TODO
		//if(level == textureSize)
		//{
		//ilEnable(IL_FILE_OVERWRITE);
		//ilSaveImage("textureatlas.png");
		//}

		//std::cout << oglLevel << ":" << level << ":" << mipmapSize << std::endl;
		glTexImage2D(GL_TEXTURE_2D, oglLevel++, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		//CHECK_GL_ERROR();

		ilEnable(IL_FILE_OVERWRITE);
		ilSaveImage((ILconst_string)filename.c_str());


		ilDeleteImages(1, &atlasTex);

		if (!mipmap)
			break;

		level /= 2;
		mipmapSize /= 2;
	}

	m_isValid = true;
	return true;
}


bool TextureAtlas::IsValid() const
{
	return m_isValid;
}

void TextureAtlas::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void TextureAtlas::TextureIndexToCoord(TextureIndex idx, float& x, float& y) const
{
	float w = 1.f / (float)m_nbTexturePerSide;
	float h = 1.f / (float)m_nbTexturePerSide;

	float u = (float)((unsigned int)idx % m_nbTexturePerSide) * w;
	float v = (float)(m_nbTexturePerSide - 1 - (unsigned int)idx / m_nbTexturePerSide) * h;
	for (auto& x : m_textureList)
	{
		if (x.second.texIdx == idx)
		{
			u = x.second.x * w;
			v = x.second.y * h;
		}
	}


	float halfPixel = 0.125f / (32.0f * m_nbTexturePerSide);//prevent texture bleeding, 32 is hardcoded cuz struggle is real

	x = (u + w * x) + (x == 0 ? halfPixel : -halfPixel);
	y = (v + h * y) + (y == 0 ? halfPixel : -halfPixel);
}

float TextureAtlas::get_u_offset_coordinate(TextureIndex idx) const
{
	float len = 1.f / (float)m_nbTexturePerSide;

	float u = (float)((unsigned int)idx % m_nbTexturePerSide) * len;
	for (auto& x : m_textureList)
		if (x.second.texIdx == idx)
			u = x.second.x * len;

	return u;
}

float TextureAtlas::get_v_offset_coordinate(TextureIndex idx) const
{
	float len = 1.f / (float)m_nbTexturePerSide;

	float v = (float)(m_nbTexturePerSide - 1 - (unsigned int)idx / m_nbTexturePerSide) * len;
	for (auto& x : m_textureList)
		if (x.second.texIdx == idx)
			v = x.second.y * len;

	return v;
}

Vector2f TextureAtlas::GetTexCoordWH()
{
	return Vector2f(1.f / (float)m_nbTexturePerSide, 1.f / (float)m_nbTexturePerSide);
}
