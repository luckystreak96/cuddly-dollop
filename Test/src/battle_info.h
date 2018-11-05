#ifndef BATTLE_INFO_H__
#define BATTLE_INFO_H__

#include "fighter.h"
#include "battleUnit.h"

#include <deque>
#include <vector>
#include <memory>

class battle_info
{
public:
	battle_info();
	void init(std::vector<Fighter_ptr> fighters);

	// fighter logic
	int get_team_size(int team);
	void recalc_fighter_stats();
	void sort_fighter_queue();
	void cycle_actors();

	// list logic
	int get_num_teams();

	// other logic
	void select_index(int target);
	void select_indices(std::set<int> targets);

public:
	std::deque<Fighter_ptr> _fighterQueue;
	std::vector<Fighter_ptr> _fighters;
	std::set<int> _targets;
	std::vector<Skill_ptr> _chooseSkill;
	std::vector<unsigned int> _fonts;
	std::map<SkillProgress, std::vector<triple>> _action_buffer;


	Skill_ptr _selectedSkill;
	Fighter_ptr _owner;
	std::set<int> _selectedIndices;
	BattleState _state;
	bool m_animating;
	int _winner;

private:
};

#endif // !BATTLE_INFO_H__
