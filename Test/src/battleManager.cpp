#include "battleManager.h"

#include "input_manager.h"
#include "fontManager.h"
#include "localizationData.h"
#include "battleAnimationManager.h"
#include "battleData.h"
#include "passiveFactory.h"

#include <GLFW/glfw3.h>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std;

BattleManager::BattleManager()
{
}

bool BattleManager::Animating()
{
	return m_graphics->Animating();
}

BattleManager::BattleManager(std::vector<Fighter_ptr> actors)
{
	Init(actors);
}

BattleManager::~BattleManager()
{
	for (auto& x : m_info._fighters)
		x->_observers.clear();
}

bool BattleManager::status_condition_met(StatusEffect status, Fighter_ptr fighter)
{
	bool result = true;

	for (auto& condition : status._Conditions)
		switch (condition)
		{
		case TriggerCondition::TC_None:
			break;
		case TriggerCondition::TC_Targeted:
			if (m_info._targets.count(fighter->_BattleFieldPosition) && m_info._owner->Team != fighter->Team)
				break;
			result = false;
			break;
		case TriggerCondition::TC_Applier_Alive:
			if (!m_info._fighters[status._Applier]->Dead)
				break;
			result = false;
			break;
		case TriggerCondition::TC_Status_Owner_Alive:
			if (!m_info._fighters[fighter->_BattleFieldPosition]->Dead)
				break;
			result = false;
			break;
		default:
			result = false;
			break;
		}
	return result;
}


// 3 steps:
//	1 - Check statuses instead of passiveskills
//	2 - Make passiveskills create statuses at the start of battle
//	3 - Find a way to get these interventions to trigger at the right moment and do the right things :)
void BattleManager::ProcessSkillReactions(SkillProgress prog)
{
	if (m_info._action_buffer[prog].size())
		for (auto& action : m_info._action_buffer[prog])
			add_animation(action);

	m_info._action_buffer[prog].clear();

	for (auto& fighter : m_info._fighters)
	{
		for (auto& status : fighter->_Statuses)
		{
			if (prog == SP_1_Before_Anim)
				status._Triggered = false;

			if (!status._Triggered && status_condition_met(status, fighter))
			{
				status._Triggered = true;
				for (auto& action_type : status._Actions)
				{
					SetSkillArguments(action_type.second, status._Applier);
					if (action_type.first == prog)
						add_animation(action_type.second);
					else
						m_info._action_buffer[action_type.first].push_back(action_type.second);
				}
			}
		}
	}
}


void BattleManager::ProcessSkill()
{
	std::vector<triple> operations = m_info._selectedSkill->GetAnimations();

	for (auto& x : operations)
		add_animation(x);
}

void BattleManager::add_animation(triple& x)
{
	// need to fill in the vector of floats
	// ('<' because of AARG_FloatAsync)
	if (x._arg_type < AARG_Float)
		SetSkillArguments(x); // single target skill chosen by *m_info._targets.begin()

	// x is an action
	if (x._animation > AA_Start)
	{
		int t = x._args.front();
		if (x._animation == AA_DealDamage)
		{
			// Deal damage
			Damage dmg = HandleDamage(t);
			// Show dmg
			m_graphics->DamageAnimation(t, m_info._selectedSkill, dmg);

			if (m_info._targets.size() > 1)
			{
				m_info._selectedSkill->SetAnimProgressRepeat();
				m_info._targets.erase(t);
			}
		}
		else if (x._animation == AA_ApplyEffect)
		{
			//m_info._fighters.at(t)->_Statuses.push_back(BattleData::StatusEffects.at(Status::Status_Determined));
			for (auto& status : *m_info._selectedSkill->GetStatusEffects())
			{
				StatusEffect sts = BattleData::StatusEffects.at(status);
				sts._Applier = m_info._owner->_BattleFieldPosition;
				m_info._fighters.at(t)->_Statuses.push_back(sts);
			}
			m_info._fighters.at(t)->ReCalculateStats();
		}
		else if (x._animation == AA_ChangeTarget)
		{
			if (m_info._targets.count(x._args[1]))
			{
				m_info._targets.erase(x._args[1]);
				m_info._targets.emplace(x._args[0]);
			}
		}
	}
	// x is an animation
	else
	{
		Anim_ptr animation = m_graphics->CreateAnimation(x);
		if (animation)
			m_graphics->insert_animation(animation);
	}
}


void BattleManager::SetSkillArguments(triple& x)
{
	switch (x._arg_type)
	{
	case AARG_OwnerTargets:
		x._args.clear();
		x._args.push_back((float)m_info._owner->GetId());
		for (auto y : m_info._targets)
			x._args.push_back((float)y);
		break;
	case AARG_Owner:
		if (x._args.size() < 1)
			x._args.push_back((float)m_info._owner->GetId());
		else
			x._args[0] = (float)m_info._owner->GetId();
		break;
	case AARG_Targets:
		x._args.clear();
		for (auto y : m_info._targets)
			x._args.push_back((float)y);
		break;
	case AARG_Target:
		if (x._args.size() < 1)
			x._args.push_back((float)*m_info._targets.begin());
		else
			x._args[0] = (float)*m_info._targets.begin();
		break;
	default:
		break;
	}
}

void BattleManager::SetSkillArguments(triple& x, int applier)
{
	floats& f = x._args;
	if (x._arg_type == AARG_Passive_Applier2Owner)
	{
		f.clear();
		f.push_back(applier);
		f.push_back(*m_info._targets.begin());
	}
}


Damage BattleManager::HandleDamage(int target)
{
	Damage dmg = m_info._selectedSkill->CalculateDamage(*m_info._owner);
	Fighter_ptr& f = m_info._fighters.at(target);

	// Apply first damage modifications
	if (m_info._selectedSkill->_skillType != ST_Healing)
		f->DamageModifiers(dmg, dmg._critting);

	if (m_info._owner->Team != 0 && f->Team == 0 && m_info._selectedSkill->_ac._type == ACT_Attack)
		m_info._selectedSkill->_ac._type = ACT_Defend;

	if (m_info._selectedSkill->_ac._success)
	{
		// Your team under attack from enemy -> Defense Action Command
		if (m_info._owner->Team != 0 && f->Team == 0)
		{
			if (m_info._selectedSkill->_ac._type == ACT_Special)
				f->SpecialActionCommand(dmg);
			else if (m_info._selectedSkill->_skillType == ST_Physical)
				f->PhysicalDefenseActionCommand(dmg);
			else
				f->MagicalDefenseActionCommand(dmg);
		}
		// Your team attacking -> Offense Action Command
		else if (m_info._owner->Team == 0)
		{
			if (m_info._selectedSkill->_skillType == ST_Physical)
				f->PhysicalOffenseActionCommand(dmg);
			else
				f->MagicalOffenseActionCommand(dmg);
		}
	}

	// Check for bonus damage
	if (m_info._selectedSkill->_skillElement != SE_None)
	{
		SkillElement targetElement;
		if (m_info._selectedSkill->_skillElement == SE_Determined)
			targetElement = SE_Pragmatic;
		else if (m_info._selectedSkill->_skillElement == SE_Pragmatic)
			targetElement = SE_StrongWilled;
		else if (m_info._selectedSkill->_skillElement == SE_StrongWilled)
			targetElement = SE_Determined;

		if (f->HasElement(targetElement))
		{
			Damage bonus = ApplyBonusEffect(f);
			dmg._value += bonus._value;
			dmg._type = SkillType::ST_Bonus; // only affects the way it is shown on screen
		}
	}

	// Deal the dmg
	if (m_info._selectedSkill->_skillType == ST_Healing)
		f->ApplyHealing(dmg);
	else
		f->TakeDamage(dmg);


	return dmg;
}

Damage BattleManager::ApplyBonusEffect(Fighter_ptr target)
{
	Damage dmg = Damage();
	dmg._value = 10;
	dmg._type = SkillType::ST_Bonus;

	// Add more effects according to Fighters attributes here

	return dmg;
}


void BattleManager::SetupHUD()
{
	// setup hud
	for (auto& x : m_info._fighters)
	{
		BattleUnit unit = x->create_battle_unit();
		m_graphics->SetupHUD(unit);
	}
}


void BattleManager::Init(std::vector<Fighter_ptr> actors)
{
	if (!actors.size())
	{
		std::cout << "ERROR | BattleManager::Init() -> no fighters were given to work with." << std::endl;
		return;
	}

	m_info.init(actors);

	m_graphics = std::shared_ptr<BattleAnimationManager>(new BattleAnimationManager());

	_postBattleState = PBS_FightingInProgress;

	// Make sure that there are 2 teams to fight against one another
	if (m_info.get_num_teams() < 2)
		_postBattleState = PBS_FightingDone;
}

void BattleManager::UpdateColors()
{
	for (auto& x : m_info._fighters)
	{
		if (m_info._state == BS_SelectAction)
		{
			if (m_info._owner->Team == 0)
				m_graphics->UpdateColors(x->GetId(), false, x->Dead, 0);
		}
		else
		{
			bool deny = false;
			if (m_info._state != BS_SelectTargets)
				deny = true;
			int actionCommandLevel = 0;
			if (m_info._owner->_BattleFieldPosition == x->GetId() && m_info._selectedSkill)
				actionCommandLevel = m_info._selectedSkill->action_command_level(m_graphics->get_animation_progress());
			m_graphics->UpdateColors(x->GetId(), !deny && m_info._selectedIndices.count(x->_BattleFieldPosition), x->Dead, actionCommandLevel);
		}
	}
}

void BattleManager::Update()
{
	// Return if the battle logic is over
	if (_postBattleState == PBS_FightingInProgress/* && !Animating()*/)// || m_info._selectedSkill && !m_info._selectedSkill->_done)
	{
		// Battle stuffs
		ManageInput();
		UpdateLogic();
	}

	// Update some graphics stuff
	UpdateColors();

	// Update animations
	m_graphics->UpdateAnimations();

	// End the battle, gain exp and show stuff
	if (/*!Animating() && */m_info._state == BS_TurnStart)
	{
		switch (_postBattleState)
		{
		case PBS_FightingInProgress:
			m_info._winner = FindWinner();
			if (m_info._winner != -1)
			{
				Camera::_currentCam->SetFollow(Camera::_currentCam->MapCenter());
				_postBattleState = PBS_FightingDone;
			}
			break;
		case PBS_FightingDone:
			for (auto& x : m_info._fighters)
				x->NoPredictCountDown = 0;
			break;
		case PBS_ExpAnimation:
		{
			int xp = 0;
			for (auto& actor : m_info._fighters)
				if (actor->Team != 0)
					xp += actor->DeathExp;

			for (auto& actor : m_info._fighters)
			{
				if (actor->Team == 0 && !actor->Dead)
					m_graphics->ExpAnimation(actor->GetId(), xp);
			}
			m_graphics->set_last_animation_non_async();
			//m_graphics->insert_animation(m_graphics->CreateAnimation(triple(AnimationOperation::AS_Wait, AnimationArgument::AARG_Float, floats({ 0.0f }))));

			_postBattleState = PBS_ConfirmCompletion;
		}
		break;
		case PBS_PostBattleComplete:
			break;
		default:
			break;
		}
	}

	m_graphics->UpdateHUD();
	//_hud.Update();
}

// Move Animation
void BattleManager::MoveToLight(bool moveup, bool turnEnd)
{
	if (!m_info._owner->Dead || turnEnd)
		m_graphics->MoveUp(m_info._owner->_BattleFieldPosition, moveup);

}

// Display or stop displaying skills
void BattleManager::UpdateSkillDisplay()
{
	if (m_showingSkills)
	{
		if (m_info._state != BS_SelectAction)
		{
			//RemoveChooseSkillText();
			m_showingSkills = false;
			m_graphics->update_skill_display(&m_info._chooseSkill, SDS_none);

		}
	}
	else if (m_info._owner->Team == 0 && !m_showingSkills && !Animating() && m_info._state == BS_SelectAction)
	{
		//SetChooseSkillText();
		m_showingSkills = true;
		//m_info.select_index(0);
		m_graphics->update_skill_display(&m_info._chooseSkill, SDS_choose_skill);
	}
}

bool BattleManager::ValidateTargets(Fighter_ptr f)
{
	bool valid = false;

	// Check if all targets are alive
	for (auto& x : *f->GetTargets())
		if (f->PredictedSkill && m_info._fighters.at(x)->RespectsTargeting(f, f->PredictedSkill->_targetMode))
		{
			valid = true;
			break;
		}

	return valid;
}

void BattleManager::TurnStart()
{
	Camera::_currentCam->SetScale(Vector3f(1.2f));
	Camera::_currentCam->SetFollow(Camera::_currentCam->MapCenter());

	// Make sure enemy targets are still valid
	for (auto& x : m_info._fighters)
	{
		// Setup next skill if the current skill targets are invalidated
		if ((x->Team != 0 && x->PredictedSkill == nullptr)
			|| (x->PredictedSkill != nullptr && ValidateTargets(x) == false))
		{
			x->PredictNextSkill(x, &m_info._fighters);
			print_attack_prediction(x);
		}
	}
	std::cout << std::endl;

	if (m_info._owner->Team != 0)
	{
		for (auto x : *m_info._owner->GetTargets())
			m_info._targets.insert(x);
	}

	// Check to see if predictions should be dispayed
	//{
	//	bool shouldDisplay = true;
	//	for (auto& x : m_info._fighters)
	//	{
	//		if (x->_Fighter->NoPredictCountDown > 0)
	//		{
	//			if (x == m_info._owner)
	//				x->_Fighter->NoPredictCountDown--;
	//			shouldDisplay = false;
	//		}
	//	}

	//	_hud.ToggleTurnOrderDisplay(shouldDisplay);
	//}


	MoveToLight(true);
	if (m_info._owner->Dead)
		m_info._state = BS_TurnEnd;
	else
		m_info._state = BS_SelectAction;

	m_graphics->_owner = m_info._owner->GetId();
}

void BattleManager::print_attack_prediction(Fighter_ptr fighter)
{
	if (fighter->PredictedSkill == NULL)
		std::cout << "Gel at " << fighter->_BattleFieldPosition << " will do nothing." << std::endl;
	else
		std::cout << "Gel at " << fighter->_BattleFieldPosition << " will attack girl at " << fighter->GetTargets()->front() << " for " << fighter->PredictedSkill->_preCalculatedDamage._value << " damage" << std::endl;
}

// BS_SelectAction and BS_SelectTarget are both purely input handled for the player
void BattleManager::SelectAction()
{
	// Choose ENEMY action
	if (m_info._owner->Team != 0)
	{
		// Try to choose a skill, if it doesnt work then skip your turn
		if (m_info._owner->PredictedSkill == NULL)
		{
			m_info._state = BS_ActionDone;
			return;
		}

		m_info._selectedSkill = m_info._owner->PredictedSkill;
		//m_info._targets = m_info._owner->PredictedSkill->m_info._targets;

		UseSkill();
	}
}

void BattleManager::ActionProgress()
{
	if (Animating() && m_info._selectedSkill == NULL)
		return;

	if (m_info._selectedSkill == NULL && m_info._owner->PredictedSkill != NULL)
	{
		m_info._selectedSkill = m_info._owner->PredictedSkill;
		//m_info._targets = m_info._owner->GetTargets();
		if (m_info._owner->Team != 0)
			m_info._selectedSkill->Setup();
		return;
	}

	if (m_info._selectedSkill != NULL)
	{
		if (m_info._selectedSkill->_done)
		{
			m_info._state = BS_ActionDone;
		}
		else
		{
			m_info._selectedSkill->HandleActionCommand(m_graphics->get_animation_progress());
			if (!Animating())
			{
				SkillProgress prog = m_info._selectedSkill->Update();
				ProcessSkillReactions(prog);
				ProcessSkill();
			}
		}
	}
	else
	{
		m_info._state = BS_ActionDone;
	}
}

void BattleManager::ActionDone()
{
	if (m_info._selectedSkill)
		m_info._selectedSkill->Reset();
	m_info._selectedSkill = NULL;
	m_info._state = BS_TurnEnd;
}

void BattleManager::TurnEnd()
{
	m_info._owner->PredictedSkill = NULL;
	m_info._owner->UpdateObservers();

	// Setup next skill
	//if (m_info._owner->Team != 0)
	//{
	//	if (m_info._owner->PredictNextSkill(m_info._owner, &m_info._fighters))
	//		PrintAttackPrediction(m_info._owner);
	//}

	MoveToLight(false, true);
	m_info.cycle_actors();

	m_info._state = BS_TurnStart;
	for (unsigned int i = 0; i < m_info._fighterQueue.size(); i++)
		m_info._fighterQueue[i]->_OrderPosition = i + 1;
}

void BattleManager::ResetPartyPredictedSkills()
{
	for (auto& x : m_info._fighters)
		if (x->Team == 0)
			x->PredictedSkill = NULL;
}

void BattleManager::UpdateLogic()
{
	// Display or stop displaying skills
	UpdateSkillDisplay();

	// If there are animations, let them run out
	if (m_info._state == BS_ActionProgress || !Animating())
	{
		switch (m_info._state)
		{
		case BS_ChooseActor:
			// Method just for show, selection is handled in input
			SelectActor();
			break;
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

void BattleManager::ManageInput()
{
	// When you hold R2 or D, show arrows, udpate once on action done in case an actor loses vision of enemy predictions
	if (InputManager::GetInstance().FrameKeyStatus(A_AltR, KeyStatus::Release) ||
		InputManager::GetInstance().FrameKeyStatus(A_AltR, KeyStatus::KeyPressed) ||
		m_info._state == BS_ActionDone)
	{
		bool status = !InputManager::GetInstance().FrameKeyStatus(A_AltR, KeyStatus::AnyPress);
		//_hud.ToggleTurnOrderDisplay(status);
		//_hud.ToggleDamagePredictionArrowDisplay(status);
	}

	// Dont allow any input if theres an animation running
	// This doesnt apply if theres a skill in progress, gotta be interactive!
	if ((Animating() || m_info._owner->Team != 0) && m_info._state != BS_ActionProgress)
		return;

	// Get input
	std::set<int> input;
	KeyStatus status = KeyPressed;

	// Accept
	if (InputManager::GetInstance().FrameKeyStatus(A_Accept, status))
		input.emplace(A_Accept);

	// Menu
	if (InputManager::GetInstance().FrameKeyStatus(A_Menu, status))
		input.emplace(A_Menu);

	// Cancel
	if (InputManager::GetInstance().FrameKeyStatus(A_Cancel, status))
		input.emplace(A_Cancel);

	// Directions
	for (int i = A_Right; i < A_Up + 1; i++)
		if (InputManager::GetInstance().FrameKeyStatus((InputAction)i, status))
			input.emplace(i);

	// If theres an animation going, SEND THE INPUT TO THE SKILL
	if (m_info._state != BS_SelectTargets && m_info._state != BS_SelectAction && m_info._state != BS_ChooseActor)
	{
		if (m_info._selectedSkill != NULL)
			m_info._selectedSkill->_input = input;
		return;
	}

	// Up/Down input
	if (input.count(A_Down) || input.count(A_Up))
		HandleUpDownInput(input);

	// Left/right input
	if (input.count(A_Left) || input.count(A_Right))
		HandleLeftRightInput(input);

	// Accept input
	if (input.count(A_Accept))
		HandleAcceptInput();

	// Cancel input
	if (input.count(A_Cancel))
		HandleCancelInput();
}

void BattleManager::HandleUpDownInput(std::set<int> input)
{
	bool down = false;
	if (input.count(A_Down))
		down = true;

	// Choose your targets
	if (m_info._state == BS_SelectTargets)
	{
		if (m_info._selectedSkill->_targetAmount == TA_Party)
		{
		}
		else if (m_info._state == BS_ChooseActor || m_info._selectedSkill && m_info._selectedSkill->_targetAmount == TA_One)
		{
			if ((down && ((*m_info._selectedIndices.begin() > 0 && *m_info._selectedIndices.begin() < m_info.get_team_size(0)) || *m_info._selectedIndices.begin() > m_info.get_team_size(0))) ||
				(!down && (*m_info._selectedIndices.begin() < m_info.get_team_size(0) - 1 || (*m_info._selectedIndices.begin() >= m_info.get_team_size(0) && *m_info._selectedIndices.begin() < (int)m_info._fighters.size() - 1))))
			{
				for (int i = *m_info._selectedIndices.begin() + (down ? -1 : 1);
					(down && i >= 0) || (!down && i < (int)m_info._fighters.size());
					down ? i-- : i++)
				{
					// If up or down would bring you to the other team
					if ((*m_info._selectedIndices.begin() < m_info.get_team_size(0) && i >= m_info.get_team_size(0)) || (*m_info._selectedIndices.begin() >= m_info.get_team_size(0) && i < m_info.get_team_size(0)))
						break;
					if (m_info._state == BS_ChooseActor && !m_info._fighters[i]->Dead || m_info._fighters[i]->RespectsTargeting(m_info._owner, m_info._selectedSkill->_targetMode))
					{
						m_info.select_index(i);
						break;
					}
				}
			}
		}
	}
}
void BattleManager::HandleLeftRightInput(std::set<int> input)
{
	bool left = false;
	if (input.count(A_Left))
		left = true;

	// Attempt to manage LEFT logic immediately here
	if (m_info._state == BS_SelectTargets)
	{
		// Only do anything if the cursor is on the enemy side
		if ((left && *m_info._selectedIndices.begin() >= m_info.get_team_size(0)) || (!left && *m_info._selectedIndices.begin() < m_info.get_team_size(0)))
		{
			std::set<int> result;
			if (m_info._selectedSkill->_targetAmount == TA_Party)
			{
				for (unsigned int i = 0; i < m_info._fighters.size(); i++)
					if (((left && m_info._fighters.at(i)->Team == 0) || (!left && m_info._fighters.at(i)->Team != 0))
						&& m_info._fighters.at(i)->RespectsTargeting(m_info._owner, m_info._selectedSkill->_targetMode))
						result.emplace(i);
			}
			else if (m_info._selectedSkill->_targetAmount == TA_One)
			{
				bool found = false;
				int value = *m_info._selectedIndices.begin();
				int increment = 0;

				while (!found)
				{
					if (left)
					{
						// Try to find nearest target straight left
						if (value + increment - m_info.get_team_size(0) < m_info.get_team_size(0) && m_info._fighters[value - m_info.get_team_size(0) + increment]->RespectsTargeting(m_info._owner, m_info._selectedSkill->_targetMode))
							m_info.select_index(value - m_info.get_team_size(0) + increment);
						else if (value - increment - m_info.get_team_size(0) >= 0 && m_info._fighters[value - m_info.get_team_size(0) - increment]->RespectsTargeting(m_info._owner, m_info._selectedSkill->_targetMode))
							m_info.select_index(value - m_info.get_team_size(0) - increment);
					}
					else
					{
						// Try to find nearest target straight right
						if (value + increment + m_info.get_team_size(0) < (int)m_info._fighters.size() && m_info._fighters[value + m_info.get_team_size(0) + increment]->RespectsTargeting(m_info._owner, m_info._selectedSkill->_targetMode))
							m_info.select_index(value + m_info.get_team_size(0) + increment);
						else if (value - increment + m_info.get_team_size(0) >= m_info.get_team_size(0) && m_info._fighters[value + m_info.get_team_size(0) - increment]->RespectsTargeting(m_info._owner, m_info._selectedSkill->_targetMode))
							m_info.select_index(value + m_info.get_team_size(0) - increment);
					}

					if (value != *m_info._selectedIndices.begin())
						found = true;

					increment++;
					// If the counter goes too high, then there was no valid target on this side
					if (increment >= m_info.get_team_size(0))
						found = true;
				}

				result.emplace(*m_info._selectedIndices.begin());
			}

			if (result.size() != 0)
				m_info.select_indices(result);
		}
	}
}
void BattleManager::HandleAcceptInput()
{
	if (m_info._state == BS_SelectAction)
	{
		//m_info._selectedSkill = m_info._chooseSkill.at(*m_info._selectedIndices.begin());
		m_info._selectedSkill = m_info._chooseSkill.at(m_graphics->get_menu_index());
		//std::cout << "Skill selected: " << m_info._selectedSkill->_name << std::endl;
		m_info._state = BS_SelectTargets;
		std::set<int> targets = DefaultTargetActorIndex();
		m_info.select_indices(targets);
	}
	else if (m_info._state == BS_SelectTargets)
	{
		m_info._targets = m_info._selectedIndices;

		if ((int)m_info._targets.size() < m_info._selectedSkill->_minTargets)
			return;

		m_info._selectedSkill->Reset();
		UseSkill();
		m_info.select_index(0);
		//m_info._targets = std::vector<int>();
	}
	else if (m_info._state == BS_ChooseActor)
	{
		m_info._owner = m_info._fighters.at(*m_info._selectedIndices.begin());


		m_info._state = BS_SelectAction;
		m_info._chooseSkill = m_info._owner->GetSkills();
		m_info.select_index(0);

		//m_info._selectedSkill->Reset();
		//UseSkill();
	}
}
void BattleManager::HandleCancelInput()
{
	if (m_info._state == BS_SelectTargets)
	{
		m_info.select_index(0);
		m_info._state = BS_SelectAction;
		//m_info._showingSkills = true;
	}
}

std::set<int> BattleManager::DefaultTargetActorIndex()
{
	std::set<int> result;
	bool type = m_info._selectedSkill->_targetAmount;
	for (int i = 0; i < (int)m_info._fighters.size(); i++)
	{
		Fighter_ptr fighter = m_info._fighters[i];

		if (m_info._selectedSkill->_defaultTarget == DT_Self && (fighter == m_info._owner || (type == TA_Party && fighter->SameTeam(m_info._owner))))
			result.emplace(i);
		else if (m_info._selectedSkill->_defaultTarget != DT_Self && (fighter->RespectsTargeting(m_info._owner, m_info._selectedSkill->_targetMode)
			&& (fighter->SameTeam(m_info._owner) == (m_info._selectedSkill->_defaultTarget == DT_Ally))))
			result.emplace(i);

		if (result.size() && type == TA_One)
			break;
	}

	if (!result.size())
		result.emplace(-1);

	return result;
}

void BattleManager::UseSkill()
{
	//std::cout << "Using skill: " << m_info._selectedSkill->_name << std::endl;
	if (m_info._selectedSkill != NULL)
	{
		m_info._state = BS_ActionProgress;
		m_info._owner->PredictedSkill = m_info._selectedSkill;
	}
	else
		m_info._state = BS_SelectAction;
}


// Return winning team, -1 if battle isn't over
int BattleManager::FindWinner()
{
	std::set<int> activeTeams;
	for (auto x : m_info._fighterQueue)
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
