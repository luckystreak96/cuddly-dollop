#include "menuParty.h"

#include "renderer.h"
#include "menuBackgroundBox.h"

MenuParty::MenuParty()
{
	//float right = OrthoProjInfo::GetRegularInstance().Right;
	//float top = OrthoProjInfo::GetRegularInstance().Top;
	//float size = OrthoProjInfo::GetRegularInstance().Size;

	//GraphComp_ptr topBG = GraphComp_ptr(new FontGraphicsComponent("TILE", "res/sprites/special/dialogue.png"));
	//dynamic_cast<FontGraphicsComponent*>(topBG.get())->SetStatic(true);
	//topBG->SetPhysics(Vector3f(right * 0.2f / size, top * 0.1f / size, 0.5f));
	//topBG->GetModelMat()->SetScale(right * 1.6f / size, top * 1.8f / size, 1);
	//topBG->SetColorAll(Vector3f(1, 1, 0.5f), 1.1f);
	//topBG->Update();

	//m_backgrounds.push_back(topBG);
	m_backgrounds.push_back(std::shared_ptr<MenuBackgroundBox>(new MenuBackgroundBox()));
}

void MenuParty::Update()
{
}

void MenuParty::Destroy()
{
	MenuComponent::Destroy();
}

void MenuParty::SetRender()
{
	for (auto& x : m_backgrounds)
		Renderer::GetInstance().Add(x);
}
