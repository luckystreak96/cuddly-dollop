#include "resource_manager.h"

Texture* ResourceManager::GetTexture(std::string texture)
{
	if (m_textures.count(texture) > 0)
		return &m_textures.at(texture);

	return nullptr;
}

bool ResourceManager::LoadTexture(std::string tex)
{
	static std::mutex mutex;
	mutex.lock();
	if (m_textures.count(tex) > 0)
	{
		mutex.unlock();
		return true;
	}
	mutex.unlock();

	std::wstring ws(tex.begin(), tex.end());
	const wchar_t* path = ws.c_str();

	mutex.lock();
	m_textures.emplace(tex, Texture(GL_TEXTURE_2D, path));
	mutex.unlock();
	return m_textures.at(tex).LoadFromFile();

}

void ResourceManager::AddTexture(std::string name, Texture tex)
{
	m_textures.emplace(name, tex);
}

void ResourceManager::LoadGLTexture(std::string tex)
{
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
