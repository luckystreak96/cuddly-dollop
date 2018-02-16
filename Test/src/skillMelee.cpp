#include "skillMelee.h"
#include "battleManager.h"
#include "animJumpTo.h"
#include "animBasic.h"

SkillMelee::SkillMelee()
{
	Skill::DefaultSetup();
	DefaultSetup();
}

void SkillMelee::DefaultSetup()
{
	_name = "Skill Melee -_-";
}

// Must return the new state
BattleState SkillMelee::Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
{
	Skill::Start(targets, actors, anims, owner);
	_basePos = _owner->GetPos();
	// INSERT JUMP FOREWARD ANIMATION HERE
	float distance = _owner->Team == 0 ? 0.7f : -0.7f;
	bool protector = _targets->at(0)->Protector != NULL;
	if (protector)
		distance *= 2.0f;
	_anims->push_back(Anim_ptr(new AnimJumpTo(_targets->at(0)->GetPos() - Vector3f(distance, 0, 0), _owner)));
	if (protector)
	{
		_targets->push_back(_targets->at(0));
		_targets->at(0) = _targets->at(0)->Protector;
		_anims->push_back(Anim_ptr(new AnimJumpTo(_targets->at(1)->GetPos() - Vector3f(distance / 2.0f, 0, 0), _targets->at(0))));
		_anims->back()->_duration = 0.2f;
		_anims->back()->_async = true;
	}

	return BS_ActionProgress;
}

void SkillMelee::Update()
{
	double duration = -1;
	double progress = 0;
	if (_anims->size() > 0)
	{
		progress = _anims->front()->_progress;
		duration = _anims->front()->_duration;
	}

	HandleActionCommand(progress / duration);

	switch (_animProg)
	{
	case 0:
		if (!AnimationsDone())
			break;
		Animate();
		_animProg++;
		break;
	case 1:
		// DEAL DMG
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
		// JUMP BACK
		if (!AnimationsDone())
			break;
		_anims->push_back(Anim_ptr(new AnimJumpTo(_basePos, _owner)));
		// Someone is protecting
		if (_targets->size() > 1)
		{
			_anims->push_back(Anim_ptr(new AnimJumpTo(_targets->at(0)->BasePosition, _targets->at(0))));
			_anims->back()->_duration = 0.2f;
			_anims->back()->_async = true;
		}
		_animProg++;
		break;
	case 3:
		// SET THIS SKILL TO DONE
		if (!AnimationsDone())
			break;
		_done = true;
		break;
	}
	// Reset input
	_input = std::set<int>();
}

void SkillMelee::Animate()
{
	_anims->push_back(Anim_ptr(new AnimBasic(AE_Attack, _owner, 1)));
}

void SkillMelee::ApplyEffect()
{
	Damage result;
	result._value = 0;
	result._type = ST_Physical;
	//dmg = rand() % 3 + 1;
	_targets->at(0)->TakeDamage(result);

	// Damage text
	SpawnDamageText(_targets->at(0), result._value);
}

bool SkillMelee::TimingCondition(double progress, double duration)
{
	return progress > duration / 2;
}

bool SkillMelee::IsReady()
{
	return _currentCooldown <= 0;
}
