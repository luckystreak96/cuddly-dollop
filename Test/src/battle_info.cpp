#include "battle_info.h"

battle_info::battle_info() : _state(BS_TurnStart), m_animating(false), _winner(-1)
{
	for (unsigned int i = 0; i < SkillProgress::SP_SkillLast; i++)
		_action_buffer.emplace((SkillProgress)i, std::vector<triple>());
}

void battle_info::init(std::vector<Fighter_ptr> fighters)
{
	// Populate actors
	_fighters = fighters;

	// Setup queue
	for (auto a : _fighters)
		_fighterQueue.push_back(a);

	sort_fighter_queue();

	for (unsigned int i = 0; i < _fighterQueue.size(); i++)
		_fighterQueue[i]->_OrderPosition = i + 1;

	select_index(0);

	// Make sure the first person to choose is a player
	while (_fighterQueue.front()->Team != 0)
		cycle_actors();

	_chooseSkill = _fighterQueue.front()->GetSkills();
	_owner = _fighterQueue.front();

	// Setup next skill
	for (auto& x : _fighters)
		if (x->Team != 0)
			x->PredictNextSkill(x, &_fighters);
}

int battle_info::get_team_size(int team)
{
	int counter = 0;
	for (auto& x : _fighters)
		if (x->Team == team)
			counter++;

	return counter;
}

void battle_info::recalc_fighter_stats()
{
	// Reset modified stats
	for (auto& a : _fighters)
		a->ResetModified();
}

void battle_info::sort_fighter_queue()
{
	std::sort(_fighterQueue.begin(), _fighterQueue.end(), Fighter::FighterSpeedSort);
}

void battle_info::cycle_actors()
{
	Fighter_ptr front = _fighterQueue.front();

	_fighterQueue.pop_front();
	_fighterQueue.push_back(front);
	_owner = _fighterQueue.front();
	_chooseSkill = _owner->GetSkills();
	_targets.clear();
	select_index(0);
}

int battle_info::get_num_teams()
{
	std::set<int> teams;
	for (auto& x : _fighters)
		teams.emplace(x->Team);

	return teams.size();
}

void battle_info::select_index(int target)
{
	select_indices(std::set<int>{target});
}

void battle_info::select_indices(std::set<int> targets)
{
	if (targets.size())
		_selectedIndices = targets;
	else
	{
		_selectedIndices.clear();
		_selectedIndices.emplace(0);
	}
}
