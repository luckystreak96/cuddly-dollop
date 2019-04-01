#ifndef PLAYERGRAPHICSCOMPONENT_H__
#define PLAYERGRAPHICSCOMPONENT_H__

#include "iComponent.h"
#include "graphics_component.h"
#include "animation.h"
#include "observable.h"
#include <string>

enum Direction { dir_Left, dir_Down, dir_Right, dir_Up };

class PlayerGraphicsComponent : public GraphicsComponent, public Animation, public Observable
{
public:
	PlayerGraphicsComponent(std::string tex = "res/sprites/entities/entity_ghost_shiny_blue.png", std::string model = "CENTERED_TILE");
	void Update();
	void Draw(bool withTex = true);
	void NormalDraw(bool withTex = true);
	void DrawOutline(bool withTex = true);
	void set_texture(std::string texture);
	void SetAnimation(Anim_Enum anim, std::string spritesheet);

	Direction GetDirection();
	void SetDirection(Direction dir);
	void SetDirection(GraphicsComponent* graph);
	void update_direction(Vector3f);

	//void reset_to_default_animation();

public:
	bool _outline;

private:
	bool m_firstLoad;
	bool m_force_update;
	int m_actualDelay;
	Vector3f m_prevPosition;

	Direction m_direction;
	Direction m_prev_dir;
	bool m_dir_determined;
};

#endif
