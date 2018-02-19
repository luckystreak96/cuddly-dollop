#include "battleHUD.h"
#include "renderer.h"
#include "fontManager.h"
#include "gameData.h"

BattleHUD::BattleHUD()
{
}

void BattleHUD::Destroy()
{
	for (auto x : _actorInfo)
	{
		FontManager::GetInstance().RemoveFont(x._name);
		FontManager::GetInstance().RemoveFont(x._health);
	}
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
	GraphComp_ptr ptr2 = GraphComp_ptr(new GraphicsComponent("BAR", "res/sprites/special/bar.png"));
	if (ap->_Fighter->Team == 0)
	{
		ptr->SetPhysics(Vector3f(4.25f + 2.5f * party, 8.25f, 0), Vector3f());
		ptr2->SetPhysics(Vector3f(4.25f + 2.5f * party, 8.25f, 0.1f), Vector3f());
		party++;
	}
	else
	{
		ptr->SetPhysics(Vector3f(4.25f + 2.5f * enemies, 1.45f, 0), Vector3f());
		ptr2->SetPhysics(Vector3f(4.25f + 2.5f * enemies, 1.45f, 0.1f), Vector3f());
		enemies++;
	}
	ptr->Update();
	ptr2->SetColorAll(Vector3f(0.5f, 0.5f, 0.5f), 0.2f);
	ptr2->Update();

	// health 
	unsigned int font = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(font, 0.25f, 0.25f);
	FontManager::GetInstance().GetFont(font)->_letterSpacing = 0.55f;

	// name
	unsigned int namefont = FontManager::GetInstance().AddFont(true, false, true, "res/fonts/lowercase.png");
	FontManager::GetInstance().SetScale(namefont, 0.3f, 0.3f);
	FontManager::GetInstance().GetFont(namefont)->_letterSpacing = 0.75f;
	Vector3f offset = Vector3f(-1.45f, -0.25f, -1);
	FontManager::GetInstance().SetText(namefont, _(ap->_Name), ptr->GetPos() + offset, false);
	FontManager::GetInstance().GetFont(namefont)->Update(0);

	// Set everything
	ActorInfoBundle aib;
	aib._type = HC_HealthBar;
	aib._foreground = ptr;
	aib._background = ptr2;
	aib._health = font;
	aib._name = namefont;
	_actorInfo.push_back(aib);
}

void BattleHUD::Update()
{
	for (int i = 0; i < _actorInfo.size(); i++)
	{
		Vector3f color = Vector3f(0, 0.6f, 0.8f);
		int health = _actors.at(i)->_Fighter->Health;
		int maxhealth = _actors.at(i)->_Fighter->GetMaxHealth();

		// Set color
		if (health > maxhealth / 2)
			color = Vector3f(0.08f, 0.4f, 1.0f);
		else if (health <= maxhealth / 10)
			color = Vector3f(0.8f, 0, 0.4f);
		else
			color = Vector3f(1.0f, 0.6f, 0.2f);

		// Update the font
		if (health <= 0)
			FontManager::GetInstance().DisableFont(_actorInfo[i]._health);
		else
		{
			FontManager::GetInstance().EnableFont(_actorInfo[i]._health);
			Vector3f offset = Vector3f(-0.75f, -0.7f, -1);
			std::string text = std::to_string(health) + " / " + std::to_string(maxhealth);
			std::string current = FontManager::GetInstance().GetFont(_actorInfo[i]._health)->_text;

			// Only update font when it changes
			if (current.substr(0, current.length() - 1) != text)
				FontManager::GetInstance().SetText(_actorInfo[i]._health, text, _actorInfo[i]._foreground->GetPos() + offset, true);

			FontManager::GetInstance().GetFont(_actorInfo[i]._health)->Update(0);
		}

		_actorInfo[i]._foreground->SetColorAll(color, 0.9f);
		_actorInfo[i]._foreground->GetModelMat()->SetScale(fmax((float)health / (float)maxhealth, 0), 1, 1);
		_actorInfo[i]._foreground->Update();
		_actorInfo[i]._background->Update();
	}
}

void BattleHUD::SetRender()
{
	for (auto& x : _actorInfo)
	{
		Renderer::GetInstance().Add(x._foreground);
		Renderer::GetInstance().Add(x._background);
	}
}
