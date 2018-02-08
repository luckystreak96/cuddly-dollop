#include "battleManager.h"
#include "input_manager.h"
#include <GLFW/glfw3.h>
#include <set>
#include <algorithm>
#include <iostream>
#include "fontManager.h"
#include "gameData.h"
#include "animMoveTo.h"

BattleManager::BattleManager()
{
	Init();
}

BattleManager::BattleManager(std::vector<Actor_ptr> actors)
{
	// Populate actors
	for (auto a : actors)
		_actors.push_back(a);

	std::sort(actors.begin(), actors.end(), Actor::ActorSpeedSort);
	for (auto a : actors)
		_actorQueue.push_back(a);

	Init();
}

void BattleManager::Init()
{
	_hud.Init(_actors);
	_showingSkills = false;
	_state = BS_TurnStart;
	m_animating = false;
	counter = 0;
	_winner = -1;
	_selectedIndex = 0;
	_done = false;
	if (_actorQueue.size() > 0)
	{
		_chooseSkill = &_actorQueue.front()->Skills;
		_owner = _actorQueue.front();
	}
}

void BattleManager::Update()
{
	// Return when the battle is over
	if (_done)
		return;

	// Cout battle state
	//counter++;
	//if (counter % 120 == 0)
	//	std::cout << "State: " << _state << std::endl;

	_winner = FindWinner();
	if (_winner == -1 || _selectedSkill && !_selectedSkill->_done)
	{
		// Battle stuffs
		ManageInput();
		UpdateLogic();
		if (_animations.size() > 0)
		{
			for (int i = 0; i < _animations.size(); i++)
			{
				if(_animations.at(i)->_async || i == 0)
					_animations.at(i)->Update();
				if (_animations.at(i)->_done)
				{
					_animations.erase(_animations.begin() + i);
					i--;
				}
			}
		}

		_hud.Update();
	}
	else
	{
		// Battle is over, theres a winner etc
		std::cout << "The winner is: team " << _winner << std::endl;
		_done = true;
	}
}

// Move Animation
void BattleManager::MoveToLight(bool moveup, bool turnEnd)
{
	if (!_owner->Dead || turnEnd)
		if (moveup)
		{
			Anim_ptr move1 = Anim_ptr(new AnimMoveTo(_owner->GetPos() + Vector3f(_owner->Team == 0 ? 1.f : -1, 0, 0), _owner));
			_animations.push_back(move1);
		}
		else
		{
			Anim_ptr move2 = Anim_ptr(new AnimMoveTo(_owner->BasePosition, _owner));
			_animations.push_back(move2);
		}
}

// Display or stop displaying skills
void BattleManager::UpdateSkillDisplay()
{
	if (_showingSkills)
	{
		if (_state != BS_SelectAction)
		{
			RemoveChooseSkillText();
			_showingSkills = false;
		}
	}
	else if (_owner->Team == 0 && !_showingSkills && _animations.size() == 0 && _state == BS_SelectAction)
	{
		SetChooseSkillText();
		_showingSkills = true;
		Select(0);
	}
}

void BattleManager::TurnStart()
{
	MoveToLight(true);
	if (_owner->Dead)
		_state = BS_TurnEnd;
	else
	{
		_owner->TurnStart(_actors);
		_state = BS_SelectAction;
	}
}
// BS_SelectAction and BS_SelectTarget are both purely input handled for the player
void BattleManager::SelectAction()
{
	// Choose ENEMY action
	if (_owner->Team != 0)
	{
		bool done = false;
		int targ = 0;
		std::set<int> alreadyTriedSkills;

		// Ensure possible targets and skills
		while (!done)
		{
			std::set<int> alreadyTargeted;
			targ = 0;

			int skill;
			do {
				// None of the skills found valid targets -> end turn
				if (alreadyTriedSkills.size() == _owner->Skills.size())
				{
					done = true;
					_state = BS_ActionDone;
					return;
				}
				skill = rand() % _owner->Skills.size();
				_selectedSkill = _owner->Skills.at(skill);
			} while (alreadyTriedSkills.count(skill));
			alreadyTriedSkills.emplace(skill); // havent tried this, lets go

			do {
				// if you already tried all the actors, select another skill
				if (alreadyTargeted.size() == _actors.size())
				{
					targ = -1;
					break;
				}
				targ = rand() % _actors.size();
				// if the target is illegal, re-pick and dont choose that target again
				if (!_actors.at(targ)->RespectsTargeting(_owner, _selectedSkill->_targetMode) || // doesnt respect targeting
					_actors.at(DefaultTargetActorIndex())->Team != _actors.at(targ)->Team)
					alreadyTargeted.emplace(targ);
			} while (targ < 0 || targ >= _actors.size() || alreadyTargeted.count(targ)); // is targeting someone of a different team than the default target

			alreadyTargeted.emplace(targ);

			// target is good, can move on, otherwise choose new skill
			if (targ != -1)
				done = true;
		}

		_targets.push_back(_actors.at(targ));

		UseSkill();
	}
}
void BattleManager::SelectTargets()
{
}
void BattleManager::ActionProgress()
{
	if (_selectedSkill != NULL)
	{
		if (_selectedSkill->_done)
		{
			_state = BS_ActionDone;
		}
		else
		{
			_selectedSkill->Update();
		}
	}
	else
	{
		_state = BS_ActionDone;
	}
}
void BattleManager::ActionDone()
{
	_selectedSkill->Reset();
	_selectedSkill = NULL;
	_state = BS_TurnEnd;
}
void BattleManager::TurnEnd()
{
	MoveToLight(false, true);
	CycleActors();
	_state = BS_TurnStart;
}

void BattleManager::UpdateLogic()
{
	// Display or stop displaying skills
	UpdateSkillDisplay();

	// If there are animations, let them run out
	if (_state == BS_ActionProgress || !_animations.size())
	{
		switch (_state)
		{
		case BS_TurnStart:
			TurnStart();
			break;
		case BS_SelectAction:
			// BS_SelectAction and BS_SelectTarget are both purely input handled for the player
			SelectAction();
			break;
		case BS_SelectTargets:
			// Handled by input, method only for show
			SelectTargets();
			break;
		case BS_ActionProgress:
			// Update the skill
			ActionProgress();
			break;
		case BS_ActionDone:
			// Handles the action being done
			ActionDone();
			break;
		case BS_TurnEnd:
			TurnEnd();
			break;
		}
	}
}

void BattleManager::SetChooseSkillText()
{
	while (_fonts.size() < _chooseSkill->size())
		_fonts.push_back(FontManager::GetInstance().AddFont(false, false, true, "res/fonts/lowercase.png"));

	for (int i = 0; i < _chooseSkill->size(); i++)
	{
		FontManager::GetInstance().EnableFont(_fonts[i]);
		FontManager::GetInstance().SetScale(_fonts[i], 0.5f, 0.5f);
		FontManager::GetInstance().SetText(_fonts[i], _(_chooseSkill->at(i)->_name), Vector3f(7.5f, 5.5f - i * 0.5f, 0));
		//FontManager::GetInstance().GetFont(_fonts[i])->SetText(_chooseSkill->at(i)->_name, Vector3f(6, 7 - i, 1));
	}
}

void BattleManager::RemoveChooseSkillText()
{
	for (int i = 0; i < _fonts.size(); i++)
		FontManager::GetInstance().GetFont(_fonts[i])->_enabled = false;
}


void BattleManager::ManageInput()
{
	// Dont allow any input if theres an animation running
	// This doesnt apply if theres a skill in progress, gotta be interactive!
	if (_animations.size() && _state != BS_ActionProgress)
		return;

	// Get input
	std::set<int> input;
	// Handle key release to choose skills, key press for action commands
	KeyStatus status = (_state != BS_SelectTargets && _state != BS_SelectAction)
		? KeyPressed : Release;
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_X, status, 5))
		input.emplace(GLFW_KEY_X);
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_Z, status, 5))
		input.emplace(GLFW_KEY_Z);
	for (int i = GLFW_KEY_RIGHT; i < GLFW_KEY_UP + 1; i++)
		if (InputManager::GetInstance().FrameKeyStatus(i, status, 5))
			input.emplace(i);

	// If theres an animation going, SEND THE INPUT TO THE SKILL
	if (_state != BS_SelectTargets && _state != BS_SelectAction)
	{
		if (_selectedSkill != NULL)
			_selectedSkill->_input = input;
		return;
	}

	// Handle Input
	if (input.count(GLFW_KEY_DOWN))
	{
		// Attempt to manage DOWN logic immediately here
		if (_state == BS_SelectAction)
		{
			if (_selectedIndex < _chooseSkill->size() - 1)
				Select(_selectedIndex + 1);
			else
				Select(0);
		}
		else if (_state == BS_SelectTargets)
		{
			if (_selectedIndex > 0)
			{
				for (int i = _selectedIndex - 1; i >= 0; i--)
				{
					if (_actors[i]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
					{
						Select(i);
						break;
					}
				}
			}
			else // _selectedIndex == 0
			{
				for (int i = _actors.size() - 1; i >= 0; i--)
				{
					if (_actors[i]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
					{
						Select(i);
						break;
					}
				}
			}
		}
	}

	if (input.count(GLFW_KEY_UP))
	{
		// Attempt to manage UP logic immediately here
		if (_state == BS_SelectAction)
		{
			if (_selectedIndex > 0)
			{
				Select(_selectedIndex - 1);
			}
			else
				Select(_chooseSkill->size() - 1);
		}
		else if (_state == BS_SelectTargets)
		{
			if (_selectedIndex < _actors.size() - 1)
			{
				for (int i = _selectedIndex + 1; i < _actors.size(); i++)
				{
					if (_actors[i]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
					{
						Select(i);
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < _actors.size(); i++)
				{
					if (_actors[i]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
					{
						Select(i);
						break;
					}
				}
			}
		}
	}

	if (input.count(GLFW_KEY_LEFT))
	{
		// Attempt to manage LEFT logic immediately here
		if (_state == BS_SelectTargets)
		{
			// Only do anything if the cursor is on the enemy side
			if (_selectedIndex >= MAX_FIGHTERS_PER_SIDE)
			{
				bool found = false;
				int value = _selectedIndex;
				int increment = 0;

				while (!found)
				{
					if (value + increment - MAX_FIGHTERS_PER_SIDE < MAX_FIGHTERS_PER_SIDE && _actors[value - MAX_FIGHTERS_PER_SIDE + increment]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
						_selectedIndex = value - MAX_FIGHTERS_PER_SIDE + increment;
					else if (value - increment - MAX_FIGHTERS_PER_SIDE >= 0 && _actors[value - MAX_FIGHTERS_PER_SIDE - increment]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
						_selectedIndex = value - MAX_FIGHTERS_PER_SIDE - increment;

					if (value != _selectedIndex)
						found = true;

					increment++;
					// If the counter goes too high, then there was no valid target on this side
					if (increment >= MAX_FIGHTERS_PER_SIDE)
						found = true;
				}
			}

			Select(_selectedIndex);
		}
	}

	if (input.count(GLFW_KEY_RIGHT))
	{
		// Attempt to manage RIGHT logic immediately here
		if (_state == BS_SelectTargets)
		{
			// Only do anything if the cursor is on the enemy side
			if (_selectedIndex < MAX_FIGHTERS_PER_SIDE)
			{
				bool found = false;
				int value = _selectedIndex;
				int increment = 0;

				while (!found)
				{
					if (value + increment + MAX_FIGHTERS_PER_SIDE < _actors.size() && _actors[value + MAX_FIGHTERS_PER_SIDE + increment]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
						_selectedIndex = value + MAX_FIGHTERS_PER_SIDE + increment;
					else if (value - increment + MAX_FIGHTERS_PER_SIDE >= MAX_FIGHTERS_PER_SIDE && _actors[value + MAX_FIGHTERS_PER_SIDE - increment]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
						_selectedIndex = value + MAX_FIGHTERS_PER_SIDE - increment;

					if (value != _selectedIndex)
						found = true;

					increment++;
					// If the counter goes too high, then there was no valid target on this side
					if (increment >= MAX_FIGHTERS_PER_SIDE)
						found = true;
				}
			}
			Select(_selectedIndex);
		}
	}

	if (input.count(GLFW_KEY_X))
	{
		if (_state == BS_SelectAction)
		{
			_selectedSkill = _chooseSkill->at(_selectedIndex);
			//std::cout << "Skill selected: " << _selectedSkill->_name << std::endl;
			_state = BS_SelectTargets;
			int target = DefaultTargetActorIndex();
			Select(target);
		}
		else if (_state == BS_SelectTargets)
		{
			for (auto x : _actorQueue)
				if (x->Selected)
				{
					_targets.push_back(x);
					x->SetColor();
					x->Selected = false;
				}

			if (_targets.size() < _selectedSkill->_minTargets)
				return;

			_selectedSkill->Reset();
			UseSkill();
		}
	}

	if (input.count(GLFW_KEY_Z))
	{
		if (_state == BS_SelectTargets)
		{
			_selectedIndex = 0;
			_state = BS_SelectAction;
			SetChooseSkillText();
			Select(_selectedIndex);
			_showingSkills = true;
			for (auto x : _actors)
			{
				x->SetColor();
				x->Selected = false;
			}
		}
	}
}

int BattleManager::DefaultTargetActorIndex()
{
	int i;
	bool done = false;
	for (i = 0; i < _actors.size(); i++)
	{
		switch (_selectedSkill->_defaultTarget)
		{
		case DT_Self:
			if (_actors[i] == _owner)
				done = true;
			break;
		case DT_Ally:
			if (_actors[i]->Team == _owner->Team && _actors[i]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
				done = true;
			break;
		case DT_Enemy:
			if (_actors[i]->Team != _owner->Team && _actors[i]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
				done = true;
			break;
		}

		if (done)
			break;
	}

	return i;
}

void BattleManager::Select(int target)
{
	if (_state == BS_SelectTargets)
	{
		for (auto x : _actors)
		{
			if (x->Selected)
				x->SetColor();
			x->Selected = false;
		}

		_actors[_selectedIndex]->Selected = false;
		_actors.at(target)->Selected = true;
		_actors.at(target)->SetColorAll(Vector3f(1.f, 0.25f, 0.25f));
		_selectedIndex = target;
	}
	else if (_state == BS_SelectAction)
	{
		if (_owner->Team == 0)
		{
			FontManager::GetInstance().GetFont(_fonts[_selectedIndex])->GetGraphics()->SetColorAll();
			FontManager::GetInstance().GetFont(_fonts[target])->GetGraphics()->SetColorAll(Vector3f(1, 0, 0));
		}
		_selectedIndex = target;
	}
}

void BattleManager::UseSkill()
{
	//std::cout << "Using skill: " << _selectedSkill->_name << std::endl;
	if (_selectedSkill != NULL)
		_state = _selectedSkill->Start(&_targets, &_actorQueue, &_animations, _owner);
	else
		_state = BS_SelectAction;
}

void BattleManager::CycleActors()
{
	Actor_ptr front = _actorQueue.front();
	front->ChoosingAction = false;
	front->SetColor();

	_actorQueue.pop_front();
	_actorQueue.push_back(front);
	_owner = _actorQueue.front();
	_owner->ChoosingAction = true;
	_owner->SetColor();
	_chooseSkill = &_owner->Skills;
	_targets.clear();
	_selectedIndex = 0;
}

// Return winning team, -1 if battle isn't over
int BattleManager::FindWinner()
{
	std::set<int> activeTeams;
	for (auto x : _actorQueue)
	{
		// Dont bother continuing if the fight isn't over
		if (activeTeams.size() > 1)
			break;
		// If someone isn't dead on that team, that team is still going
		if (!x->Dead && !activeTeams.count(x->Team))
			activeTeams.emplace(x->Team);
	}

	// More than one team is alive, return -1
	if (activeTeams.size() != 1)
		return -1;

	// Only one team is alive, return that team
	return *activeTeams.begin();
}

void BattleManager::SetRender()
{
	_hud.SetRender();
}
