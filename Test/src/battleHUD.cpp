#include "battleHUD.h"
#include "renderer.h"

BattleHUD::BattleHUD()
{
}

void BattleHUD::Init(std::vector<Actor_ptr> actors)
{
	_actors = actors;

	int counter = 0;
	for (auto& x : _actors)
	{
		GraphComp_ptr ptr = GraphComp_ptr(new GraphicsComponent("BAR", "res/sprites/special/bar.png"));
		ptr->SetPhysics(Vector3f(5, 5.f - counter * 0.75f, 0), Vector3f());
		ptr->Update();
		_actorInfo.push_back(ptr);
		counter++;
	}
	Update();
}

void BattleHUD::Update()
{
	for (int i = 0; i < _actorInfo.size(); i++)
	{
		Vector3f color = Vector3f(0, 0.6f, 0.8f);
		int health = _actors.at(i)->Health;
		int maxhealth = _actors.at(i)->MaxHealth;
		if (health > maxhealth / 2)
			color = Vector3f(0, 0.6f, 0.8f);
		else if (health <= maxhealth / 10)
			color = Vector3f(0.8f, 0, 0.4f);
		else
			color = Vector3f(1.0f, 0.6f, 0.2f);
		_actorInfo[i]->SetColorAll(color, 0.8f);
		_actorInfo[i]->GetModelMat()->SetScale(fmax((float)health / (float)maxhealth, 0), 1, 1);
		_actorInfo[i]->Update();
	}
}

void BattleHUD::SetRender()
{
	for (auto& x : _actorInfo)
		Renderer::GetInstance().Add(x);
}
