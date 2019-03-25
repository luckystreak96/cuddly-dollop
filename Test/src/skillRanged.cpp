#include "skillRanged.h"
#include "animBasic.h"

SkillRanged::SkillRanged() : Skill()
{
	DefaultSetup();
}

void SkillRanged::DefaultSetup()
{
	//Skill::DefaultSetup();
	_name = "Skill Ranged -_-";
	//_targetProgress = 0;
}

Damage SkillRanged::CalculateDamage(StatUser& user)
{
	Damage dmg;
	dmg._value = 17;
	return dmg;
}

void SkillRanged::SetAnimations()
{
	switch (m_progress)
	{
	case 1:
		m_state = SP_1_Before_Anim;// warn others that we're coming
		break;
	case 2:
		m_state = SP_2_BeginAnim;
		m_animationBuffer.push_back(triple(AC_CameraFollow, AARG_OwnerTargets, floats({ 0 }))); // follow the targets and player
		m_animationBuffer.push_back(triple(AC_CameraScale, AARG_Float, floats({ 1.5f }))); // zoom 1.5
		m_animationBuffer.push_back(triple(AS_MoveTo, AARG_Owner, floats({ 0, 1.f * 64.f, 0 }))); // move self forward (1, 0)
		m_animationBuffer.push_back(triple(AS_Animation, AARG_FloatAsync, floats({ AE_Attack, 1.0f }))); // smack animation async (animation, seconds)
		m_animationBuffer.push_back(triple(AS_Wait, AARG_Float, floats({ 0.5f }))); // wait half the animation time
		break;
	case 3:
		m_state = SP_3_DealDamage;
		m_animationBuffer.push_back(triple(AS_ScreenShake, AARG_Float, floats({ 0.5f, 0.1f }))); // shake screen, amount of shake and time
		m_animationBuffer.push_back(triple(AA_DealDamage, AARG_Targets, floats())); // Deal damage
		m_animationBuffer.push_back(triple(AS_FloatingText, AARG_Float, floats())); // Spawn damage text
		m_animationBuffer.push_back(triple(AS_Wait, AARG_Float, floats({ 0.5f }))); // wait other half of animation time
		break;
	case 4:
		m_state = SP_4_PostSkillAnim;
		//m_animationBuffer.push_back(triple(AS_JumpBack, AARG_Target, floats())); // jump back using animJumpTo's new constructor that just jumps back to active position
		m_animationBuffer.push_back(triple(AC_CameraScale, AARG_Float, floats({ 1.2f }))); // zoom 1.0 async
		m_animationBuffer.push_back(triple(AC_CameraFollow, AARG_Owner, floats()));; // follow owner
		break;
	case 5:
		m_state = SP_5_SkillDone;
		_done = true;
		m_animationBuffer.push_back(triple(AC_CameraCenter, AARG_Targets, floats()));; // center camera
		break;
	default:
		break;
	}
	ModifyAnimations();
}

// Must return the new state
//BattleState SkillRanged::Setup(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
//{
//	Skill::Setup(targets, actors, anims, owner);
//
//	return BS_ActionProgress;
//}

//void SkillRanged::Start()
//{
//	Skill::Start();
//	_basePos = _owner.lock()->_Graphics->get_position();
//
//	_anims->push_back(Anim_ptr(new AnimMoveTo(_owner.lock()->_Graphics->get_position_ref() + Vector3f(_owner.lock()->_Fighter->Team ? -1 : 1, 0, 0), _owner.lock())));
//
//	// Reset for re-use
//	_cameraTarget = Vector3f();
//
//	// Get average of all targets and attacker
//	for (auto& x : _targets)
//		_cameraTarget += x->_Graphics->get_position_ref();
//
//	_cameraTarget.x += _owner.lock()->_Graphics->get_position_ref().x;
//	_cameraTarget /= (float)_targets.size();
//
//	_targetProgress = 0;
//}

//void SkillRanged::Update()
//{
//	double duration = -1;
//	double progress = 0;
//	if (_anims->size() > 0)
//	{
//		progress = _anims->front()->_progress;
//		duration = _anims->front()->_duration;
//	}
//
//	if(_anims->size() && dynamic_cast<AnimBasic*>(_anims->front().get()) != NULL)
//		HandleActionCommand(progress / duration);
//
//	Camera::_currentCam->SetScale(Vector3f(1.10f));
//
//	switch (_animProg)
//	{
//	case 0:
//		Camera::_currentCam->SetFollowCenteredXY(_cameraTarget);
//		if (!AnimationsDone())
//			break;
//		Animate();
//		_animProg++;
//		break;
//	case 1:
//		// DEAL DMG
//		if (AnimationsDone())
//		{
//			if (_targetProgress == _targets.size())
//			{
//				// walk back at a decent speed
//				_anims->push_back(Anim_ptr(new AnimMoveTo(_owner.lock()->WaitPosition, _owner.lock(), 1)));
//				_animProg++;
//			}
//			else
//				ApplyEffect();
//		}
//		else if(_targetProgress == 0)
//		{
//			progress = _anims->front()->_progress;
//			duration = _anims->front()->_duration;
//			if (TimingCondition(progress, duration))
//			{
//				// Allow the rest of the animations to go on without a hitch
//				if(_anims->size())
//					_anims->front()->_async = true;
//				Camera::_currentCam->SetFollowCenteredXY(_cameraTarget);
//				ApplyEffect();
//			}
//		}
//		break;
//	case 2:
//		// SET THIS SKILL TO DONE
//		Camera::_currentCam->SetScale(Vector3f(1.f));
//		Camera::_currentCam->SetFollowCenteredXY(_owner.lock()->_Graphics->get_position_ref());
//		if (!AnimationsDone())
//			break;
//		_done = true;
//		break;
//	}
//	// Reset input
//	_input = std::set<int>();
//}
//
//void SkillRanged::Animate()
//{
//	_anims->push_back(Anim_ptr(new AnimBasic(AE_Attack, _owner.lock(), 1.0)));
//}
//
//void SkillRanged::ApplyEffect()
//{
//	Damage result;
//	result._value = 0;
//	result._type = ST_Physical;
//	//dmg = rand() % 3 + 1;
//	_targets.at(0)->_Fighter->TakeDamage(result);
//
//	// Damage text
//	SpawnDamageText(_targets.at(0), result);
//}

bool SkillRanged::TimingCondition(double progress, double duration)
{
	return progress > duration / 2;
}

bool SkillRanged::IsReady()
{
	return _currentCooldown <= 0;
}
