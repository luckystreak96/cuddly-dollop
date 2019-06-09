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
	static float GetFloat(EntityArgType eat);
	static std::shared_ptr<Entity> BuildEntity(std::map<std::string, EntityArgType> args);
	static std::map<unsigned int, std::shared_ptr<Entity>> GetEntities(unsigned int map_id, std::shared_ptr<JsonHandler> jh);
};

#endif // !DIALOGUE_GRAPH_H__

