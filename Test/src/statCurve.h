#ifndef STAT_CURVE_H__
#define STAT_CURVE_H__

#include <map>
#include "statUser.h"

struct CurveStyle
{
	std::string StatOperator;
	float Value;
};

struct CurveInfo
{
	std::map<std::string, CurveStyle> Stats;
};

// StatCurves allow characters to have base stats set according to level,
//	this would be calculated before all other skills/stat mods

class StatCurve
{
public:
	static void SetCurve(StatUser* user);

	static std::map<std::string, CurveInfo> Curves;
	static int* GetStatPointer(std::string stat, StatUser* user);
	static void ApplyFunction(int* stat, float value, std::string opName, StatUser* user);
};

#endif