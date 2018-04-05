#include "battleManager.h"
#include "input_manager.h"
#include <GLFW/glfw3.h>
#include <set>
#include <algorithm>
#include <iostream>
#include "fontManager.h"
#include "localizationData.h"
#include "animMoveTo.h"

BattleManager::BattleManager()
{
	Init();
}

BattleManager::BattleManager(std::vector<Actor_ptr> actors)
{
	// Populate actors
	for (auto a : actors)
	{
		// Reset modified stats
		a->_Fighter->ResetModified();
		_actors.push_back(a);
	}

	std::sort(actors.begin(), actors.end(), Actor::ActorSpeedSort);
	for (auto a : actors)
		_actorQueue.push_back(a);

	Init();
}

void BattleManager::Init()
{
	Camera::_currentCam->SetCameraFollowSpeed(CAMSPEED_Slow);
	_hud.Init(_actors);
	_showingSkills = false;
	_state = BS_TurnStart;
	m_animating = false;
	counter = 0;
	_winner = -1;
	_selectedIndex = 0;

	// Make sure that there are 2 teams to fight against one another
	{
		std::set<int> teams;
		for (auto& x : _actors)
			if (!teams.count(x->_Fighter->Team))
				teams.emplace(x->_Fighter->Team);

		// If there aren't 2 teams, end the battle
		_done = teams.size() < 2;
	}

	if (_actorQueue.size() > 0)
	{
		_chooseSkill = &_actorQueue.front()->_Fighter->Skills;
		_owner = _actorQueue.front();
	}
}

void BattleManager::Update()
{
	// Return if the battle logic is over
	if (!_done || _selectedSkill && !_selectedSkill->_done)
	{
		// Battle stuffs
		ManageInput();
		UpdateLogic();
	}

	if (_animations.size() > 0)
	{
		for (int i = 0; i < _animations.size(); i++)
		{
			if (_animations.at(i)->_async || i == 0)
				_animations.at(i)->Update();
			if (_animations.at(i)->_done)
			{
				_animations.erase(_animations.begin() + i);
				i--;
			}
		}
	}

	// End the battle, gain exp and show stuff
	if (_animations.size() == 0 && _state == BS_TurnStart && !_done)
	{
		Camera::_currentCam->SetFollow(Camera::_currentCam->MapCenter());
		_winner = FindWinner();
		if (_winner != -1)
		{
			int xp = 0;
			for (auto& actor : _actors)
				if (actor->_Fighter->Team != 0)
					xp += actor->_Fighter->DeathExp;

			for (auto& actor : _actors)
			{
				/* Inside here:
					Create an xp bar lerp that continues over many level ups
					XP bar needs to observe one of it's own variables that well update to control what it looks like
				*/
				if (actor->_Fighter->Team == 0 && !actor->_Fighter->Dead)
				{
					int level = actor->_Fighter->GetLevel();
					actor->_Fighter->GiveExp(xp);
					if (level != actor->_Fighter->GetLevel())
						FontManager::GetInstance().CreateFloatingText(actor->_Graphics->GetPosRef(), "Level up!");
					else
						FontManager::GetInstance().CreateFloatingText(actor->_Graphics->GetPosRef(), "+" + std::to_string(xp) + " XP");
				}
			}

			_done = true;
		}
	}

	_hud.Update();
}

// Move Animation
void BattleManager::MoveToLight(bool moveup, bool turnEnd)
{
	if (!_owner->_Fighter->Dead || turnEnd)
		if (moveup)
		{
			Anim_ptr move1 = Anim_ptr(new AnimMoveTo(_owner->_Graphics->GetPos() + Vector3f(_owner->_Fighter->Team == 0 ? 1.f : -1, 0, 0), _owner));
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
	else if (_owner->_Fighter->Team == 0 && !_showingSkills && _animations.size() == 0 && _state == BS_SelectAction)
	{
		SetChooseSkillText();
		_showingSkills = true;
		Select(0);
	}
}

void BattleManager::TurnStart()
{
	MoveToLight(true);
	if (_owner->_Fighter->Dead)
		_state = BS_TurnEnd;
	else
	{
		_owner->_Fighter->TurnStart(_actors);
		_state = BS_SelectAction;
	}
}
// BS_SelectAction and BS_SelectTarget are both purely input handled for the player
void BattleManager::SelectAction()
{
	// Choose ENEMY action
	if (_owner->_Fighter->Team != 0)
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
				if (alreadyTriedSkills.size() == _owner->_Fighter->Skills.size())
				{
					done = true;
					_state = BS_ActionDone;
					return;
				}
				skill = rand() % _owner->_Fighter->Skills.size();
				_selectedSkill = _owner->_Fighter->Skills.at(skill);
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
				if (!_actors.at(targ)->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode) || // doesnt respect targeting
					_actors.at(DefaultTargetActorIndex())->_Fighter->Team != _actors.at(targ)->_Fighter->Team)
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

	//if (_targets.size() == 0 && _state != BS_ActionProgress)
	//{
	//}
	//else
	//{
		// zoom towards first target
		//Camera::_currentCam->Scale(Vector3f(2));
		//Camera::_currentCam->Follow(_targets[0]->_Graphics->GetPosRef());
		//Camera::_currentCam->FollowScale(_targets[0]->_Graphics->GetPosRef(), Vector3f(1.5f));
	//}

	// If there are animations, let them run out
	if (_state == BS_ActionProgress || !_animations.size())
	{
		switch (_state)
		{
		case BS_TurnStart:
			// zoom back to normal
			Camera::_currentCam->SetScale(Vector3f(1));
			Camera::_currentCam->SetFollow(Camera::_currentCam->MapCenter());
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
			//Camera::_currentCam->SetScale(Vector3f(1.5f));
			//Camera::_currentCam->SetFollow(_owner->_Graphics->GetPosRef());
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
		_fonts.push_back(FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png"));

	for (int i = 0; i < _chooseSkill->size(); i++)
	{
		FontManager::GetInstance().EnableFont(_fonts[i]);
		FontManager::GetInstance().SetScale(_fonts[i], 0.5f, 0.5f);
		FontManager::GetInstance().SetText(_fonts[i], _(_chooseSkill->at(i)->_name), Vector3f(5.0f, 4.75f - i * 0.5f, 0));
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
	if ((_animations.size() || _owner->_Fighter->Team != 0) && _state != BS_ActionProgress)
		return;

	// Get input
	std::set<int> input;
	// Handle key release to choose skills, key press for action commands
	KeyStatus status = KeyPressed;//(_state != BS_SelectTargets && _state != BS_SelectAction) ? KeyPressed : Release;
	if (InputManager::GetInstance().FrameKeyStatus(A_Accept, status))
		input.emplace(A_Accept);
	if (InputManager::GetInstance().FrameKeyStatus(A_Cancel, status))
		input.emplace(A_Cancel);
	for (int i = A_Right; i < A_Up + 1; i++)
		if (InputManager::GetInstance().FrameKeyStatus((InputAction)i, status))
			input.emplace(i);

	// If theres an animation going, SEND THE INPUT TO THE SKILL
	if (_state != BS_SelectTargets && _state != BS_SelectAction)
	{
		if (_selectedSkill != NULL)
			_selectedSkill->_input = input;
		return;
	}

	// Handle Input
	if (input.count(A_Down))
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
					if (_actors[i]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
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
					if (_actors[i]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
					{
						Select(i);
						break;
					}
				}
			}
		}
	}

	if (input.count(A_Up))
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
					if (_actors[i]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
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
					if (_actors[i]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
					{
						Select(i);
						break;
					}
				}
			}
		}
	}

	if (input.count(A_Left))
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
					if (value + increment - MAX_FIGHTERS_PER_SIDE < MAX_FIGHTERS_PER_SIDE && _actors[value - MAX_FIGHTERS_PER_SIDE + increment]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
						_selectedIndex = value - MAX_FIGHTERS_PER_SIDE + increment;
					else if (value - increment - MAX_FIGHTERS_PER_SIDE >= 0 && _actors[value - MAX_FIGHTERS_PER_SIDE - increment]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
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

	if (input.count(A_Right))
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
					if (value + increment + MAX_FIGHTERS_PER_SIDE < _actors.size() && _actors[value + MAX_FIGHTERS_PER_SIDE + increment]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
						_selectedIndex = value + MAX_FIGHTERS_PER_SIDE + increment;
					else if (value - increment + MAX_FIGHTERS_PER_SIDE >= MAX_FIGHTERS_PER_SIDE && _actors[value + MAX_FIGHTERS_PER_SIDE - increment]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
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

	if (input.count(A_Accept))
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
					x->UpdateColor();
					x->Selected = false;
				}

			if (_targets.size() < _selectedSkill->_minTargets)
				return;

			_selectedSkill->Reset();
			UseSkill();
		}
	}

	if (input.count(A_Cancel))
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
				x->UpdateColor();
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
			if (_actors[i]->_Fighter->Team == _owner->_Fighter->Team && _actors[i]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
				done = true;
			break;
		case DT_Enemy:
			if (_actors[i]->_Fighter->Team != _owner->_Fighter->Team && _actors[i]->_Fighter->RespectsTargeting(_owner, _selectedSkill->_targetMode))
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
			{
				x->Selected = false;
				x->UpdateColor();
			}
		}

		_actors[_selectedIndex]->Selected = false;
		_actors.at(target)->Selected = true;
		//_actors.at(target)->_Graphics->SetColorAll(Vector3f(1.f, 0.25f, 0.25f));
		_selectedIndex = target;
	}
	else if (_state == BS_SelectAction)
	{
		if (_owner->_Fighter->Team == 0)
		{
			if (_fonts.size() > _selectedIndex)
			{
				FontManager::GetInstance().GetFont(_fonts[_selectedIndex])->GetGraphics()->SetColorAll();
				FontManager::GetInstance().GetFont(_fonts[target])->GetGraphics()->SetColorAll(Vector3f(1, 0, 0));
			}
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
	front->UpdateColor();

	_actorQueue.pop_front();
	_actorQueue.push_back(front);
	_owner = _actorQueue.front();
	_owner->ChoosingAction = true;
	_owner->UpdateColor();
	_chooseSkill = &_owner->_Fighter->Skills;
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
		if (!x->_Fighter->Dead && !activeTeams.count(x->_Fighter->Team))
			activeTeams.emplace(x->_Fighter->Team);
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
