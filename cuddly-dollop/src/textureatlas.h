#ifndef TEXTUREATLAS_H__
#define TEXTUREATLAS_H__
#include <string>
#include <map>
#include <IL/ilu.h>
#include "GL/glew.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <cassert>
#include "utils.h"
#include "vector3f.h"


class TextureAtlas
{
public:
	typedef unsigned int TextureIndex;
	struct TextureInfo
	{
		ILuint texId;
		TextureIndex texIdx;
		int x;
		int y;

		TextureInfo(ILuint _texId, unsigned int _texIdx, int x, int y) : texId(_texId), texIdx(_texIdx), x(x), y(y) {}
	};
	typedef std::map<std::string, TextureInfo> TextureList;

public:
	TextureAtlas(unsigned int nbTexture);
	~TextureAtlas();
	std::vector<Vector2f> get_placeholder_uv_offset();
	TextureIndex AddTexture(const std::string& fname);
	bool Generate(int textureSize, bool mipmap, std::string filename);
	TextureList& GetTextureList();
	void ShortenTextureList();

	bool IsValid() const;
	void Bind() const;
	GLuint GetID();
	Vector2f GetTexCoordWH();

	void TextureIndexToCoord(TextureIndex idx, float& u, float& v) const;
	float get_u_offset_coordinate(TextureIndex idx) const;
	float get_v_offset_coordinate(TextureIndex idx) const;

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
