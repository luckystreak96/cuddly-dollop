#include "skillMelee.h"
#include "animBasic.h"

void SkillMelee::DefaultSetup()
{
	//Skill::DefaultSetup();
	_name = "Skill Melee -_-";
}

SkillMelee::SkillMelee() : Skill()
{
	DefaultSetup();
}


// Must return the new state
//void SkillMelee::Setup()
//{
//	Skill::Setup();
//}

//void SkillMelee::Start()
//{
//	Skill::Start();
//	_basePos = _owner.lock()->_Graphics->GetPos();
//	// INSERT JUMP FOREWARD ANIMATION HERE
//	float distance = _owner.lock()->_Fighter->Team == 0 ? 0.7f : -0.7f;
//	bool protector = _targets.at(0)->_Fighter->Protector != NULL && !_targets.at(0)->_Fighter->Protector->_Fighter->Dead;
//	if (protector)
//		distance *= 2.0f;
//	_anims->push_back(Anim_ptr(new AnimJumpTo(_targets.at(0)->_Graphics->GetPos() - Vector3f(distance, 0, 0), _owner.lock())));
//	if (protector)
//	{
//		Skill::SetupProtector();
//		_anims->push_back(Anim_ptr(new AnimJumpTo(_targets.at(1)->_Graphics->GetPos() - Vector3f(distance / 2.0f, 0, 0), _targets.at(0))));
//		_anims->back()->_duration = 0.2f;
//		_anims->back()->_async = true;
//	}
//}

Damage SkillMelee::CalculateDamage(StatUser& user)
{
	Damage dmg;
	dmg._value = 4;
	return dmg;
}

void SkillMelee::SetAnimations()
{
	switch (m_progress)
	{
	case 1:
		m_state = SP_1_Before_Anim;// warn others that we're coming
		break;
	case 2:
		m_state = SP_2_BeginAnim;
		m_animationBuffer.push_back(triple(AC_CameraFollow, AARG_Targets, floats({0}))); // follow the target async
		m_animationBuffer.push_back(triple(AC_CameraScale, AARG_Float, floats({ 1.5f }))); // zoom 1.5
		m_animationBuffer.push_back(triple(AS_JumpTo, AARG_Target, floats({ 0 }))); // jump to target
		m_animationBuffer.push_back(triple(AS_Animation, AARG_FloatAsync, floats({ AE_Attack, 0.6f }))); // smack animation async (animation, seconds)
		m_animationBuffer.push_back(triple(AS_Wait, AARG_Float, floats({ 0.3f }))); // wait half the animation time
		break;
	case 3:
		m_state = SP_3_DealDamage;
		m_animationBuffer.push_back(triple(AS_ScreenShake, AARG_Float, floats({2.f, 0.2f}))); // shake screen, amount of shake and time
		m_animationBuffer.push_back(triple(AA_DealDamage, AARG_Target, floats())); // Deal damage
		m_animationBuffer.push_back(triple(AS_FloatingText, AARG_Float, floats())); // Spawn damage text
		m_animationBuffer.push_back(triple(AS_Wait, AARG_Float, floats({ 0.3f }))); // wait other half of animation time
		break;
	case 4:
		m_state = SP_4_PostSkillAnim;
		m_animationBuffer.push_back(triple(AS_JumpBack, AARG_Target, floats())); // jump back using animJumpTo's new constructor that just jumps back to active position
		m_animationBuffer.push_back(triple(AC_CameraScale, AARG_Float, floats({ 1.0f }))); // zoom 1.0 async
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

//SkillProgress SkillMelee::Update()
//{
//	double duration = -1;
//	double progress = 0;
//	if (_anims->size() > 0)
//	{
//		progress = _anims->front()->_progress;
//		duration = _anims->front()->_duration;
//	}
//
//	HandleActionCommand(progress / duration);
//
//	if(Transformation::perspectiveOrtho)
//		Camera::_currentCam->SetScale(Vector3f(1.5f));
//
//	switch (_animProg)
//	{
//	case 0:
//		Camera::_currentCam->SetFollowCenteredXY(_targets.at(0)->_Graphics->GetPosRef());
//		if (!AnimationsDone())
//			break;
//		Animate();
//		_animProg++;
//		break;
//	case 1:
//		// DEAL DMG
//		Camera::_currentCam->SetFollowCenteredXY(_targets.at(0)->_Graphics->GetPosRef());
//		if (AnimationsDone())
//		{
//			_animProg++;
//		}
//		else
//		{
//			progress = _anims->front()->_progress;
//			duration = _anims->front()->_duration;
//			if (TimingCondition(progress, duration))
//			{
//				ApplyEffect();
//				_animProg++;
//			}
//		}
//		break;
//	case 2:
//		// JUMP BACK
//		Camera::_currentCam->SetScale(Vector3f(1.f));
//		Camera::_currentCam->SetFollowCenteredXY(_owner.lock()->_Graphics->GetPosRef());
//		//Camera::_currentCam->SetFollowCenteredXY(_owner->BasePosition);
//		if (!AnimationsDone())
//			break;
//		_anims->push_back(Anim_ptr(new AnimJumpTo(_basePos, _owner.lock())));
//		// Someone is protecting
//		if (_targets.size() > 1)
//		{
//			_anims->push_back(Anim_ptr(new AnimJumpTo(_targets.at(0)->WaitPosition, _targets.at(0))));
//			_anims->back()->_duration = 0.2f;
//			_anims->back()->_async = true;
//		}
//		_animProg++;
//		break;
//	case 3:
//		// SET THIS SKILL TO DONE
//		Camera::_currentCam->SetScale(Vector3f(1.f));
//		Camera::_currentCam->SetFollowCenteredXY(_owner.lock()->_Graphics->GetPosRef());
//		if (!AnimationsDone())
//			break;
//		_done = true;
//		break;
//	}
//	// Reset input
//	_input = std::set<int>();
//
//	return m_state;
//}

//void SkillMelee::Animate()
//{
//	_anims->push_back(Anim_ptr(new AnimBasic(AE_Attack, _owner.lock(), 1)));
//}

//void SkillMelee::ApplyEffect()
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

bool SkillMelee::TimingCondition(double progress, double duration)
{
	return progress > duration / 2;
}

bool SkillMelee::IsReady()
{
	return _currentCooldown <= 0;
}
