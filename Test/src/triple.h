#ifndef TRIPLE_H__
#define TRIPLE_H__

#include <vector>
#include <tuple>

enum SkillProgress {
	SP_0_None, SP_1_Before_Anim, SP_2_BeginAnim, SP_3_DealDamage, SP_4_PostSkillAnim, SP_5_SkillDone
};

enum AnimationOperation {
	AS_JumpTo, AS_JumpBack, AS_ColorFlash, AS_ScreenShake, AS_BonusEffect, AS_MoveTo, AS_Wait, AS_Animation, AS_FloatingText,
	AC_CameraFollow, AC_CameraScale, AC_CameraCenter,
	AO_DamageParticle,
	AA_Start, AA_DealDamage, AA_ApplyEffect, AA_ChangeTarget
}; // AA_Start is there to be able to do > on the enum

enum AnimationArgument {
	AARG_Owner, AARG_Passive2Owner, AARG_Targets, AARG_FloatTargets, AARG_Target, AARG_OwnerTargets, AARG_Float, AARG_AsyncStart, AARG_FloatAsync
};

//using namespace std;
using floats = std::vector<float>;
using triple = std::tuple<AnimationOperation, AnimationArgument, floats>;

#endif // !TRIPLE_H__
