#include "texture.h"

Texture::Texture(GLuint texID, GLenum target) : m_textureTarget(target), m_filename(""), m_isValid(true), m_texId(texID), loaded(true), m_filenameS("")
{
}

Texture::Texture(GLenum TextureTarget, char* FileName) : m_textureTarget(TextureTarget), m_filename(FileName), m_isValid(false), m_filenameS(std::string(FileName))
{
}

Texture::Texture(GLenum TextureTarget, const char* FileName) : m_textureTarget(TextureTarget), m_filename(FileName), m_isValid(false), m_filenameS(std::string(FileName))
{
}

Texture::~Texture()
{
	Unload();
}

bool Texture::LoadFromFile()
{
	if (m_filenameS == "")
		return false;
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
	if (!ilLoadImage((const ILstring)m_filenameS.c_str()))
		return false;

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		return false;

	return true;
}

unsigned char* Texture::GetPixels()
{
	LoadFromFile();
	unsigned char* pixels = new unsigned char[32 * 32 * 4];
	ILubyte* data = ilGetData();
	for (int i = 0; i < (32 * 32 * 4); i++)
		pixels[i] = (unsigned char)data[i];

	ilDeleteImages(1, &m_texId);

	return pixels;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(m_textureTarget, 0, GL_SRGB_ALPHA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	// Get width and height
	_width = ilGetInteger(IL_IMAGE_WIDTH);
	_height = ilGetInteger(IL_IMAGE_HEIGHT);

	// Unbind and delete the un-gl'd texture
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
