#ifndef ACTOR_H__
#define ACTOR_H__

#include <string>
#include <memory>
#include "playerGraphicsComponent.h"
#include "fighter.h"

class Actor
{
public:
	Actor();
	void Update();
	void UpdateColor();

public:
	Fighter_ptr _Fighter;
	PGraphics_ptr _Graphics;

public:
	std::string _Name;
	bool Selected;
	bool ChoosingAction;
	std::string Sprite;
	Vector3f BasePosition;
	static bool ActorSpeedSort(Actor_ptr a, Actor_ptr b);

protected:
	void SetDefault();
};

#endif