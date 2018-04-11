#include "skillRanged.h"
#include "battleManager.h"
#include "animJumpTo.h"
#include "animBasic.h"
#include "animMoveTo.h"

SkillRanged::SkillRanged()
{
	Skill::DefaultSetup();
	DefaultSetup();
}

void SkillRanged::DefaultSetup()
{
	_name = "Skill Ranged -_-";
}

// Must return the new state
BattleState SkillRanged::Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
{
	Skill::Start(targets, actors, anims, owner);
	_basePos = _owner->_Graphics->GetPos();

	_anims->push_back(Anim_ptr(new AnimMoveTo(_owner->_Graphics->GetPosRef() + Vector3f(_owner->_Fighter->Team ? -1 : 1, 0, 0), _owner)));

	return BS_ActionProgress;
}

void SkillRanged::Update()
{
	double duration = -1;
	double progress = 0;
	if (_anims->size() > 0)
	{
		progress = _anims->front()->_progress;
		duration = _anims->front()->_duration;
	}

	HandleActionCommand(progress / duration);

	Camera::_currentCam->SetScale(Vector3f(1.5f));

	switch (_animProg)
	{
	case 0:
		Camera::_currentCam->SetFollowCenteredXY((_targets.at(0)->_Graphics->GetPosRef() + _owner->BasePosition) / 2.f);
		if (!AnimationsDone())
			break;
		Animate();
		_animProg++;
		break;
	case 1:
		// DEAL DMG
		Camera::_currentCam->SetFollowCenteredXY(_targets.at(0)->_Graphics->GetPosRef());
		if (AnimationsDone())
		{
			_animProg++;
		}
		else
		{
			progress = _anims->front()->_progress;
			duration = _anims->front()->_duration;
			if (TimingCondition(progress, duration))
			{
				ApplyEffect();
				_animProg++;
			}
		}
		break;
	case 2:
		// SET THIS SKILL TO DONE
		Camera::_currentCam->SetScale(Vector3f(1.f));
		Camera::_currentCam->SetFollowCenteredXY(_owner->_Graphics->GetPosRef());
		if (!AnimationsDone())
			break;
		_done = true;
		break;
	}
	// Reset input
	_input = std::set<int>();
}

void SkillRanged::Animate()
{
	_anims->push_back(Anim_ptr(new AnimBasic(AE_Attack, _owner, 1)));
}

void SkillRanged::ApplyEffect()
{
	Damage result;
	result._value = 0;
	result._type = ST_Physical;
	//dmg = rand() % 3 + 1;
	_targets.at(0)->_Fighter->TakeDamage(result);

	// Damage text
	SpawnDamageText(_targets.at(0), result._value);
}

bool SkillRanged::TimingCondition(double progress, double duration)
{
	return progress > duration / 2;
}

bool SkillRanged::IsReady()
{
	return _currentCooldown <= 0;
}
