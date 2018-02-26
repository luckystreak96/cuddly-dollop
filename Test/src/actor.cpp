#include "actor.h"
#include "skill.h"
#include "skillSmack.h"

Actor::Actor()
{
	_Fighter = NULL;
	_Graphics = NULL;
	_ColorState = CS_Normal;
	SetDefault();
}

bool Actor::ActorSpeedSort(Actor_ptr a, Actor_ptr b)
{
	if (a->_Fighter && b->_Fighter)
		return a->_Fighter->Speed.Real > b->_Fighter->Speed.Real;
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
	bool dead = (_Fighter && _Fighter->Dead);

	if(Selected)
	{
		if (_ColorState != CS_Red)
		{
			_Graphics->SetColorAll(Vector3f(1, 0.25f, 0.25f), dead ? 0.5f : 1.0f);
			_ColorState = CS_Red;
		}
	}
	else if (dead)
	{
		if (_ColorState != CS_Invis)
		{
			_Graphics->SetColorAll(Vector3f(0.25f, 0.25f, 0.25f), 0.5f);
			_ColorState = CS_Invis;
		}
	}
	else if(_ColorState != CS_Normal)
	{
		_Graphics->SetColorAll();
		_ColorState = CS_Normal;
	}
}


void Actor::Update()
{
	UpdateColor();
	_Graphics->Update();
}

