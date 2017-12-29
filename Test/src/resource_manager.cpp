#include "resource_manager.h"

Texture* ResourceManager::GetTexture(std::string texture)
{
	if (m_textures.count(texture) > 0)
		return &m_textures.at(texture);

	return nullptr;
}

bool ResourceManager::LoadTexture(std::string tex)
{
	if (tex == "" || tex[tex.size() - 1] == '\\' || tex[tex.size() - 1] == '/')
		return false;

	static std::mutex mutex;
	mutex.lock();
	if (m_textures.count(tex) > 0)
	{
		mutex.unlock();
		return true;
	}
	mutex.unlock();

	const char* path = tex.c_str();

	mutex.lock();
	m_textures.emplace(tex, Texture(GL_TEXTURE_2D, path));
	mutex.unlock();
	if (m_textures.at(tex).LoadFromFile())
	{
		m_textures.at(tex).LoadGL();
		return true;
	}
	return false;
}

void ResourceManager::AddTexture(std::string name, Texture tex)
{
	m_textures.emplace(name, tex);
}

void ResourceManager::LoadGLTexture(std::string tex)
{
	if (tex != "")
		m_textures.at(tex).LoadGL();
}

void ResourceManager::UnloadAllResources()
{
	m_textures.clear();
}

void ResourceManager::LoadAllExternalResources(std::vector<ResourceUser*>* list)
{
	m_loader = ResourceLoader(list);
	m_loader.Run();
}
