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
	_name = "Virtual class";
}

// Must return the new state
BattleState SkillMelee::Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
{
	_animProg = 0;
	_owner = owner;
	_actors = actors;
	_anims = anims;
	_targets = targets;
	_basePos = _owner->GetPos();
	// INSERT JUMP FOREWARD ANIMATION HERE
	_anims->push_back(Anim_ptr(new AnimJumpTo(_targets->at(0)->GetPos() - Vector3f(_owner->Team == 0 ? 0.7f : -0.7f, 0, 0), _owner)));

	return BS_ActionProgress;
}

void SkillMelee::Update()
{
	double duration;
	double progress;
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
			if(TimingCondition(progress, duration))
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
	int dmg = 0;
	//dmg = rand() % 3 + 1;
	_targets->at(0)->TakeDamage(dmg);

	// Damage text
	SpawnDamageText(_targets->at(0), dmg);
}

bool SkillMelee::TimingCondition(double progress, double duration)
{
	return progress > duration / 2;
}

bool SkillMelee::IsReady()
{
	return _currentCooldown <= 0;
}
