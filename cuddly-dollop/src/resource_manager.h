#ifndef RESOURCE_MANAGER_H__
#define RESOURCE_MANAGER_H__

#include <mutex>
#include <map>
#include <string>
#include <vector>
#include "texture.h"
#include "resource_loader.h"
#include "resource_user.h"

class ResourceManager
{
public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;
		return instance;
	}
	Texture* GetTexture(std::string texture);
	bool LoadTexture(std::string texture);
	void LoadGLTexture(std::string texture);
	void LoadAllExternalResources(std::vector<ResourceUser*>* list);
	void AddTexture(std::string name, Texture texture);
	void UnloadAllResources();
private:
	ResourceManager() : m_textures(std::map<std::string, Texture>()) {}
	std::map<std::string, Texture> m_textures;
	ResourceLoader m_loader;
};

#endif // !RESOURCE_MANAGER_H__
