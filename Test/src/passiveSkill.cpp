#include "passiveSkill.h"

std::map<std::string, int> PassiveSkill::EnumToInt = std::map<std::string, int> {
	std::make_pair("PP_AbsoluteFirst", (int)PP_AbsoluteFirst),
	std::make_pair("PP_Secondary", (int)PP_Secondary),
	std::make_pair("PP_Last", (int)PP_Last),
	std::make_pair("PP_BeforeSkill", (int)PP_BeforeSkill),
	std::make_pair("PP_BeforeDamage", (int)PP_BeforeDamage),
	std::make_pair("PP_AfterDamage", (int)PP_AfterDamage),
	std::make_pair("PP_AfterSkill", (int)PP_AfterSkill),
	std::make_pair("PT_Skill", (int)PT_Skill),
	std::make_pair("PT_SkillUpgrade", (int)PT_SkillUpgrade),
	std::make_pair("PT_Stat", (int)PT_Stat),
	std::make_pair("PT_Special", (int)PT_Special),
	std::make_pair("PS_Flat", (int)PS_Flat),
	std::make_pair("PS_Percent", (int)PS_Percent),
	std::make_pair("PS_None", (int)PS_None)
};

PassiveData::PassiveData()
{
	_String = ""; _Integer = 0; _Float = 0;
}

PassiveSkill::PassiveSkill()
{
	_Id = 0;
	_Name = "";
	_Type = PT_Stat;
	_Priority = PP_AbsoluteFirst;
	_Data = PassiveData();
	_Specifier = PS_None;
}