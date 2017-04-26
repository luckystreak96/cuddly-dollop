#include "texture.h"

Texture::Texture(GLuint texID, GLenum target) : m_textureTarget(target), m_filename(L""), m_isValid(true), m_texId(texID), loaded(true)
{
}

Texture::Texture(GLenum TextureTarget, wchar_t* FileName) : m_textureTarget(TextureTarget), m_filename(FileName), m_isValid(false)
{
}

Texture::Texture(GLenum TextureTarget, const wchar_t* FileName) : m_textureTarget(TextureTarget), m_filename(FileName), m_isValid(false)
{
}

Texture::~Texture()
{
	Unload();
}

bool Texture::LoadFromFile()
{

	// Initialize Devil only once:
	static bool alreadyInitialized = false;
	if (!alreadyInitialized)
	{
		ilInit();
		alreadyInitialized = true;
	}

	// Use Devil library to load image data in memory
	ilGenImages(1, &m_texId);
	ilBindImage(m_texId);

	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	ilEnable(IL_ORIGIN_SET);
	if (!ilLoadImage((const ILstring)m_filename))
		return false;

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		return false;

	return true;
}

void Texture::LoadGL()
{
	if (m_isValid)
		return;

	ilBindImage(m_texId);

	// Create mipmapped opengl texture from image data
	glGenTextures(1, &m_textureId);
	glBindTexture(m_textureTarget, m_textureId);


	//gluBuild2DMipmaps(m_textureTarget, 3, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(m_textureTarget, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
	glBindTexture(m_textureTarget, 0);
	ilDeleteImages(1, &m_texId);

	m_isValid = true;
}

bool Texture::Load()
{
	if (!LoadFromFile())
		return false;

	LoadGL();

	return true;
}

bool Texture::Unload()
{
	if (IsValid())
	{
		glDeleteTextures(1, &m_textureId);
		return true;
	}

	return false;
}

bool Texture::IsValid()
{
	return m_isValid;
}

void Texture::Bind(GLenum TextureUnit)
{
	assert(m_isValid);
	glActiveTexture(TextureUnit);
    glBindTexture(m_textureTarget, m_textureId);
}
