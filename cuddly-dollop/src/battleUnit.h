#ifndef BATTLE_UNIT_H__
#define BATTLE_UNIT_H__

#include "statUser.h"
#include "vector3f.h"
#include "hudBar.h"

#include <vector>
#include <string>

struct BattleUnit
{
	int id;
	std::string name;
	StatUser* stats;


	// Fighter Observers
	std::vector<Obs_ptr>* fobservers;

	// Actor Observers
	std::vector<Obs_ptr>* aobservers;


	Vector3f* position;
	int team;
	int* turnOrder;
	bool* dead;
	std::vector<int>* targets;

	//bool operator<(BattleUnit other) { id < other.id; }
};

struct HudBattleUnit

{
	BattleUnit unit;
	std::vector<HudComp_ptr> hud;
};


#endif // !BATTLE_UNIT_H__
