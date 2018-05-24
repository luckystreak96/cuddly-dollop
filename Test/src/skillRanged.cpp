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
	_targetProgress = 0;
}

// Must return the new state
BattleState SkillRanged::Setup(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
{
	Skill::Setup(targets, actors, anims, owner);

	return BS_ActionProgress;
}

void SkillRanged::Start()
{
	Skill::Start();
	_basePos = _owner->_Graphics->GetPos();

	_anims->push_back(Anim_ptr(new AnimMoveTo(_owner->_Graphics->GetPosRef() + Vector3f(_owner->_Fighter->Team ? -1 : 1, 0, 0), _owner)));

	// Reset for re-use
	_cameraTarget = Vector3f();

	// Get average of all targets and attacker
	for (auto& x : _targets)
		_cameraTarget += x->_Graphics->GetPosRef();

	_cameraTarget.x += _owner->_Graphics->GetPosRef().x;
	_cameraTarget /= (float)_targets.size();

	_targetProgress = 0;
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

	if(_anims->size() && dynamic_cast<AnimBasic*>(_anims->front().get()) != NULL)
		HandleActionCommand(progress / duration);

	Camera::_currentCam->SetScale(Vector3f(1.10f));

	switch (_animProg)
	{
	case 0:
		Camera::_currentCam->SetFollowCenteredXY(_cameraTarget);
		if (!AnimationsDone())
			break;
		Animate();
		_animProg++;
		break;
	case 1:
		// DEAL DMG
		if (AnimationsDone())
		{
			if (_targetProgress == _targets.size())
			{
				// walk back at a decent speed
				_anims->push_back(Anim_ptr(new AnimMoveTo(_owner->BasePosition, _owner, 1)));
				_animProg++;
			}
			else
				ApplyEffect();
		}
		else if(_targetProgress == 0)
		{
			progress = _anims->front()->_progress;
			duration = _anims->front()->_duration;
			if (TimingCondition(progress, duration))
			{
				// Allow the rest of the animations to go on without a hitch
				if(_anims->size())
					_anims->front()->_async = true;
				Camera::_currentCam->SetFollowCenteredXY(_cameraTarget);
				ApplyEffect();
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
	_anims->push_back(Anim_ptr(new AnimBasic(AE_Attack, _owner, 1.0)));
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
