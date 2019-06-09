#ifndef RESOURCE_USER_H__
#define RESOURCE_USER_H__

class ResourceUser
{
public:
	virtual bool LoadExternalResources() = 0;
	virtual bool UnloadExternalResources() = 0;
};

#endif // !RESOURCE_USER_H__
