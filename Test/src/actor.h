#ifndef ACTOR_H__
#define ACTOR_H__

#include <string>

class Actor
{
public:
	Actor(std::string sprite);

public:
	std::string Sprite;
	std::string Name;
};

#endif