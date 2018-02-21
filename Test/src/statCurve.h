#ifndef STAT_CURVE_H__
#define STAT_CURVE_H__

#include <map>
#include "statUser.h"

struct CurveInfo
{
	std::map<std::string, char> OperatorPerStat;
};

enum CurveType {CT_Basic};

class StatCurve : public StatUser
{
public:
	static void SetCurve(int curve, StatUser* user);

	static std::map<int, CurveInfo> Curves;
};

#endif