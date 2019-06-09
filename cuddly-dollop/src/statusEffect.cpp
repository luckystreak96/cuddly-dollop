#include "statusEffect.h"

unsigned int StatusEffect::counter = 0;

StatusEffect::StatusEffect() : _Element(SE_None), _Triggered(false) 
{ 
	_Id = counter++;
	_Conditions.push_back(TriggerCondition::TC_Status_Owner_Alive);
}