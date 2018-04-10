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
	_skillUpgradeProgress = 0;
	_skillUpgradeMax = 0;
	_isPreCalculated = false;
}

Damage Skill::HandleDamage()
{
	Damage dmg = _preCalculatedDamage;
	if(!_isPreCalculated)
		dmg = CalculateDamage();

	// Apply first damage modifications
	if (_skillType != ST_Healing)
		_targets.at(0)->_Fighter->DamageModifiers(dmg, _critting);

	if (_ac._success)
	{
		// Your team under attack from enemy -> Defense Action Command
		if (_owner->_Fighter->Team != 0 && _targets.at(0)->_Fighter->Team == 0)
		{
			if (_ac._type == ACT_Special)
				_targets.at(0)->_Fighter->SpecialActionCommand(dmg);
			else if (_skillType == ST_Physical)
				_targets.at(0)->_Fighter->PhysicalDefenseActionCommand(dmg);
			else
				_targets.at(0)->_Fighter->MagicalDefenseActionCommand(dmg);
		}
		// Your team attacking -> Offense Action Command
		else if (_owner->_Fighter->Team == 0)
		{
			if (_skillType == ST_Physical)
				_targets.at(0)->_Fighter->PhysicalOffenseActionCommand(dmg);
			else
				_targets.at(0)->_Fighter->MagicalOffenseActionCommand(dmg);
		}
	}

	// Deal the dmg
	if (_skillType == ST_Healing)
		_targets.at(0)->_Fighter->ApplyHealing(dmg);
	else
		_targets.at(0)->_Fighter->TakeDamage(dmg);

	return dmg;
}


bool Skill::ValidateTargets()
{
	bool valid = false;

	// Check if all targets are alive
	for (auto& x : _targets)
		if (x->_Fighter->RespectsTargeting(_owner.get(), _targetMode))
		{
			valid = true;
			break;
		}

	return valid;
}

void Skill::CheckActionCommand(double percentProgress)
{
	// Did you input anything
	if (_input.size() > 0 && (_input.count(A_Accept) || _input.count(A_Cancel)))
	{
		// Are you in a situation to try an action command
		if (_ac._tried || _ac._success)
			return;
		else
		{
			if (percentProgress <= _ac._end)
			{
				if (_input.count(A_Cancel))
				{
					if (percentProgress >= _ac._startHard)
					{
						_ac._type = ACT_Special;
						_ac._success = true;
					}
				}
				else if (_input.count(A_Accept))
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
	_targets = std::vector<Actor_ptr>(*targets);
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
			_owner->_Graphics->SetColorAll(Vector3f(3.0f, 3.0f, 3.0f), 1.0f);
		else if (percentProgress <= _ac._end && percentProgress >= _ac._start)
			_owner->_Graphics->SetColorAll(Vector3f(2.0f, 2.0f, 2.0f), 1.0f);
		else
			_owner->_ColorState = CS_Update;

		// Handle input for action command
		// Can only happen if you press within the right animation
		CheckActionCommand(percentProgress);
	}
}

void Skill::SetupProtector()
{
	_targets.push_back(_targets.at(0));
	_targets.at(0) = _targets.at(0)->_Fighter->Protector;

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
	pos = target->_Graphics->GetPos() + Vector3f(0.5f, 1.f, 0);
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
	pos = target->_Graphics->GetPos() + Vector3f(0.5f, 1.f, 0);
	pos.z = 0;

	// create font
	font = Font_ptr(new FontFloat(0.7));
	if (_critting)
		font->SetScale(0.75f, 0.75f);
	font->SetText((dmg < 0 || _skillType == ST_Healing ? "+" : "") + std::to_string(dmg), pos, true);

	// color
	Vector3f color;
	if (_critting)
		// purpleish
		color = _ac._success ? Vector3f(0.85f, 0.23f, 0.54f) : Vector3f(0.35f, 0.31f, 0.87f);
	else if (dmg >= 0 && _skillType != ST_Healing)
		// redish / yellowish
		color = _ac._success ? Vector3f(1, 0.8f, 0) : Vector3f(1, 0, 0);
	else
		// Greenish
		color = _ac._success ? Vector3f(0, 0.95f, 0.6f) : Vector3f(0, 1, 0);
	dynamic_cast<FontFloat*>(font.get())->Color = color;

	FontManager::GetInstance().AddFont(font);
}

bool Skill::AnimationsDone()
{
	return !_anims->size();
}

