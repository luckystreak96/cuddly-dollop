#include "actor.h"
#include "skill.h"
#include "skillSmack.h"

Actor::Actor()
{
	_Fighter = NULL;
	_Graphics = NULL;
	SetDefault();
}

bool Actor::ActorSpeedSort(Actor_ptr a, Actor_ptr b)
{
	if (a->_Fighter && b->_Fighter)
		return a->_Fighter->Speed > b->_Fighter->Speed;
}

void Actor::SetDefault()
{
	if(!_Graphics)
		_Graphics = PGraphics_ptr(new PlayerGraphicsComponent());
	if(!_Fighter)
		_Fighter = Fighter_ptr(new Fighter());

	Sprite = "res/sprites/entities/entity_ghost.png";
	_Graphics->SetTexture(Sprite);
	_Name = "Player";
	Selected = false;
	ChoosingAction = false;
}

void Actor::UpdateColor()
{
	if (ChoosingAction)
		_Graphics->SetColorAll(/*Vector3f(1.f, 1.f, 0), 1.f*/);
	else if (_Fighter && _Fighter->Dead)
		_Graphics->SetColorAll(Vector3f(0.25f, 0.25f, 0.25f), 0.5f);
	else
		_Graphics->SetColorAll();
}


void Actor::Update()
{
	_Graphics->Update();
}

