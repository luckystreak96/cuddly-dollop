#include "hudArrow.h"

#include "fontManager.h"
#include "renderer.h"
#include "actor.h"
#include "skill.h"

HudArrow::HudArrow(std::pair<Fighter_ptr, Actor_ptr> targeter, std::map<int, std::pair<Fighter_ptr, Actor_ptr>>* actors)
{
	_targeter = targeter;
	_targets = actors;

	// arrow
	_graphic = GraphComp_ptr(new GraphicsComponent("CENTERED_TILE_X", "res/sprites/special/arrow.png"));
	_graphic->SetColorAll(Vector3f(targeter.second->WaitPosition).Normalize(), 0.5f);
	_graphic->Update();

	bool value = targeter.first->Team != 0;
	_hidden = value;
	_showing = !value;

	SetPosition(_start, _end);

	Update();
}

void HudArrow::SetPosition(Vector3f start, Vector3f end)
{
	// Gotta point the center
	start += 0.5f;
	end += 0.5f;

	if (start == _start && end == _end)
		return;

	_start = start;
	_end = end;
	float distance = start.Distance2D(end);

	// Get the vector as a zero-pos vector to get the angle
	Vector3f vec = end - start;
	float angle = vec.AngleBetween(Vector3f(0, 1, 0));
	if (start.x - end.x < 0)
		angle = -angle;

	start.z = 0;

	_graphic->SetPhysics(start);
	_graphic->GetModelMat()->SetScale(1, distance, 1);
	_graphic->GetModelMat()->SetRotation(0, 0, angle);
	_graphic->Update();
}

void HudArrow::Destroy()
{
	HudComponent::Destroy();
}

void HudArrow::Update()
{
	if (_targeter.first && _targeter.first->PredictedSkill)
	{
		Vector3f start = _targeter.second->WaitPosition;
		Vector3f end = _targets->at(_targeter.first->GetTargets().at(0)).second->WaitPosition;
		SetPosition(start, end);
	}

	if (_showing)
	{
		if (_targeter.first->Dead)
			_hidden = true;
		else
			_hidden = false;
	}

	// Ensure that the value we're following actually changed to do something
	//if (m_prevValue == *m_observed)
	//	return;

	//// Handle updating text and health bar here
	//Vector3f color = Vector3f(0, 0.6f, 0.8f);
	//int value = *m_observed;

	//_foreground->SetColorAll(color, 0.9f);
	//_foreground->GetModelMat()->SetScale(fmax((float)value / (float)m_max, 0), 1, 1);
	//_foreground->Update();
}

void HudArrow::ToggleHidden(bool hidden)
{
	if (_targeter.first->Team == 0)
		hidden = !hidden;

	_showing = !hidden;

	if (_targeter.first->Dead)
	{
		_hidden = true;
		return;
	}

	_hidden = hidden;
}

void HudArrow::SetRender()
{
	// FontManager handles setting the render for the label

	if (!_hidden && _targeter.first->PredictedSkill)
		Renderer::GetInstance().Add(_graphic);
}
