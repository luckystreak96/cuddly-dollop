#include "battleAnimationManager.h"
#include "fontFloat.h"
#include "fontManager.h"
#include "animMoveTo.h"
#include "animJumpTo.h"
#include "animBasic.h"
#include "animWait.h"
#include "animScreenShake.h"
#include "animColorFlash.h"
#include "anim_particle_effect.h"
#include "particleManager.h"
#include "soundManager.h"
#include "renderer.h"
#include "localizationData.h"
#include "colors.h"

using namespace std;

BattleAnimationManager::BattleAnimationManager() : m_skill_display(SDS_none), m_particleSprite(PS_Star)
{
}

void BattleAnimationManager::UpdateColors(int fighterid, bool selected, bool dead, int actionCommandLevel)
{
	m_actors.at(fighterid)->UpdateColor(dead, selected, actionCommandLevel);
}

void BattleAnimationManager::update_skill_display(std::vector<Skill_ptr>* skills, skill_display_state state)
{
	if (state == SDS_choose_skill)
	{
		// Create a list of fonts that is long enough for all the skills
		while (m_fonts.size() < skills->size())
			m_fonts.push_back(FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png"));

		// Set the text of each font according to skill text
		for (unsigned int i = 0; i < skills->size(); i++)
		{
			FontManager::GetInstance().EnableFont(m_fonts[i]);
			FontManager::GetInstance().SetScale(m_fonts[i], 0.5f, 0.5f);
			FontManager::GetInstance().SetText(m_fonts[i], _(skills->at(i)->_name), Vector3f(5.0f, 4.75f - i * 0.5f, 0));
			//FontManager::GetInstance().GetFont(m_info._fonts[i])->SetText(m_info._chooseSkill->at(i)->_name, Vector3f(6, 7 - i, 1));
		}
	}
	else
	{
		for (unsigned int i = 0; i < m_fonts.size(); i++)
			FontManager::GetInstance().GetFont(m_fonts[i])->_enabled = false;
	}
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
	particles->Init(PT_Explosion, abs(dmg._value), pos, false, SpriteFromEnum.at((ParticleSprite)m_particleSprite));
	particles->SetColor(CalculateDamageColor(skill, dmg));
	ParticleManager::GetInstance().AddParticles(particles);

	// Sound fx
	SoundManager::GetInstance().PlaySoundFX("res/audio/fx/swish_2.wav");
}

void BattleAnimationManager::generate_particles(int amount, Vector3f color, Vector3f position)
{
	// Particles
	Vector3f pos = position;
	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
	particles->SetPowerLevel(0.3f);
	particles->Init(PT_Explosion, amount, pos, false, SpriteFromEnum.at((ParticleSprite)m_particleSprite));
	particles->SetColor(color);
	ParticleManager::GetInstance().AddParticles(particles);
}

Vector3f BattleAnimationManager::CalculateDamageColor(Skill_ptr skill, Damage dmg)
{
	// color
	Vector3f color;
	if (dmg._critting)
		// purpleish
		color += Vector3f(0.35f, 0.31f, 0.87f);

	if (dmg._type != ST_Healing)
	{
		if (skill->_ac._success && skill->_ac._type != ACT_Defend ||
			!skill->_ac._success && skill->_ac._type == ACT_Defend)
			color += Vector3f(1.0f, 0.0f, 0.0f);
		else
			color += Vector3f(1.0f, 0.2f, 0.2f);
	}
	else
	{
		if (skill->_ac._success)
			color += Vector3f(0, 1, 0);
		else
			color += Vector3f(0.2f, 1, 0.2f);
	}

	if (dmg._type == ST_Bonus)
		color += 1.0f;

	return color;
	//return dmg._critting ? Vector3f(0.35f, 0.31f, 0.87f) : Vector3f(1.0f, 0.2f, 0.2f);
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

	if (skill->_ac._success && skill->_ac._type == ACT_Defend)
		color += 0.2f;

	return color;
}


Anim_ptr BattleAnimationManager::CreateAnimation(triple ao)
{
	AnimationOperation op = ao._animation;
	//AnimationOperation op = get<0>(ao);
	AnimationArgument aa = ao._arg_type;
	//AnimationArgument aa = get<1>(ao);
	floats args = ao._args;
	//floats args = get<2>(ao);
	Anim_ptr result;
	Vector3f pos;
	Actor_ptr actor;
	float distance;
	int counter = 0;
	int owner = _owner;
	bool boolean = true;

	switch (op)
	{
	case AS_JumpTo:
		if (args.size() > 1)
		{
			owner = args[0];
			actor = m_actors[args[1]];
		}
		else
		{
			actor = m_actors.at(args.at(0));
		}
		distance = actor->GetId() > 3 ? -0.7f : 0.7f;
		// if on same team, go behind them instead
		if (m_actors.at(owner)->GetId() / 4 == actor->GetId() / 4 && args.size() <= 1)
			distance = -distance;
		result = Anim_ptr(new AnimJumpTo(actor->GetPos() + Vector3f(distance, 0, 0), m_actors.at(owner)));
		break;
	case AS_JumpBack:
		if (aa == AARG_Passive_Applier2Owner)
		{
			boolean = false;
			owner = args[0];
		}
		result = Anim_ptr(new AnimJumpTo(boolean, m_actors.at(owner)));
		break;
	case AS_ColorFlash:
		break;
	case AS_ScreenShake:
		result = Anim_ptr(new AnimScreenShake(args[0], args[1]));
		break;
	case AS_ParticleEffect:
		result = Anim_ptr(new anim_particle_effect(args[0], colors::green, &m_actors[owner]->GetPosRef(), "leaf.png"));
		//generate_particles(args[0], colors::black, m_actors[owner]->GetPosRef());
		break;
	case AS_BonusEffect:
		break;
	case AS_MoveTo:
		if (aa == AARG_Owner)
			actor = m_actors.at(_owner);
		else
			actor = m_actors.at(args[0]);

		result = Anim_ptr(new AnimMoveTo(actor->GetPos() + Vector3f(args[1], args[2], 0), actor));
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
	case AO_DamageParticle:
		m_particleSprite = args[0];
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

void BattleAnimationManager::set_last_animation_non_async()
{
	m_animations.back()->_async = false;
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

void BattleAnimationManager::SetupHUD(BattleUnit unit)
{
	unit.position = &m_actors.at(unit.id)->GetPosRef();
	unit.aobservers = &m_actors.at(unit.id)->_observers;
	m_hud.AddUnit(unit);
}

void BattleAnimationManager::UpdateHUD()
{
	m_hud.Update();
}

void BattleAnimationManager::ExpAnimation(int id, int xp)
{
	insert_animation(m_hud.GetActorHealthBar(id)->SetupExpAnimation(xp));
	CreateFloatingText(id, "+" + std::to_string(xp) + " XP");
}

void BattleAnimationManager::SetRender()
{
	m_hud.SetRender();
}

