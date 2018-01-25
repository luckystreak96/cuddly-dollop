#include "battleHUD.h"
#include "renderer.h"
#include "fontManager.h"

BattleHUD::BattleHUD()
{
}

void BattleHUD::Destroy()
{
	for (auto x : _actorInfo)
		FontManager::GetInstance().RemoveFont(x._font);
}

void BattleHUD::Init(std::vector<Actor_ptr> actors)
{
	_actors = actors;

	int enemies = 0;
	int party = 0;
	for (auto& x : _actors)
	{
		// HEALTH BAR
		AddActorHealthBar(x, party, enemies);
	}
	Update();
}


void BattleHUD::AddActorHealthBar(Actor_ptr ap, int& party, int& enemies)
{
	// Bar
	GraphComp_ptr ptr = GraphComp_ptr(new GraphicsComponent("BAR", "res/sprites/special/bar.png"));
	if (ap->Team == 0)
	{
		ptr->SetPhysics(Vector3f(4.25f + 2.5f * party, 8.25f, 0), Vector3f());
		party++;
	}
	else
	{
		ptr->SetPhysics(Vector3f(4.25f + 2.5f * enemies, 1.45f, 0), Vector3f());
		enemies++;
	}
	ptr->Update();

	// Font
	unsigned int font = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(font, 0.25f, 0.25f);
	FontManager::GetInstance().GetFont(font)->_letterSpacing = 0.55f;

	// Set everything
	ActorInfoBundle aib;
	aib._type = HC_HealthBar;
	aib._graphics = ptr;
	aib._font = font;
	_actorInfo.push_back(aib);
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

		if (health <= 0)
			FontManager::GetInstance().DisableFont(_actorInfo[i]._font);
		else
		{
			Vector3f offset = Vector3f(-0.75f, -0.7f, -1);
			FontManager::GetInstance().SetText(_actorInfo[i]._font, std::to_string(health) + " / " + std::to_string(maxhealth), _actorInfo[i]._graphics->GetPos() + offset, true);
			FontManager::GetInstance().GetFont(_actorInfo[i]._font)->Update(0);
		}

		_actorInfo[i]._graphics->SetColorAll(color, 0.8f);
		_actorInfo[i]._graphics->GetModelMat()->SetScale(fmax((float)health / (float)maxhealth, 0), 1, 1);
		_actorInfo[i]._graphics->Update();
	}
}

void BattleHUD::SetRender()
{
	for (auto& x : _actorInfo)
		Renderer::GetInstance().Add(x._graphics);
}
