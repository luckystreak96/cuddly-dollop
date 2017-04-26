#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <IL/devil_internal_exports.h>
#include <cassert>

class Texture
{
public:
	Texture(GLuint texID, GLenum target = GL_TEXTURE_2D);
   Texture(GLenum TextureTarget, wchar_t* FileName);
   Texture(GLenum TextureTarget, const wchar_t* FileName);
   ~Texture();

   bool LoadFromFile();
   void LoadGL();

   bool Load();
   bool Unload();
   bool IsValid();

   void Bind(GLenum TextureUnit);
public:
   bool loaded = false;
private:
	ILuint m_texId;
	bool m_isValid;
   GLenum m_textureTarget;
   GLuint m_textureId;
   const wchar_t* m_filename;
};

#endif
