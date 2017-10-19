#ifndef ENTITY_FACTORY_H__
#define ENTITY_FACTORY_H__

#include <string>
#include <vector>
#include <map>
#include <variant>
#include "jsonHandler.h"
#include "entity.h"

typedef std::variant<bool, float, int, unsigned int, std::string, Vector3f> EntityArgType;

class EntityFactory
{
public:
	static Entity* BuildEntity(std::map<std::string, EntityArgType> args);
	static std::map<unsigned int, Entity*> GetEntities(unsigned int map_id);
};

#endif // !DIALOGUE_GRAPH_H__

