#include "battleAnimationManager.h"
#include "fontFloat.h"
#include "fontManager.h"
#include "animMoveTo.h"
#include "animJumpTo.h"
#include "animBasic.h"
#include "animWait.h"
#include "animScreenShake.h"
#include "animColorFlash.h"
#include "particleManager.h"
#include "soundManager.h"

using namespace std;

BattleAnimationManager::BattleAnimationManager()
{

}

void BattleAnimationManager::UpdateColors(int fighterid, bool selected, bool dead, int actionCommandLevel)
{
	m_actors.at(fighterid)->UpdateColor(dead, selected, actionCommandLevel);
}

void BattleAnimationManager::MoveUp(int fighterid, bool foreward)
{
	Actor_ptr owner = m_actors.at(fighterid);
	if (foreward)
	{
		Anim_ptr move1 = Anim_ptr(new AnimMoveTo(owner->GetPos() + Vector3f(fighterid < 4 ? 1.f : -1, 0, 0), owner));
		//if (m_singleFileAttacks) // We dont want async movement when there's too many attacks gonna happen
		move1->_async = false;
		m_animations.push_back(move1);
	}
	else
	{
		Anim_ptr move2 = Anim_ptr(new AnimMoveTo(owner->WaitPosition, owner));
		m_animations.push_back(move2);
	}
}

// Return whether or not any non-async animations are running
bool BattleAnimationManager::Animating()
{
	for (auto& x : m_animations)
		if (!x->_async)
			return true;

	return false;
}

double BattleAnimationManager::get_animation_progress()
{
	double duration = -1;
	double progress = 0;
	if (m_animations.size() > 0)
	{
		int counter = 0;
		for (auto& x : m_animations)
			if (!x->_async)
				counter++;
		if (counter <= 1)
		{
			progress = m_animations.front()->_progress;
			duration = m_animations.front()->_duration;
		}
	}
	return progress / duration;
}


void BattleAnimationManager::DamageAnimation(int target, Skill_ptr skill, Damage dmg)
{
	// Damage text
	SpawnDamageText(m_actors.at(target), dmg, skill);

	// Color flash
	m_animations.push_front(Anim_ptr(new AnimColorFlash(Vector3f(5, 3, 3), m_actors[target])));

	// Particles
	Vector3f pos = m_actors[target]->GetPos() + Vector3f(0.5f, 0.5f, 0.6f);
	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
	particles->SetPowerLevel(0.3f);
	particles->Init(PT_Explosion, abs(dmg._value), pos, false, "star.png");
	particles->SetColor(CalculateDamageColor(skill, dmg));
	ParticleManager::GetInstance().AddParticles(particles);

	// Sound fx
	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}

Vector3f BattleAnimationManager::CalculateDamageColor(Skill_ptr skill, Damage dmg)
{
	return dmg._critting ? Vector3f(0.35f, 0.31f, 0.87f) : Vector3f(1.0f, 0.2f, 0.2f);
}

Vector3f BattleAnimationManager::CalculateTextColor(Skill_ptr skill, Damage dmg)
{
	// color
	Vector3f color;
	if (dmg._critting)
		// purpleish
		color = skill->_ac._success ? Vector3f(0.85f, 0.23f, 0.54f) : Vector3f(0.35f, 0.31f, 0.87f);
	else if (dmg._value >= 0 && skill->_skillType != ST_Healing && dmg._type != ST_Bonus)
		// redish / yellowish
		color = skill->_ac._success ? Vector3f(1, 0.8f, 0) : Vector3f(1, 0, 0);
	else if (dmg._value > 0 && skill->_skillType == ST_Healing || dmg._value < 0 && dmg._type == ST_Bonus)
		// Greenish
		color = skill->_ac._success ? Vector3f(0, 0.95f, 0.6f) : Vector3f(0, 1, 0);
	else if (dmg._type == ST_Bonus)
		color = Vector3f(1, 1.f, 1.f);

	return color;
}


Anim_ptr BattleAnimationManager::CreateAnimation(triple ao)
{
	AnimationOperation op = get<0>(ao);
	AnimationArgument aa = get<1>(ao);
	floats args = get<2>(ao);
	Anim_ptr result;
	Vector3f pos;
	Actor_ptr actor;
	float distance;

	switch (get<0>(ao))
	{
	case AS_JumpTo:
		actor = m_actors.at(args.at(0));
		distance = actor->GetId() > 3 ? -0.7f : 0.7f;
		result = Anim_ptr(new AnimJumpTo(actor->GetPos() + Vector3f(distance, 0, 0), m_actors.at(_owner)));
		break;
	case AS_JumpBack:
		result = Anim_ptr(new AnimJumpTo(true, m_actors.at(_owner)));
		break;
	case AS_ColorFlash:
		break;
	case AS_ScreenShake:
		result = Anim_ptr(new AnimScreenShake(args[0], args[1]));
		break;
	case AS_BonusEffect:
		break;
	case AS_MoveTo:
		break;
	case AS_Wait:
		result = Anim_ptr(new AnimWait(args.at(0)));
		break;
	case AS_Animation:
		result = Anim_ptr(new AnimBasic((Anim_Enum)(int)args.at(0), m_actors.at(_owner), args.at(1)));
		result->_async = aa >= AARG_AsyncStart;
		break;
	case AS_FloatingText:
		break;
	case AC_CameraFollow:
		for (auto& x : args)
			pos += m_actors.at(x)->GetPosRef();
		pos /= args.size();
		Camera::_currentCam->SetFollowCenteredXY(pos);
		break;
	case AC_CameraScale:
		Camera::_currentCam->SetScale(Vector3f(args.at(0), args.at(0), 0));
		break;
	case AC_CameraCenter:
		Camera::_currentCam->SetFollowCenteredXY(Camera::_currentCam->MapCenter());
		break;
	case AA_Start:
		break;
	case AA_DealDamage:
		break;
	default:
		break;
	}

	return result;
}

void BattleAnimationManager::UpdateAnimations()
{
	if (m_animations.size() > 0)
	{
		bool nonAsyncHit = false;
		for (int i = 0; i < m_animations.size() && !nonAsyncHit; i++)
		{
			if (m_animations.at(i)->_async || i == 0 || (!m_animations.at(i)->_async && !nonAsyncHit))
				m_animations.at(i)->Update();
			if (!m_animations.at(i)->_async)
				nonAsyncHit = true;
			if (m_animations.at(i)->_done)
			{
				m_animations.erase(m_animations.begin() + i);
				i--;
			}
		}
	}
}

void BattleAnimationManager::CreateFloatingText(int fighterid, std::string text)
{
	FontManager::GetInstance().CreateFloatingText(m_actors.at(fighterid)->GetPosRef(), text);
}

void BattleAnimationManager::insert_animation(Anim_ptr anim)
{
	m_animations.push_back(anim);
}


void BattleAnimationManager::SpawnStatusText(Actor_ptr target, std::string statusText)
{
	Vector3f pos;
	Font_ptr font;

	// Setup font
	pos = target->GetPos() + Vector3f(0.5f, 1.f, 0);
	pos.z = 0;

	// create font
	font = Font_ptr(new FontFloat(0.7));
	font->SetText(statusText, pos, true);

	// color
	dynamic_cast<FontFloat*>(font.get())->Color = Vector3f(0, 0.4f, 0.9f);

	FontManager::GetInstance().AddFont(font);
}

void BattleAnimationManager::SpawnDamageText(Actor_ptr target, Damage dmg, Skill_ptr skill)
{
	Vector3f pos;
	Font_ptr font;

	// Setup font
	pos = target->GetPos() + Vector3f(0.5f, dmg._type == ST_Bonus ? 0.75f : 1.f, 0);
	pos.z = 0;

	// create font
	font = Font_ptr(new FontFloat(0.7));
	if (skill->_critting)
		font->SetScale(0.75f, 0.75f);
	else if (dmg._type == ST_Bonus)
		font->SetScale(1.0f, 1.0f);
	font->SetText((dmg._value < 0 || skill->_skillType == ST_Healing ? "+" : "") + std::to_string(dmg._value), pos, true);

	dynamic_cast<FontFloat*>(font.get())->Color = CalculateTextColor(skill, dmg);

	FontManager::GetInstance().AddFont(font);
}