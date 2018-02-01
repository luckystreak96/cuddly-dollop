#include "playerGraphicsComponent.h"
#include "effectManager.h"
#include "bloom.h"

PlayerGraphicsComponent::PlayerGraphicsComponent(std::string tex, std::string model) : GraphicsComponent(model, tex), m_firstLoad(true)
{
	m_modelName = model;
	m_texture = tex;
	SetWidthHeight(tex);
	Construct();
	Update();
	m_firstLoad = false;
	m_outline = false;
}

void PlayerGraphicsComponent::NormalDraw(bool withTex)
{
	Effect::SetDrawType(DrawType::DT_ENTITY);
	GraphicsComponent::Draw(withTex);
	Effect::SetDrawType(DrawType::DT_FLAT);
}

void PlayerGraphicsComponent::DrawOutline(bool withTex)
{
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	Effect::SetDrawType(DrawType::DT_ENTITY);
	GraphicsComponent::Draw(withTex);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	EffectManager::GetInstance().Enable(E_SingleColor);
	//SingleColorEffect::GetInstance().Enable();
	Effect::SetDrawType(DrawType::DT_ENTITY);
	Vector3f size = Vector3f(1.1f, 1.1f, 1);
	m_modelMat.SetScale(size);
	UpdateTranslation();

	//Bloom::GetInstance().Begin();

	GraphicsComponent::UpdateMModels();
	GraphicsComponent::Draw(withTex);
	
	//Bloom::GetInstance().End(false);

	glEnable(GL_DEPTH_TEST);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	Vector3f scale = Vector3f(1, 1, 1);
	m_modelMat.SetScale(scale);

	Effect::SetDrawType(DrawType::DT_FLAT);
	EffectManager::GetInstance().EnablePrevious();
	Effect::SetDrawType(DrawType::DT_FLAT);
}

void PlayerGraphicsComponent::Draw(bool withTex)
{
	if (m_outline)
	{
		DrawOutline(withTex);
	}
	else
	{
		NormalDraw(withTex);
	}
}


void PlayerGraphicsComponent::Update()
{
	int dir = m_direction;
	//Change the direction hes facing
	if (m_vel != 0)
	{
		if (abs(m_vel.x) > abs(m_vel.y))
		{
			if (m_vel.x > 0)
				m_direction = dir_Right;
			else
				m_direction = dir_Left;
		}
		else
		{
			if (m_vel.y > 0)
				m_direction = dir_Up;
			else
				m_direction = dir_Down;
		}
	}

	//use the backup to reset the texture coordinates for proper analysis from the animator
	//m_vertices = std::vector<Vertex>(m_originalVertices);

	//Change the sprite depending on direction
	//m_animation = m_direction;
	if (dir != m_direction && !_specialAnimation)
		SetAnimation((Anim_Enum)((int)m_direction), m_texture);

	bool forceUpdate = (dir != m_direction && !_specialAnimation) || (m_direction != m_lastInteraction) || m_firstLoad;
	if (SetTileModelTC(&m_vertices, forceUpdate))
		ResetVBO();

	GraphicsComponent::Update();
}

void PlayerGraphicsComponent::SetTexture(std::string texture)
{
	GraphicsComponent::SetTexture(texture);
	SetWidthHeight(texture);
}
