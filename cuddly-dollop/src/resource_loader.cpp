#include "resource_loader.h"

ResourceLoader::ResourceLoader(std::vector<ResourceUser*>* list) : m_list(list), m_stop(false) {}

ResourceLoader::~ResourceLoader()
{
}

void ResourceLoader::Fetcher()
{
	static std::mutex mutex;
	while (true)
	{
		mutex.lock();
		if (m_list->size() <= 0)
		{
			mutex.unlock();
			break;
		}
		ResourceUser* user = m_list->back();
		m_list->pop_back();

		mutex.unlock();

		mutex.lock();
		user->LoadExternalResources();
		mutex.unlock();
	}
}

void ResourceLoader::Run()
{
	m_stop = false;
	std::thread threads[15];

	unsigned int MAX_THREAD_COUNT = std::thread::hardware_concurrency();

	for (int i = 0; i < MAX_THREAD_COUNT - 1; i++)
		threads[i] = std::thread(&ResourceLoader::Fetcher, this);

	for (int i = 0; i < MAX_THREAD_COUNT - 1; i++)
		threads[i].join();
}

void ResourceLoader::Stop()
{
	//m_stop = true;
	//for (int i = 0; i < m_threads.size(); i++)
	//	m_threads[i].join();

	//m_threads.clear();
}