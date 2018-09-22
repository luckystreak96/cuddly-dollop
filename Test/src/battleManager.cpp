#include "battleManager.h"
#include "input_manager.h"
#include <GLFW/glfw3.h>
#include <set>
#include <algorithm>
#include <iostream>
#include "fontManager.h"
#include "localizationData.h"
#include "battleAnimationManager.h"

using namespace std;

BattleManager::BattleManager()
{
	Init();
}

bool BattleManager::Animating()
{
	return m_graphics->Animating();
}

BattleManager::BattleManager(std::vector<Fighter_ptr> actors)
{
	// Populate actors
	for (auto& a : actors)
	{
		// Reset modified stats
		a->ResetModified();
		_actors.push_back(a);
	}

	for (auto a : _actors)
		_actorQueue.push_back(a);
	std::sort(_actorQueue.begin(), _actorQueue.end(), Fighter::FighterSpeedSort);

	Init();
}

BattleManager::~BattleManager()
{
	for (auto& x : _fonts)
		FontManager::GetInstance().RemoveFont(x);
}

void BattleManager::ProcessSkill()
{
	std::vector<triple> operations = _selectedSkill->GetAnimations();

	for (auto& x : operations)
	{
		// need to fill in the vector of floats
		// ('<' because of AARG_FloatAsync)
		if (get<1>(x) < AARG_Float)
			SetSkillArguments(x);

		// x is an action
		if (get<0>(x) > AA_Start)
		{
			// Deal damage
			int target = (int)get<2>(x).front();
			Damage dmg = HandleDamage(target);
			// Show dmg
			m_graphics->DamageAnimation(target, _selectedSkill, dmg);
		}
		// x is an animation
		else
		{
			Anim_ptr animation = m_graphics->CreateAnimation(x);
			if (animation)
				m_graphics->insert_animation(animation);
		}
	}
}

void BattleManager::SetSkillArguments(triple& x)
{
	get<2>(x).clear();
	switch (get<1>(x))
	{
	case AARG_Owner:
		get<2>(x).push_back((float)_owner->GetId());
		break;
	case AARG_Targets:
		for (auto y : _targets)
			get<2>(x).push_back((float)y);
		break;
	case AARG_Target:
		get<2>(x).push_back((float)*_targets.begin());
		break;
	default:
		break;
	}
}


Damage BattleManager::HandleDamage(int target)
{
	Damage dmg = _selectedSkill->CalculateDamage(*_owner);
	Fighter_ptr& f = _actors.at(target);

	// Apply first damage modifications
	if (_selectedSkill->_skillType != ST_Healing)
		f->DamageModifiers(dmg, dmg._critting);

	if (_selectedSkill->_ac._success)
	{
		// Your team under attack from enemy -> Defense Action Command
		if (_owner->Team != 0 && f->Team == 0)
		{
			if (_selectedSkill->_ac._type == ACT_Special)
				f->SpecialActionCommand(dmg);
			else if (_selectedSkill->_skillType == ST_Physical)
				f->PhysicalDefenseActionCommand(dmg);
			else
				f->MagicalDefenseActionCommand(dmg);
		}
		// Your team attacking -> Offense Action Command
		else if (_owner->Team == 0)
		{
			if (_selectedSkill->_skillType == ST_Physical)
				f->PhysicalOffenseActionCommand(dmg);
			else
				f->MagicalOffenseActionCommand(dmg);
		}
	}

	// Deal the dmg
	if (_selectedSkill->_skillType == ST_Healing)
		f->ApplyHealing(dmg);
	else
		f->TakeDamage(dmg);

	// Check for bonus damage
	if (_selectedSkill->_skillElement != SE_None)
	{
		SkillElement targetElement;
		if (_selectedSkill->_skillElement == SE_Determined)
			targetElement = SE_Pragmatic;
		else if (_selectedSkill->_skillElement == SE_Pragmatic)
			targetElement = SE_StrongWilled;
		else if (_selectedSkill->_skillElement == SE_StrongWilled)
			targetElement = SE_Determined;

		//if (f->HasElement(targetElement))
			//	_anims->push_front(Anim_ptr(new AnimBonusEffect(_owner.lock(), _targets.at(0))));
			//ApplyBonusEffect(f);
	}

	return dmg;
}

//void BattleManager::ApplyBonusEffect(Fighter_ptr target)
//{
//	Damage dmg = Damage();
//	dmg._value = 10;
//	dmg._type = SkillType::ST_Bonus;
//
//	// Damage text
//	SpawnDamageText(target, dmg);
//	_anims->push_front(Anim_ptr(new AnimColorFlash(Vector3f(3, 3, 5), target)));
//	//_anims->push_front(Anim_ptr(new AnimScreenShake()));
//
//	Particle_ptr particles = Particle_ptr(new ParticleGenerator());
//	Vector3f pos = target->_Graphics->GetPos() + Vector3f(0.5f, 0.5f, 0.6f);
//	particles->SetPowerLevel(0.3f);
//	particles->Init(PT_Explosion, dmg._value, pos, false, "star.png");
//	Vector3f color = Vector3f(1.f, 1.f, 1.f);
//	particles->SetColor(color);
//	ParticleManager::GetInstance().AddParticles(particles);
//
//	target->TakeDamage(dmg);
//}

void BattleManager::Init()
{
	m_graphics = std::shared_ptr<BattleAnimationManager>(new BattleAnimationManager());

	bool singleFile = true;

	{
		m_singleFileAttacks = singleFile;
		m_isPlayerTurn = !singleFile;
	}
	m_attackSequenceProgress = 0;
	//_hud.Init(_actors);
	_postBattleState = PBS_FightingInProgress;
	_showingSkills = false;
	_state = m_singleFileAttacks ? BS_TurnStart : BS_ChooseActor;
	m_animating = false;
	counter = 0;
	_winner = -1;
	Select(0);
	_numAllies = 0;

	// Make sure that there are 2 teams to fight against one another
	{
		std::set<int> teams;
		for (auto& x : _actors)
			if (!teams.count(x->Team))
				teams.emplace(x->Team);

		// If there aren't 2 teams, end the battle
		if (teams.size() < 2)
			_postBattleState = PBS_FightingDone;
	}

	// Make sure the first person to choose is a player
	if (m_singleFileAttacks && _postBattleState == PBS_FightingInProgress)
		while (_actorQueue.front()->Team != 0)
			CycleActors();

	if (_actorQueue.size() > 0)
	{
		_chooseSkill = _actorQueue.front()->GetSkills();
		_owner = _actorQueue.front();
	}

	for (auto& x : _actors)
	{
		// Setup next skill
		if (x->Team != 0)
		{
			x->PredictNextSkill(x, &_actors);
			PrintAttackPrediction(x);
		}
		else
			_numAllies++;
	}

	if (!m_singleFileAttacks && _actors.size())
		InitiateChooseActor();

	for (unsigned int i = 0; i < _actorQueue.size(); i++)
		_actorQueue[i]->_OrderPosition = i + 1;
}

void BattleManager::UpdateColors()
{
	for (auto& x : _actors)
	{
		if (_state == BS_SelectAction)
		{
			if (_owner->Team == 0)
			{
				if (_fonts.size() > (unsigned int)*_selectedIndices.begin())
				{
					for (auto x : _fonts)
						FontManager::GetInstance().GetFont(x)->GetGraphics()->SetColorAll();
					FontManager::GetInstance().GetFont(_fonts[*_selectedIndices.begin()])->GetGraphics()->SetColorAll(Vector3f(1, 0, 0));
				}
			}
		}
		else
		{
			bool deny = false;
			if (_state != BS_SelectTargets)
				deny = true;
			int actionCommandLevel = 0;
			if (_owner->_BattleFieldPosition == x->GetId() && _selectedSkill)
				actionCommandLevel = _selectedSkill->action_command_level(m_graphics->get_animation_progress());
			m_graphics->UpdateColors(x->GetId(), !deny && _selectedIndices.count(x->_BattleFieldPosition), x->Dead, actionCommandLevel);
		}
	}
}

void BattleManager::Update()
{
	// Return if the battle logic is over
	if (_postBattleState == PBS_FightingInProgress/* && !Animating()*/)// || _selectedSkill && !_selectedSkill->_done)
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
	if (/*!Animating() && */_state == BS_TurnStart)
	{
		switch (_postBattleState)
		{
		case PBS_FightingInProgress:
			_winner = FindWinner();
			if (_winner != -1)
			{
				Camera::_currentCam->SetFollow(Camera::_currentCam->MapCenter());
				_postBattleState = PBS_FightingDone;
			}
			break;
		case PBS_FightingDone:
			for (auto& x : _actors)
				x->NoPredictCountDown = 0;
			break;
		case PBS_ExpAnimation:
		{
			int xp = 0;
			for (auto& actor : _actors)
				if (actor->Team != 0)
					xp += actor->DeathExp;

			for (auto& actor : _actors)
			{
				if (actor->Team == 0 && !actor->Dead)
				{
					ExpAnimation(actor, xp);
				}
			}

			_postBattleState = PBS_ConfirmCompletion;
		}
		break;
		case PBS_PostBattleComplete:
			break;
		default:
			break;
		}
	}

	_hud.Update();
}

void BattleManager::ExpAnimation(Fighter_ptr fighter, int xp)
{
	m_graphics->insert_animation(_hud.GetActorHealthBar(fighter)->SetupExpAnimation(fighter->GetExp() + xp));
	m_graphics->CreateFloatingText(fighter->_BattleFieldPosition, "+" + std::to_string(xp) + " XP");
}

// Move Animation
void BattleManager::MoveToLight(bool moveup, bool turnEnd)
{
	if (!_owner->Dead || turnEnd)
		m_graphics->MoveUp(_owner->_BattleFieldPosition, moveup);

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
	else if (_owner->Team == 0 && !_showingSkills && !Animating() && _state == BS_SelectAction)
	{
		SetChooseSkillText();
		_showingSkills = true;
		Select(0);
	}
}

bool BattleManager::ValidateTargets()
{
	bool valid = false;

	// Check if all targets are alive
	for (auto& x : _targets)
		if (_actors.at(x)->RespectsTargeting(_owner, _selectedSkill->_targetMode))
		{
			valid = true;
			break;
		}

	return valid;
}

void BattleManager::TurnStart()
{
	// Make sure enemy targets are still valid
	for (auto& x : _actors)
	{
		// Setup next skill if the current skill targets are invalidated
		if (x->Team != 0 && x->PredictedSkill != NULL && ValidateTargets() == false)
		{
			x->PredictNextSkill(x, &_actors);
			PrintAttackPrediction(x);
		}
	}

	if (_owner->Team != 0)
	{
		for (auto x : _owner->GetTargets())
			_targets.insert(x);
	}

	// Check to see if predictions should be dispayed
	//{
	//	bool shouldDisplay = true;
	//	for (auto& x : _actors)
	//	{
	//		if (x->_Fighter->NoPredictCountDown > 0)
	//		{
	//			if (x == _owner)
	//				x->_Fighter->NoPredictCountDown--;
	//			shouldDisplay = false;
	//		}
	//	}

	//	_hud.ToggleTurnOrderDisplay(shouldDisplay);
	//}


	MoveToLight(true);
	if (_owner->Dead)
		_state = BS_TurnEnd;
	else
	{
		_owner->TurnStart(_actors);
		if (m_singleFileAttacks)
			_state = BS_SelectAction;
		else
			_state = BS_ActionProgress;
	}

	m_graphics->_owner = _owner->GetId();
}

void BattleManager::PrintAttackPrediction(Fighter_ptr x)
{
	if (x->PredictedSkill == NULL)
		std::cout << "Gel at " << x->_BattleFieldPosition << " will do nothing." << std::endl;
	else
		std::cout << "Gel at " << x->_BattleFieldPosition << " will attack girl at " << "?" << " for " << x->PredictedSkill->_preCalculatedDamage._value << " damage" << std::endl;
}

// BS_SelectAction and BS_SelectTarget are both purely input handled for the player
void BattleManager::SelectAction()
{
	// Choose ENEMY action
	if (_owner->Team != 0)
	{
		// Try to choose a skill, if it doesnt work then skip your turn
		if (_owner->PredictedSkill == NULL)
		{
			_state = BS_ActionDone;
			return;
		}

		_selectedSkill = _owner->PredictedSkill;
		//_targets = _owner->PredictedSkill->_targets;

		UseSkill();
	}
}

void BattleManager::SelectActor()
{

}

void BattleManager::InitiateChooseActor()
{
	_state = BS_ChooseActor;
	RemoveChooseSkillText();
	_showingSkills = false;

	Select(0);

	// Choose who to select by default
	for (unsigned int i = 0; i < _actors.size(); i++)
	{
		if (_actors[i]->Team == 0 && _actors[i]->PredictedSkill == NULL)
		{
			Select(i);
			break;
		}
	}

	//Select(_selectedIndices);
	Camera::_currentCam->SetScale(Vector3f(1));
	Camera::_currentCam->SetFollow(Camera::_currentCam->MapCenter());
}

void BattleManager::SelectTargets()
{
}

void BattleManager::ActionProgress()
{
	if (Animating() && _selectedSkill == NULL)
		return;

	if (_selectedSkill == NULL && _owner->PredictedSkill != NULL)
	{
		_selectedSkill = _owner->PredictedSkill;
		//_targets = _owner->GetTargets();
		if (_owner->Team != 0)
			_selectedSkill->Setup();
		_owner->UseSkill();
		return;
	}

	if (_selectedSkill != NULL)
	{
		if (_selectedSkill->_done)
		{
			_state = BS_ActionDone;
		}
		else
		{
			_selectedSkill->HandleActionCommand(m_graphics->get_animation_progress());
			if (!Animating())
			{
				_selectedSkill->Update();
				ProcessSkill();
			}
		}
	}
	else
	{
		_state = BS_ActionDone;
	}
}

void BattleManager::ActionDone()
{
	if (_selectedSkill)
		_selectedSkill->Reset();
	_selectedSkill = NULL;
	_state = BS_TurnEnd;
}

void BattleManager::TurnEnd()
{
	_owner->PredictedSkill = NULL;
	_owner->UpdateObservers();

	// Setup next skill
	if (_owner->Team != 0)
	{
		if (_owner->PredictNextSkill(_owner, &_actors))
			PrintAttackPrediction(_owner);
	}

	MoveToLight(false, true);
	CycleActors();

	if (m_singleFileAttacks)
	{
		_state = BS_TurnStart;
		for (unsigned int i = 0; i < _actorQueue.size(); i++)
			_actorQueue[i]->_OrderPosition = i + 1;
	}
	else
	{
		if (!m_isPlayerTurn)
			_state = BS_TurnStart;
		else
		{
			InitiateChooseActor();
			ResetPartyPredictedSkills();
			for (unsigned int i = 0; i < _actorQueue.size(); i++)
				_actorQueue[i]->_OrderPosition = i + 1;
		}
	}
}

void BattleManager::ResetPartyPredictedSkills()
{
	for (auto& x : _actors)
		if (x->Team == 0)
			x->PredictedSkill = NULL;
}

void BattleManager::UpdateLogic()
{
	// Display or stop displaying skills
	UpdateSkillDisplay();

	// If there are animations, let them run out
	if (_state == BS_ActionProgress || !Animating())
	{
		switch (_state)
		{
		case BS_ChooseActor:
			if (m_isPlayerTurn)
				// Method just for show, selection is handled in input
				SelectActor();
			break;
		case BS_TurnStart:
			// zoom back to normal
			if (m_singleFileAttacks)
			{
				Camera::_currentCam->SetScale(Vector3f(1));
				Camera::_currentCam->SetFollow(Camera::_currentCam->MapCenter());
				TurnStart();
			}
			else
			{
				if (!m_isPlayerTurn)
				{
					Camera::_currentCam->SetScale(Vector3f(1));
					Camera::_currentCam->SetFollow(Camera::_currentCam->MapCenter());
					TurnStart();
				}
			}
			break;
		case BS_SelectAction:
			// BS_SelectAction and BS_SelectTarget are both purely input handled for the player
			SelectAction();
			break;
		case BS_SelectTargets:
			// Handled by input, method only for show
			if (m_singleFileAttacks || (m_isPlayerTurn && !m_singleFileAttacks))
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
	while (_fonts.size() < _chooseSkill.size())
		_fonts.push_back(FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png"));

	for (unsigned int i = 0; i < _chooseSkill.size(); i++)
	{
		FontManager::GetInstance().EnableFont(_fonts[i]);
		FontManager::GetInstance().SetScale(_fonts[i], 0.5f, 0.5f);
		FontManager::GetInstance().SetText(_fonts[i], _(_chooseSkill.at(i)->_name), Vector3f(5.0f, 4.75f - i * 0.5f, 0));
		//FontManager::GetInstance().GetFont(_fonts[i])->SetText(_chooseSkill->at(i)->_name, Vector3f(6, 7 - i, 1));
	}
}

void BattleManager::RemoveChooseSkillText()
{
	for (unsigned int i = 0; i < _fonts.size(); i++)
		FontManager::GetInstance().GetFont(_fonts[i])->_enabled = false;
}


void BattleManager::ManageInput()
{
	// When you hold R2 or D, show arrows, udpate once on action done in case an actor loses vision of enemy predictions
	if (InputManager::GetInstance().FrameKeyStatus(A_AltR, KeyStatus::Release) ||
		InputManager::GetInstance().FrameKeyStatus(A_AltR, KeyStatus::KeyPressed) ||
		_state == BS_ActionDone)
	{
		bool status = !InputManager::GetInstance().FrameKeyStatus(A_AltR, KeyStatus::AnyPress);
		_hud.ToggleTurnOrderDisplay(status);
		_hud.ToggleDamagePredictionArrowDisplay(status);
	}

	// Dont allow any input if theres an animation running
	// This doesnt apply if theres a skill in progress, gotta be interactive!
	if ((Animating() || _owner->Team != 0) && _state != BS_ActionProgress)
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
	if (_state != BS_SelectTargets && _state != BS_SelectAction && _state != BS_ChooseActor)
	{
		if (_selectedSkill != NULL)
			_selectedSkill->_input = input;
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

	if (!m_singleFileAttacks && _state == BS_ChooseActor && input.count(A_Menu))
		BeginAnimations();
}

void BattleManager::BeginAnimations()
{
	Select(std::set<int>());
	m_isPlayerTurn = false;
	_owner = _actorQueue.front();
	_selectedSkill = NULL;
	_state = BS_TurnStart;
}

void BattleManager::HandleUpDownInput(std::set<int> input)
{
	bool down = false;
	if (input.count(A_Down))
		down = true;

	// Choose your skill
	if (_state == BS_SelectAction)
	{
		int selectTarget = 0;
		if (down && *_selectedIndices.begin() < (int)_chooseSkill.size() - 1)
		{
			selectTarget = *_selectedIndices.begin() + 1;
		}
		else if (!down)
		{
			if (*_selectedIndices.begin() > 0)
				selectTarget = *_selectedIndices.begin() - 1;
			else
				selectTarget = _chooseSkill.size() - 1;
		}

		Select(selectTarget);
	}

	// Choose your targets
	else if (_state == BS_SelectTargets || !m_singleFileAttacks && _state == BS_ChooseActor)
	{
		if ((!m_singleFileAttacks && _state != BS_ChooseActor && _selectedSkill && _selectedSkill->_targetAmount == TA_Party) || m_singleFileAttacks && _selectedSkill->_targetAmount == TA_Party)
		{
		}
		else if (_state == BS_ChooseActor || _selectedSkill && _selectedSkill->_targetAmount == TA_One)
		{
			if ((down && ((*_selectedIndices.begin() > 0 && *_selectedIndices.begin() < _numAllies) || *_selectedIndices.begin() > _numAllies)) ||
				(!down && (*_selectedIndices.begin() < _numAllies - 1 || (*_selectedIndices.begin() >= _numAllies && *_selectedIndices.begin() < (int)_actors.size() - 1))))
			{
				for (int i = *_selectedIndices.begin() + (down ? -1 : 1);
					(down && i >= 0) || (!down && i < (int)_actors.size());
					down ? i-- : i++)
				{
					// If up or down would bring you to the other team
					if ((*_selectedIndices.begin() < _numAllies && i >= _numAllies) || (*_selectedIndices.begin() >= _numAllies && i < _numAllies))
						break;
					if (_state == BS_ChooseActor && !_actors[i]->Dead || _actors[i]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
					{
						Select(i);
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
	if (_state == BS_SelectTargets)
	{
		// Only do anything if the cursor is on the enemy side
		if ((left && *_selectedIndices.begin() >= _numAllies) || (!left && *_selectedIndices.begin() < _numAllies))
		{
			std::set<int> result;
			if (_selectedSkill->_targetAmount == TA_Party)
			{
				for (unsigned int i = 0; i < _actors.size(); i++)
					if (((left && _actors.at(i)->Team == 0) || (!left && _actors.at(i)->Team != 0))
						&& _actors.at(i)->RespectsTargeting(_owner, _selectedSkill->_targetMode))
						result.emplace(i);
			}
			else if (_selectedSkill->_targetAmount == TA_One)
			{
				bool found = false;
				int value = *_selectedIndices.begin();
				int increment = 0;

				while (!found)
				{
					if (left)
					{
						// Try to find nearest target straight left
						if (value + increment - _numAllies < _numAllies && _actors[value - _numAllies + increment]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
							Select(value - _numAllies + increment);
						else if (value - increment - _numAllies >= 0 && _actors[value - _numAllies - increment]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
							Select(value - _numAllies - increment);
					}
					else
					{
						// Try to find nearest target straight right
						if (value + increment + _numAllies < (int)_actors.size() && _actors[value + _numAllies + increment]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
							Select(value + _numAllies + increment);
						else if (value - increment + _numAllies >= _numAllies && _actors[value + _numAllies - increment]->RespectsTargeting(_owner, _selectedSkill->_targetMode))
							Select(value + _numAllies - increment);
					}

					if (value != *_selectedIndices.begin())
						found = true;

					increment++;
					// If the counter goes too high, then there was no valid target on this side
					if (increment >= _numAllies)
						found = true;
				}

				result.emplace(*_selectedIndices.begin());
			}

			if (result.size() != 0)
				Select(result);
		}
	}
}
void BattleManager::HandleAcceptInput()
{
	if (_state == BS_SelectAction)
	{
		_selectedSkill = _chooseSkill.at(*_selectedIndices.begin());
		//std::cout << "Skill selected: " << _selectedSkill->_name << std::endl;
		_state = BS_SelectTargets;
		std::set<int> targets = DefaultTargetActorIndex(&_actors, _owner, _selectedSkill);
		Select(targets);
	}
	else if (_state == BS_SelectTargets)
	{
		_targets = _selectedIndices;

		if ((int)_targets.size() < _selectedSkill->_minTargets)
			return;

		_selectedSkill->Reset();
		UseSkill();
		Select(0);
		//_targets = std::vector<int>();
	}
	else if (_state == BS_ChooseActor)
	{
		_owner = _actors.at(*_selectedIndices.begin());


		_state = BS_SelectAction;
		_chooseSkill = _owner->GetSkills();
		Select(0);

		//_selectedSkill->Reset();
		//UseSkill();
	}
}
void BattleManager::HandleCancelInput()
{
	if (_state == BS_SelectTargets)
	{
		Select(0);
		_state = BS_SelectAction;
		SetChooseSkillText();
		_showingSkills = true;
	}
	else if (!m_singleFileAttacks && _state == BS_SelectAction)
	{
		InitiateChooseActor();
	}

}

std::set<int> BattleManager::DefaultTargetActorIndex(std::vector<Fighter_ptr>* actors, Fighter_ptr owner, Skill_ptr selectedSkill)
{
	std::set<int> result;
	bool type = selectedSkill->_targetAmount;
	int i;
	bool done = false;
	for (i = 0; i < (int)actors->size(); i++)
	{
		switch (selectedSkill->_defaultTarget)
		{
		case DT_Self:
			if ((actors->at(i) == owner && type == TA_One) || (actors->at(i)->Team == owner->Team && type == TA_Party))
			{
				result.emplace(i);
				if (type == TA_One)
					done = true;
			}
			break;
		case DT_Ally:
			if (actors->at(i)->Team == owner->Team && actors->at(i)->RespectsTargeting(owner, selectedSkill->_targetMode))
			{
				result.emplace(i);
				if (type == TA_One)
					done = true;
			}
			break;
		case DT_Enemy:
			if (actors->at(i)->Team != owner->Team && actors->at(i)->RespectsTargeting(owner, selectedSkill->_targetMode))
			{
				result.emplace(i);
				if (type == TA_One)
					done = true;
			}
			break;
		}

		if (done)
			break;
	}

	if (i >= actors->size() && result.size() == 0)
		result.emplace(0);

	return result;
}

void BattleManager::Select(int target)
{
	Select(std::set<int>{target});
}

void BattleManager::Select(std::set<int> targets)
{
	if (targets.size())
		_selectedIndices = targets;
	else
	{
		_selectedIndices.clear();
		_selectedIndices.emplace(0);
	}
}

void BattleManager::UseSkill()
{
	//std::cout << "Using skill: " << _selectedSkill->_name << std::endl;
	if (_selectedSkill != NULL)
	{
		_state = BS_ActionProgress;
		_owner->PredictedSkill = _selectedSkill;
		if (m_singleFileAttacks)
			_owner->UseSkill();
	}
	else
		_state = BS_SelectAction;

	if (!m_singleFileAttacks && m_isPlayerTurn)
	{
		InitiateChooseActor();
		_owner->UpdateObservers();
	}
}

void BattleManager::CycleActors()
{
	Fighter_ptr front = _actorQueue.front();

	_actorQueue.pop_front();
	_actorQueue.push_back(front);
	_owner = _actorQueue.front();
	_chooseSkill = _owner->GetSkills();
	_targets.clear();
	Select(0);

	if (!m_singleFileAttacks)
	{
		m_attackSequenceProgress++;
		if (m_attackSequenceProgress == _actorQueue.size())
		{
			m_attackSequenceProgress = 0;
			m_isPlayerTurn = true;
		}
	}
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
