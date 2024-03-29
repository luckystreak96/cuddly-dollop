#include "playerGraphicsComponent.h"
#include "effectManager.h"
#include "bloom.h"

PlayerGraphicsComponent::PlayerGraphicsComponent(std::string tex, std::string model) : GraphicsComponent(model, tex), m_firstLoad(true)
{
	//_mModelsNoReplace = true;
	m_modelName = model;
	m_texture = tex;
	SetWidthHeight(tex);

	_outline = false;
	m_direction = dir_Down;
	m_dir_determined = false;

	Update();

	m_firstLoad = false;
}

void PlayerGraphicsComponent::NormalDraw(bool withTex)
{
	Effect::SetDrawType(DrawType::DT_ENTITY);
	GraphicsComponent::Draw(withTex);
	Effect::SetDrawType(DrawType::DT_FLAT);
}

void PlayerGraphicsComponent::DrawOutline(bool withTex)
{
	// Update the stencil buffer every time
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);// enable writing
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	// Draw to stencil buffer and draw to screen at same time
	Effect::SetDrawType(DrawType::DT_ENTITY);
	GraphicsComponent::Draw(withTex);

	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	EffectManager::GetInstance().Enable(E_SingleColor);
	//SingleColorEffect::GetInstance().Enable();
	Effect::SetDrawType(DrawType::DT_ENTITY);
	float xsize = m_modelMat.GetScale().x;
	Vector3f size = Vector3f(xsize * 1.1f, 1.075f, 1);
	m_modelMat.SetScale(size);
	//_mModelsNoReplace = true;

	//InsertMModels(m_modelMat, 0);
	UpdateTranslation();
	UpdateMModels();
	//GraphicsComponent::ResetVBO();
	GraphicsComponent::Draw(withTex);
	//InsertMModels(m_modelMat, 0);

	glEnable(GL_DEPTH_TEST);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	Vector3f scale = Vector3f(xsize, 1, 1);
	m_modelMat.SetScale(scale);
	UpdateMModels();

	Effect::SetDrawType(DrawType::DT_FLAT);
	EffectManager::GetInstance().EnablePrevious();
	Effect::SetDrawType(DrawType::DT_FLAT);
}

void PlayerGraphicsComponent::Draw(bool withTex)
{
	if (_outline)
	{
		DrawOutline(withTex);
		//NormalDraw(withTex);
	}
	else
	{
		NormalDraw(withTex);
	}
}

void PlayerGraphicsComponent::Update()
{
	m_modelMat.SetRotation(GetProjectionRotation(), 0, 0);
	//if (!Transformation::perspectiveOrtho)
	//	m_position.z -= MathUtils::HeightGivenLengthOfHypotenuseAndAngle(0.5f, -GetProjectionRotation());
	//std::cout << _persRotation << std::endl;
	// When you need to update a component according to position
	if (m_prevPosition != m_position && _observers.size())
		UpdateObservers();
	m_prevPosition = m_position;

	update_direction(m_velocity);

	//use the backup to reset the texture coordinates for proper analysis from the animator
	//m_vertices = std::vector<Vertex>(m_originalVertices);

	//Change the sprite depending on direction
	//m_animation = m_direction;
	if ((m_prev_dir != m_direction || (m_velocity == 0 && _animation > 3) || (m_velocity != 0 && _animation <= 3)) && !_specialAnimation && !_forceAnimation)
	{
		Anim_Enum animation;
		if (_default_animation != AE_Last)
			animation = _default_animation;
		else if (m_velocity == 0)
		{
			animation = (Anim_Enum)((int)m_direction);
		}
		else
			animation = GetMoveDirection((Anim_Enum)((int)m_direction));

		SetAnimation(animation, m_texture);
		m_force_update = true;
	}

	// If walking
	if (_animation >= AE_LeftMove && _animation <= AE_UpMove)
	{
		float speed = fmax(abs(m_velocity.x), abs(m_velocity.y)) + fmin(abs(m_velocity.x), abs(m_velocity.y)) / 2.0f;
		m_delay = speed == 0 ? 1 : m_actualDelay / (speed / 64.0f);
		if (_forceAnimation && m_delay == 1)
			m_delay = m_actualDelay / 4;
	}

	bool forceUpdate = (m_prev_dir != m_direction && !_specialAnimation) || m_firstLoad || m_force_update;
	SetTileModelTC(m_buffers.update_vertex_buffer(), forceUpdate);
		//ResetVBO();

	// Used so that the characters rotation doesnt mess up graphics
	m_position.z -= 0.25f;
	GraphicsComponent::Update();
	m_position.z += 0.25f;
	m_force_update = false;
	m_dir_determined = false;
}

void PlayerGraphicsComponent::SetAnimation(Anim_Enum anim, std::string spritesheet)
{
	Animation::SetAnimation(anim, spritesheet);
	m_actualDelay = m_delay;
}

//void PlayerGraphicsComponent::reset_to_default_animation()
//{
//	Anim_Enum result;
//	if(m_direction)
//	SetAnimation(result, get_texture());
//}

void PlayerGraphicsComponent::set_texture(std::string texture)
{
    GraphicsComponent::set_texture(texture);
	SetWidthHeight(texture);
}

void PlayerGraphicsComponent::update_direction(Vector3f vel)
{
	if (m_dir_determined)
		return;

	m_prev_dir = m_direction;
	//Change the direction hes facing
	if (abs(vel.x) > 20 || abs(vel.y) > 20)
	{
		// -0.1f here to prioritize left-right sprites when moving in diagonal
		if (abs(vel.x) >= abs(vel.y) - 0.1f)
		{
			if (vel.x > 0)
				m_direction = dir_Right;
			else
				m_direction = dir_Left;
		}
		else
		{
			if (vel.y > 0)
				m_direction = dir_Up;
			else
				m_direction = dir_Down;
		}
	}

	m_dir_determined = true;
}

Direction PlayerGraphicsComponent::GetDirection() { return m_direction; }
void PlayerGraphicsComponent::SetDirection(Direction dir) { m_direction = dir; }

void PlayerGraphicsComponent::SetDirection(GraphicsComponent* graph)
{
	float x = m_position.x;
	float y = m_position.y;
	float ox = graph->get_position().x;
	float oy = graph->get_position().y;

	if (abs(x - ox) > abs(y - oy))
	{
		//x is closer

		if (x - ox < 0)
			m_direction = dir_Right;
		else
			m_direction = dir_Left;

	}
	else
	{
		//y is closer

		//your under the other guy, turn up
		if (y - oy < 0)
			m_direction = dir_Up;
		else
			m_direction = dir_Down;
	}

	m_force_update = true;
	m_dir_determined = true;
}

