#ifndef TEXTUREATLAS_H__
#define TEXTUREATLAS_H__
#include <string>
#include <map>
#include <IL/ilu.h>
#include <GL/glew.h>
#include <cmath>
#include <iostream>
#include <sstream>
#include <cassert>
#include "utils.h"


class TextureAtlas
{
public:
	typedef unsigned int TextureIndex;
	struct TextureInfo
	{
		ILuint texId;
		TextureIndex texIdx;

		TextureInfo(ILuint _texId, unsigned int _texIdx) : texId(_texId), texIdx(_texIdx) {}
	};
	typedef std::map<std::string, TextureInfo> TextureList;

public:
	TextureAtlas(unsigned int nbTexture);
	~TextureAtlas();
	TextureIndex AddTexture(const std::string& fname);
	bool Generate(int textureSize, bool mipmap, std::string filename);
	TextureList& GetTextureList();
	void ShortenTextureList();

	bool IsValid() const;
	void Bind() const;
	GLuint GetID();

	void TextureIndexToCoord(TextureIndex idx, float& u, float& v) const;

	static TextureAtlas m_textureAtlas;
private:
	bool IsPowerOfTwo(unsigned int x)
	{
		return ((x != 0) && ((x & (~x + 1)) == x));
	}

private:

	// On utilise un std::map pour avoir des valeurs uniques
	TextureList m_textureList;

	TextureIndex m_currentTextureIndex;
	GLuint  m_textureId;
	bool    m_isValid;
	unsigned int m_nbTexturePerSide;
};

#endif // TEXTUREATLAS_H__
