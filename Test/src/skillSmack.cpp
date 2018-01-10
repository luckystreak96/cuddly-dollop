#include "skillSmack.h"
#include "battleManager.h"
#include "animJumpTo.h"
#include "fontManager.h"
#include "fontFloat.h"

SkillSmack::SkillSmack()
{
	Skill::DefaultSetup();
	DefaultSetup();
}

void SkillSmack::DefaultSetup()
{
	_name = "Smack";
}

// Must return the new state
BattleState SkillSmack::Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
{
	_animProg = 0;
	_owner = owner;
	_actors = actors;
	_anims = anims;
	_targets = targets;
	_basePos = _owner->GetPos();
	// INSERT JUMP FOREWARD ANIMATION HERE
	_anims->push_back(Anim_ptr(new AnimJumpTo(_targets->at(0)->GetPos() - Vector3f(_owner->Team == 0 ? 1 : -1, 0, 0), _owner)));

	return BS_ActionProgress;
}

void SkillSmack::Update()
{
	int dmg = 0;
	unsigned int fontId;
	Vector3f pos;
	Font_ptr font;
	switch (_animProg)
	{
	case 0:
		// DEAL DMG
		dmg = rand() % 3 + 1;
		std::cout << "Smacked: " << _targets->at(0)->Name << " for " << dmg << " damage" << std::endl;
		_targets->at(0)->TakeDamage(dmg);
		std::cout << _targets->at(0)->Name << " has " << _targets->at(0)->Health << " HP left" << std::endl << std::endl;
		_animProg++;

		// Setup font
		pos = _targets->at(0)->GetPos() + Vector3f(0.5f, 1.f, 0);
		pos.z = 0;

		font = Font_ptr(new FontFloat(0.7));
		font->SetText(std::to_string(-dmg), pos, true);
		// color
		if (dmg > 0)
			dynamic_cast<FontFloat*>(font.get())->Color = Vector3f(1.0f, 0, 0);
		fontId = FontManager::GetInstance().AddFont(font);
		//FontManager::GetInstance().SetText(fontId, std::to_string(-dmg), pos, true);
		break;
	case 1:
		// JUMP BACK
		_anims->push_back(Anim_ptr(new AnimJumpTo(_basePos, _owner)));
		_animProg++;
		break;
	case 2:
		// SET THIS SKILL TO DONE
		_done = true;
		break;
	}
	// Reset input
	_input = std::set<int>();
}

bool SkillSmack::IsReady()
{
	return _currentCooldown <= 0;
}
