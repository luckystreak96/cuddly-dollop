#include "passiveSkill.h"

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