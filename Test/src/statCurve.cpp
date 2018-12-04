#include "statCurve.h"

#include "statUser.h"
#include <iostream>
#include <assert.h>
#include <math.h> // pow

std::map<std::string, CurveInfo> StatCurve::Curves = std::map<std::string, CurveInfo>();

void StatCurve::SetCurve(StatUser* user)
{
	std::string curve = user->Curve;
	if (curve == "")
		return;

	if (!StatCurve::Curves.count(curve))
		return;

	std::string name;
	float value;
	std::string op;
	for (auto stat : StatCurve::Curves.at(curve).Stats)
	{
		name = stat.first;
		value = stat.second.Value;
		op = stat.second.StatOperator;

		Stat* statVar = GetFullStatPointer(name, user);

		// Don't try to touch it if its null (invalid stat name)
		if (!statVar)
		{
			std::cout << name << " for curves are not implemented." << std::endl;
			continue;
		}

		// Now that we have our stat's pointer, its time to apply the curve according to the level
		ApplyFunction(statVar, value, op, user);
	}

	// Make sure to set real and modified to be used later
	user->ResetRealModified();
}

void StatCurve::ApplyFunction(Stat* stat, float value, std::string opName, StatUser* user)
{
	int x = user->GetLevel();
	float a = value;
	if (opName == "square")
	{
		stat->Base = (int)(a * pow(x, 2));
		//stat->Real = stat->Base;
	}
	else if (opName == "quadratic")
	{
		stat->Base = (int)(a * pow(x + 18, 3));
		//stat->Real = stat->Base;
	}
	else if (opName == "square_root")
	{
		stat->Base = (int)sqrt(a * x);
		//stat->Real = stat->Base;
	}
	else if (opName == "linear")
	{
		stat->Base = (int)(x * a);
		//stat->Real = stat->Base;
	}

	// +5 max hp
	//if (stat == user->GetMaxHealthPointer())
	//	stat->Base += 5;
}


int* StatCurve::GetBaseStatPointer(std::string stat, StatUser* user)
{
	if (stat == "max_health")
		return &user->GetMaxHealthPointer()->Base;
	else if (stat == "strength")
		return &user->Strength.Base;
	else if (stat == "endurance")
		return &user->GetEndurancePointer()->Base;
	else if (stat == "crit")
		return &user->Crit.Base;
	else if (stat == "speed")
		return &user->Speed.Base;
	else if (stat == "defense")
		return &user->Defense.Base;

	// none was found, return 0
	assert(false);// should never reach here
	return 0;
}

Stat* StatCurve::GetFullStatPointer(std::string stat, StatUser* user)
{
	if (stat == "max_health")
		return user->GetMaxHealthPointer();
	else if (stat == "strength")
		return &user->Strength;
	else if (stat == "endurance")
		return user->GetEndurancePointer();
	else if (stat == "crit")
		return &user->Crit;
	else if (stat == "speed")
		return &user->Speed;
	else if (stat == "defense")
		return &user->Defense;

	// none was found, return 0
	assert(false);// should never reach here
	return 0;
}
