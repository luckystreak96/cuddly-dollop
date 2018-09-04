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

Actor::Actor(Actor& a)
{
	_Fighter = Fighter_ptr(new Fighter(*a._Fighter.get()));
	_Graphics = NULL;
	_ColorState = CS_Normal;
	SetDefault();

	_Name = a._Name;
	Sprite = a.Sprite;
	_Graphics->SetTexture(Sprite);
}

bool Actor::ActorSpeedSort(Actor_ptr a, Actor_ptr b)
{
	if (a->_Fighter && b->_Fighter)
		return a->_Fighter->Speed.Modified > b->_Fighter->Speed.Modified;
	return true;
}

bool Actor::ActorBattleOrderSort(Actor_ptr a, Actor_ptr b)
{
	if (a->_Fighter && b->_Fighter)
		return a->_Fighter->_BattleFieldPosition < b->_Fighter->_BattleFieldPosition;
	return true;
}

void Actor::SetDefault()
{
	if (!_Graphics)
		_Graphics = PGraphics_ptr(new PlayerGraphicsComponent());
	if (!_Fighter)
		_Fighter = Fighter_ptr(new Fighter());

	Sprite = "res/sprites/entities/entity_ghost.png";
	_Graphics->SetTexture(Sprite);
	_Name = "Player";
	Selected = false;
	ChoosingAction = true;
}

void Actor::UpdateColor()
{
	bool dead = (_Fighter && _Fighter->Dead);

	_Graphics->_outline = Selected;// Always default to no outline unless selected

	if (Selected)
	{
		if (_ColorState != CS_Selected)
		{
			//_Graphics->SetColorAll(Vector3f(1, 0.25f, 0.25f), dead ? 0.5f : 1.0f);
			_Graphics->SetColorAll(Vector3f(3.0f, 3.0f, 3.0f), dead ? 0.5f : 1.0f);
			_ColorState = CS_Selected;
			_Graphics->_outline = true;
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
	else if (_Fighter->PredictedSkill != NULL && _Fighter->Team == 0 && !ChoosingAction)
	{
		_Graphics->SetColorAll(Vector3f(0.6f, 0.6f, 0.6f), 1.f);
		_ColorState = CS_Darker;
	}
	else if (_ColorState != CS_Normal)
	{
		_Graphics->SetColorAll();
		_ColorState = CS_Normal;
	}
}


void Actor::Update()
{
	UpdateColor();
	//if (!Transformation::perspectiveOrtho)
		_Graphics->GetPosRef().z += MathUtils::HeightGivenLengthOfHypotenuseAndAngle(0.5f, -GraphicsComponent::_persRotation);
	_Graphics->Update();
}

void Actor::AdjustHeightForAngle()
{
	//if (!Transformation::perspectiveOrtho)
		_Graphics->GetPosRef().z -= MathUtils::HeightGivenLengthOfHypotenuseAndAngle(0.5f, -GraphicsComponent::_persRotation);
}
