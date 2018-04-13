#ifndef PLAYERGRAPHICSCOMPONENT_H__
#define PLAYERGRAPHICSCOMPONENT_H__

#include "iComponent.h"
#include "graphicsComponent.h"
#include "animation.h"
#include "observable.h"
#include <string>

class PlayerGraphicsComponent : public GraphicsComponent, public Animation, public Observable
{
public:
	PlayerGraphicsComponent(std::string tex = "res/sprites/entities/entity_ghost_shiny_blue.png", std::string model = "CENTERED_TILE");
	void Update();
	void Draw(bool withTex = true);
	void NormalDraw(bool withTex = true);
	void DrawOutline(bool withTex = true);
	void SetTexture(std::string texture);
	void SetAnimation(Anim_Enum anim, std::string spritesheet);
private:
	bool m_firstLoad;
	int m_actualDelay;
	Vector3f m_prevPosition;
};

typedef std::shared_ptr<PlayerGraphicsComponent> PGraphics_ptr;

#endif
