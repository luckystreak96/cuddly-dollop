#ifndef ACTOR_H__
#define ACTOR_H__

#include <string>
#include <memory>
#include "playerGraphicsComponent.h"
#include "fighter.h"

enum ColorState { CS_Invis, CS_Selected, CS_Normal, CS_Update, CS_Darker };

class Actor
{
public:
	Actor();
	Actor(Actor& a);
	void Update();
	void UpdateColor();

public:
	Fighter_ptr _Fighter;
	PGraphics_ptr _Graphics;

public:
	ColorState _ColorState;
	std::string _Name;
	bool Selected;
	bool ChoosingAction;
	std::string Sprite;
	Vector3f BasePosition;
	static bool ActorSpeedSort(Actor_ptr a, Actor_ptr b);
	static bool ActorBattleOrderSort(Actor_ptr a, Actor_ptr b);

protected:
	void SetDefault();
};

#endif