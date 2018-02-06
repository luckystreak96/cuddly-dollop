#include "skill.h"
#include "battleManager.h"
#include "fontFloat.h"

Skill::Skill()
{
	DefaultSetup();
}

void Skill::DefaultSetup()
{
	_cooldown = 0;
	_name = "SKILL?";
	_done = false;
	_currentCooldown = 0;
	_minTargets = 1;
	_targetMode = TM_Alive;
	_defaultTarget = DT_Self;
}

// Must return the new state
BattleState Skill::Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
{
	_owner = owner;
	_targets = targets;
	_actors = actors;
	_anims = anims;
	//std::cout << "Did nothing to: " << targets->at(0)->Name << std::endl;
	return BS_ActionDone;
}

void Skill::Update()
{
	// Reset input
	_input = std::set<int>();
}

void Skill::Reset()
{
	_done = false;
}

bool Skill::IsReady()
{
	return _currentCooldown <= 0;
}

void Skill::SpawnStatusText(Actor_ptr target, std::string statusText)
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

void Skill::SpawnDamageText(Actor_ptr target, int dmg)
{
	Vector3f pos;
	Font_ptr font;

	// Setup font
	pos = target->GetPos() + Vector3f(0.5f, 1.f, 0);
	pos.z = 0;

	// create font
	font = Font_ptr(new FontFloat(0.7));
	font->SetText((dmg < 0 ? "+" : "") + std::to_string(-dmg), pos, true);

	// color
	dynamic_cast<FontFloat*>(font.get())->Color = dmg > 0 ? Vector3f(1.0f, 0, 0) : Vector3f(0, 1.f, 0);

	FontManager::GetInstance().AddFont(font);
}

bool Skill::AnimationsDone()
{
	return !_anims->size();
}

