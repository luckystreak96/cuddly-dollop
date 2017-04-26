#ifndef RESOURCE_LOADER_H__
#define RESOURCE_LOADER_H__

#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include "resource_user.h"
#include <iostream>

class ResourceLoader
{
public:
	ResourceLoader(std::vector<ResourceUser*>* list = NULL);
	~ResourceLoader();
	void Run();
	void Stop();//useless?

private:
	void Fetcher();

private:
	bool m_stop;
	std::vector<ResourceUser*>* m_list;
};

#endif // !RESOURCE_LOADER_H__
