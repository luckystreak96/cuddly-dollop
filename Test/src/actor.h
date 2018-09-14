#ifndef ACTOR_H__
#define ACTOR_H__

#include <string>
#include <memory>
#include "playerGraphicsComponent.h"
#include "fighter.h"

class Actor;
using Actor_ptr = std::shared_ptr<Actor>;

enum ColorState { CS_Invis, CS_Selected, CS_Normal, CS_Update, CS_Darker };

class Actor : public PlayerGraphicsComponent
{
public:
	Actor();
	Actor(Actor& a);
	~Actor();
	void Update();
	void UpdateColor(bool dead, bool selected);
	void AdjustHeightForAngle();

	int GetId() { return ID; }

public:
	int ID;
	ColorState _ColorState;
	bool Selected;
	bool ChoosingAction;
	Vector3f BasePosition;

protected:
	void SetDefault();
};

#endif