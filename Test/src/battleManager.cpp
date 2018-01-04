#include "battleManager.h"
#include "input_manager.h"
#include <GLFW\glfw3.h>
#include <set>
#include <algorithm>
#include <iostream>

BattleManager::BattleManager()
{
	Init();
}

BattleManager::BattleManager(std::vector<std::shared_ptr<Actor>> actors)
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
	_state = BS_SelectAction;
	m_animating = false;
	counter = 0;
	_selectedIndex = 0;
	if (_actorQueue.size() > 0)
		_chooseSkill = &_actorQueue.front()->Skills;
}

void BattleManager::Update()
{
	counter++;
	if (counter % 120 == 0)
		std::cout << "State: " << _state << std::endl;
	int winner = FindWinner();
	if (winner == -1)
	{
		// Battle stuffs
		ManageInput();
		UpdateLogic();
	}
	else
	{
		// Battle is over, theres a winner etc
	}
}

void BattleManager::ManageInput()
{
	// Get input
	std::set<int> input;
	if (InputManager::GetInstance().FrameKeyStatus(GLFW_KEY_SPACE, KeyStatus::Release))
		input.emplace(GLFW_KEY_SPACE);
	for (int i = GLFW_KEY_RIGHT; i < GLFW_KEY_UP + 1; i++)
		if (InputManager::GetInstance().FrameKeyStatus(i, Release))
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
		}
		else if (_state == BS_SelectTargets)
		{
			if (_selectedIndex > 0)
			{
				for (int i = _selectedIndex; i >= 0; i--)
				{
					if (!_actors[i]->Dead)
					{
						Select(i);
						break;
					}
				}
			}
			else
			{
				Select(_selectedIndex);
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
		}
		else if (_state == BS_SelectTargets)
		{
			if (_selectedIndex < _actors.size() - 1)
			{
				for (int i = _selectedIndex; i < _actors.size(); i++)
				{
					if (!_actors[i]->Dead)
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
	}

	if (input.count(GLFW_KEY_RIGHT))
	{
		// Attempt to manage RIGHT logic immediately here
	}

	if (input.count(GLFW_KEY_SPACE))
	{
		if (_state == BS_SelectAction)
		{
			_selectedSkill = _chooseSkill->at(_selectedIndex);
			std::cout << "Skill selected: " << _selectedSkill->_name << std::endl;
			_state = BS_SelectTargets;
			for (int i = 0; i < _actors.size(); i++)
			{
				if (_actors[i]->Team != _owner->Team && !_actors[i]->Dead)
				{
					Select(i);
					break;
				}
			}
		}
		else if (_state == BS_SelectTargets)
		{
			for (auto x : _actorQueue)
				if (x->Selected)
				{
					_targets.push_back(x);
					x->Selected = false;
				}

			if (_targets.size() < _selectedSkill->_minTargets)
				return;

			_selectedSkill->Reset();
			UseSkill();
		}
	}
}

void BattleManager::Select(int target)
{
	if (_state == BS_SelectTargets)
	{
		for (auto x : _actors)
			x->Selected = false;

		_actors[_selectedIndex]->Selected = false;
		_actors.at(target)->Selected = true;
		_selectedIndex = target;
		if (_owner->Team == 0)
			std::cout << "Selected: " << _actors.at(_selectedIndex)->Name << std::endl;
	}
	else if (_state == BS_SelectAction)
	{
		_selectedIndex = target;
		if (_owner->Team == 0)
			std::cout << "Selected: " << _chooseSkill->at(_selectedIndex)->_name << std::endl;
	}
}

void BattleManager::UpdateLogic()
{
	if (_state == BS_ActionDone)
	{
		_selectedSkill->Reset();
		_selectedSkill = NULL;
		_state = BS_SelectAction;
		CycleActors();
		Select(0);
	}

	// Don't end turn if theres an animation going
	if (_animations.size() < 1 && _selectedSkill == NULL)
	{
		_owner = _actorQueue.front();

		// If the actor is dead, cycle
		if (_owner->Dead || _state == BS_ActionDone)
		{
			CycleActors();
			Select(0);
			return;
		}

		// Choose enemy action
		if (_owner->Team != 0)
		{
			// ENEMY AI XD
			CycleActors();
		}
	}
	else
	{
		// Update skill
		if (_state != BS_SelectTargets)
		{
			if (_selectedSkill != NULL)
			{
				if (_selectedSkill->_done)
				{
					_state = BS_SelectAction;
					_selectedSkill = NULL;
				}
				else
				{
					_selectedSkill->Update();
				}
			}
		}
	}
}

void BattleManager::UseSkill()
{
	std::cout << "Using skill: " << _selectedSkill->_name << std::endl;
	if (_selectedSkill != NULL)
		_state = _selectedSkill->Start(_targets, &_actorQueue, &_animations);
	else
		_state = BS_SelectAction;
}

void BattleManager::CycleActors()
{
	std::shared_ptr<Actor> front = _actorQueue.front();
	_actorQueue.pop_front();
	_actorQueue.push_back(front);
	_actorQueue.front()->ChoosingAction = true;
	_chooseSkill = &_actorQueue.front()->Skills;
	if (!_actorQueue.front()->Dead)
		std::cout << "It's " << _actorQueue.front()->Name << "'s turn!" << std::endl;
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