#include "skill.h"
#include "battleManager.h"
#include "fontFloat.h"
#include "GLFW/glfw3.h"

Skill::Skill()
{
	DefaultSetup();
}

void Skill::DefaultSetup()
{
	_cooldown = 0;
	_name = "SKILL?";
	_done = false;
	_skillType = ST_Physical;
	_currentCooldown = 0;
	_minTargets = 1;
	_targetMode = TM_Alive;
	_defaultTarget = DT_Self;
	_ac._start = 0;
	_ac._startHard = 1000;
	_ac._end = 0;
	_ac._success = false;
	_ac._animProg = 1;
	_ac._type = ACT_Defend;
	_critting = false;
}

int Skill::HandleDamage()
{
	int dmg = CalculateDamage();

	// Apply first damage modifications
	if (_skillType != ST_Healing)
		_targets->at(0)->DamageModifiers(dmg, _critting);

	if (_ac._success)
	{
		// Your team under attack from enemy -> Defense Action Command
		if (_owner->Team != 0 && _targets->at(0)->Team == 0)
		{
			if (_ac._type == ACT_Special)
				_targets->at(0)->SpecialActionCommand(dmg);
			else if (_skillType == ST_Physical)
				_targets->at(0)->PhysicalDefenseActionCommand(dmg);
			else
				_targets->at(0)->MagicalDefenseActionCommand(dmg);
		}
		// Your team attacking -> Offense Action Command
		else if (_owner->Team == 0)
		{
			if (_skillType == ST_Physical)
				_targets->at(0)->PhysicalOffenseActionCommand(dmg);
			else
				_targets->at(0)->MagicalOffenseActionCommand(dmg);
		}
	}

	// Deal the dmg
	if (_skillType == ST_Healing)
		_targets->at(0)->ApplyHealing(dmg);
	else
		_targets->at(0)->TakeDamage(dmg);

	return dmg;
}

void Skill::CheckActionCommand(double percentProgress)
{
	// Did you input anything
	if (_input.size() > 0 && (_input.count(GLFW_KEY_X) || _input.count(GLFW_KEY_Z)))
	{
		// Are you in a situation to try an action command
		if (_ac._tried || _ac._success)
			return;
		else
		{
			if (percentProgress <= _ac._end)
			{
				if (_input.count(GLFW_KEY_Z))
				{
					if (percentProgress >= _ac._startHard)
					{
						_ac._type = ACT_Special;
						_ac._success = true;
					}
				}
				else if (_input.count(GLFW_KEY_X))
				{

					if (percentProgress >= _ac._start)
					{
						_ac._type = ACT_Defend;
						_ac._success = true;
					}
				}
			}
		}

		_ac._tried = true;
	}
}

// Must return the new state
BattleState Skill::Start(std::vector<Actor_ptr>* targets, std::deque<Actor_ptr>* actors, std::deque<Anim_ptr>* anims, Actor_ptr owner)
{
	_ac._tried = false;
	_ac._success = false;
	_critting = false;
	_owner = owner;
	_targets = targets;
	_actors = actors;
	_anims = anims;
	_animProg = 0;
	return BS_ActionDone;
}

void Skill::HandleActionCommand(double percentProgress)
{
	// Handle Action command stuff
	if (_ac._animProg == _animProg)
	{
		if (percentProgress <= _ac._end && percentProgress >= _ac._startHard)
			_owner->SetColorAll(Vector3f(3.0f, 3.0f, 3.0f), 1.0f);
		else if (percentProgress <= _ac._end && percentProgress >= _ac._start)
			_owner->SetColorAll(Vector3f(2.0f, 2.0f, 2.0f), 1.0f);
		else
			_owner->SetColor();

		// Handle input for action command
		// Can only happen if you press within the right animation
		CheckActionCommand(percentProgress);
	}
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
	if (_critting)
		font->SetScale(0.75f, 0.75f);
	font->SetText((dmg < 0 || _skillType == ST_Healing ? "+" : "") + std::to_string(dmg), pos, true);

	// color
	Vector3f color;
	if (_critting)
		color = Vector3f(0.35f, 0.31f, 0.87f);
	else if (dmg >= 0 && _skillType != ST_Healing)
		color = Vector3f(1, 0, 0);
	else
		color = Vector3f(0, 1, 0);
	dynamic_cast<FontFloat*>(font.get())->Color = color;

	FontManager::GetInstance().AddFont(font);
}

bool Skill::AnimationsDone()
{
	return !_anims->size();
}

