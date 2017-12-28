#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <IL/devil_internal_exports.h>
#include <cassert>

class Texture
{
public:
	Texture(GLuint texID, GLenum target = GL_TEXTURE_2D);
	Texture(GLenum TextureTarget, char* FileName);
	Texture(GLenum TextureTarget, const char* FileName);
	~Texture();

	bool LoadFromFile();
	void LoadGL();

	bool Load();
	bool Unload();
	bool IsValid();

	unsigned char* GetPixels();

	void Bind(GLenum TextureUnit);
public:
	bool loaded = false;
private:
	ILuint m_texId;
	bool m_isValid;
	GLenum m_textureTarget;
	GLuint m_textureId;
	const char* m_filename;
	std::string m_filenameS;
};

#endif
