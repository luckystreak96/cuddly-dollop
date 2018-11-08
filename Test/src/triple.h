#ifndef TRIPLE_H__
#define TRIPLE_H__

#include <vector>
#include <tuple>

enum SkillProgress {
	SP_0_None, SP_1_Before_Anim, SP_2_BeginAnim, SP_3_DealDamage, SP_4_PostSkillAnim, SP_5_SkillDone, SP_SkillLast
};

enum AnimationOperation {
	AS_JumpTo, AS_JumpBack, AS_ColorFlash, AS_ScreenShake, AS_BonusEffect, AS_MoveTo, AS_Wait, AS_Animation, AS_FloatingText, AS_ParticleEffect,
	AC_CameraFollow, AC_CameraScale, AC_CameraCenter,
	AO_DamageParticle,
	AA_Start, AA_DealDamage, AA_ApplyEffect, AA_ChangeTarget
}; // AA_Start is there to be able to do > on the enum

enum AnimationArgument {
	AARG_Owner, AARG_Passive_Applier2Owner, AARG_Targets, AARG_FloatTargets, AARG_Target, AARG_OwnerTargets, AARG_Float, AARG_AsyncStart, AARG_FloatAsync
};

//using namespace std;
using floats = std::vector<float>;

struct triple
{
	triple(AnimationOperation animation, AnimationArgument arg_type, floats args) : _animation(animation), _arg_type(arg_type), _args(args) {}
	AnimationOperation _animation;
	AnimationArgument _arg_type;
	floats _args;
};

//using triple = std::tuple<AnimationOperation, AnimationArgument, floats>;

#endif // !TRIPLE_H__
