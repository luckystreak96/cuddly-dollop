#ifndef PLAYERGRAPHICSCOMPONENT_H__
#define PLAYERGRAPHICSCOMPONENT_H__

#include "iComponent.h"
#include "graphicsComponent.h"
#include "animation.h"
#include <string>

class PlayerGraphicsComponent : public GraphicsComponent, public Animation
{
public:
	PlayerGraphicsComponent(std::string tex = "res/player.png", std::string model = "TILE");
	void Update();
	void Draw(bool withTex = true);
private:
	bool m_firstLoad;
};

#endif
