#include "statCurve.h"

#include "statUser.h"
#include <iostream>

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

		int* statVar = GetStatPointer(name, user);

		// Don't try to touch it if its null (invalid stat name)
		if (!statVar)
		{
			std::cout << name << " for curves are not implemented." << std::endl;
			continue;
		}

		// Now that we have our stat's pointer, its time to apply the curve according to the level
		ApplyFunction(statVar, value, op, user);
	}
}

void StatCurve::ApplyFunction(int* stat, float value, std::string opName, StatUser* user)
{
	int x = user->GetLevel();
	float a = value;
	if (opName == "square")
	{
		*stat = a * pow(x, 2);
	}
	else if (opName == "quadratic")
	{
		*stat = (a * pow(x, 2)) + pow(x, 2);
	}
	else if (opName == "square_root")
	{
		*stat = sqrt(a * x);
	}
	else if (opName == "linear")
	{
		*stat = x * a;
	}
}


int* StatCurve::GetStatPointer(std::string stat, StatUser* user)
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
	return 0;
}
