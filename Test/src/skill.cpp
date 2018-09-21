#include "skill.h"
#include "battleManager.h"
#include "fontFloat.h"
#include "GLFW/glfw3.h"
#include "particleManager.h"
#include "animScreenShake.h"
#include "animColorFlash.h"
#include "animBonusEffect.h"

Skill::Skill()
{
	DefaultSetup();
}

void Skill::DefaultSetup()
{
	_cooldown = 0;
	_name = "SKILL?";
	_done = false;
	_skillType = ST_Physical;
	_skillElement = SE_None;
	_currentCooldown = 0;
	_minTargets = 1;
	_targetMode = TM_Alive;
	_targetAmount = TA_One;
	_defaultTarget = DT_Self;
	m_progress = 0;
	_ac._start = 0;
	_ac._startHard = 1000;
	_ac._end = 0;
	_ac._success = false;
	_ac._animProg = 1;
	_ac._type = ACT_Defend;
	_critting = false;
	_skillUpgradeProgress = 0;
	_skillUpgradeMax = 0;
	_isPreCalculated = false;
}

void Skill::CheckActionCommand(double percentProgress)
{
	// Did you input anything
	if (_input.size() > 0 && (_input.count(A_Accept) || _input.count(A_Cancel)))
	{
		// Are you in a situation to try an action command
		if (_ac._tried || _ac._success)
			return;
		else
		{
			if (percentProgress <= _ac._end)
			{
				if (_input.count(A_Cancel))
				{
					if (percentProgress >= _ac._startHard)
					{
						_ac._type = ACT_Special;
						_ac._success = true;
					}
				}
				else if (_input.count(A_Accept))
				{

					if (percentProgress >= _ac._start)
					{
						_ac._type = ACT_Defend;
						_ac._success = true;
					}
				}
			}
		}

		_ac._tried = true;
	}
}

// Must return the new state
void Skill::Setup()
{
	_ac._tried = false;
	_ac._success = false;
	_animProg = 0;
	m_progress = 0;
	m_state = SP_1_Before_Anim;
}

std::vector<triple> Skill::GetAnimations()
{
	std::vector<triple> result;
	result.swap(m_animationBuffer);
	return result;
}

void Skill::SetAnimations()
{
	m_animationBuffer.clear();
	_done = true;
}

// Handle Action command stuff
void Skill::HandleActionCommand(double percentProgress)
{
	// Can only happen if you press within the right animation
	if (_ac._animProg == m_progress)
	{
		// Handle input for action command
		CheckActionCommand(percentProgress);
	}
}

int Skill::action_command_level(double percentProgress)
{
	if (_ac._animProg == m_progress)
	{
		if (percentProgress <= _ac._end && percentProgress >= _ac._startHard)
			return 1;
		else if (percentProgress <= _ac._end && percentProgress >= _ac._start)
			return 2;
	}
	return 0;
}


//void Skill::SetupProtector()
//{
//	_targets.push_back(_targets.at(0));
//	_targets.at(0) = _targets.at(0)->_Fighter->Protector;
//
//}


SkillProgress Skill::Update()
{
	// Reset input
	_input = std::set<int>();

	m_progress++;
	SetAnimations();

	return m_state;
}

void Skill::Reset()
{
	m_progress = 0;
	_done = false;
}

bool Skill::IsReady()
{
	return _currentCooldown <= 0;
}



//bool Skill::AnimationsDone()
//{
//	bool done = true;
//	for (auto& x : *_anims)
//		if (!x->_async)
//			done = false;
//
//	return done;
//}
//
