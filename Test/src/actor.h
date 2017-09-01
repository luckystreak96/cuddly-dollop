#ifndef ACTOR_H__
#define ACTOR_H__

#include "vector3f.h"
#include "drawable.h"
#include "define.h"
#include "animation.h"
#include "blurEffect.h"
#include <string>
#include <gl/freeglut.h>

class Actor : public Drawable, public Animation
{
public:
	Actor(Vector3f Position = Vector3f(0, 0, -5), std::string modelName = "NONE", std::string texPath = "res/whitebunny.png");

	void Update();
	void Draw();
	void Move(bool up, bool down, bool left, bool right);
	void Jump(bool released);
	void DesiredMove();
	void ApplyGravity();
	void SetProperties();
	Transformation GetMlMatrix();
	void DrawShadowMap(Transformation & p);
	Direction GetDirection() { return m_direction; }
	inline float MoveSpeed() { return m_speed / 8.f; }
	inline float SlowSpeed() { return m_speed / 16.f; }
private:
	Direction m_direction = dir_Down;
	Vector3f m_playerSize;
	float m_speed;
	float m_RotationX = 0;
	float m_RotationY = 0;
};

#endif