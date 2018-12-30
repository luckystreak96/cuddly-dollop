#include "lerper.h"

bool epsilon(float first, float second)
{
	return (abs(first - second) < 0.005f);
}

Lerper::Lerper()
{
	LowerVelocity64 = false;
	Amount = 0.025f;
	Acceleration = 0.002f;
	MinVelocity = 0;
	MaxVelocity = 1.0f;
}


Vector3f Lerper::LerpVelocity(Vector3f position, Vector3f target)
{
	if (LowerVelocity64)
		return (target / 64.0f - position / 64.0f) * Amount;
	else
		return (target - position) * Amount;
}

// Returns the next position with lerp smoothing
Vector3f Lerper::Lerp(Vector3f position, Vector3f target)
{
	// get the amount to move
	Vector3f v = LerpVelocity(position, target);
	// if its zero just return
	if (v == 0) return target;
	// store this value
	Vector3f vo = v;

	// don't allow increases in velocity beyond the specifed acceleration (ease in)
	// this also makes for smooth changes when switching direction
	//
	// only bother doing this if we're speeding up or changing direction
	// because the lerp takes care of the smoothing when slowing down
	//
	// note that multiplying two numbers together to check whether they are both
	// positive or negative is prone to overflow errors but as this class will 
	// realistically never be used for such massive numbers we should be OK! 

	// X-acceleration clamp
	if (v.x * previous_velocity.x < 0)
		if (v.x > 0)
			v.x = previous_velocity.x + Acceleration;
		else
			v.x = previous_velocity.x - Acceleration;
	if (abs(v.x - previous_velocity.x) > Acceleration)
		if (v.x - previous_velocity.x < 0)
			v.x = previous_velocity.x - Acceleration;
		else
			v.x = previous_velocity.x + Acceleration;

	// Y-acceleration clamp
	if (v.y * previous_velocity.y < 0)
		if (v.y > 0)
			v.y = previous_velocity.y + Acceleration;
		else
			v.y = previous_velocity.y - Acceleration;
	if (abs(v.y - previous_velocity.y) > Acceleration)
		if (v.y - previous_velocity.y < 0)
			v.y = previous_velocity.y - Acceleration;
		else
			v.y = previous_velocity.y + Acceleration;

	// If this is less than the minimum velocity then
	// clamp at minimum velocity
	if (abs(v.x) < MinVelocity)
	{
		v.x = (vo.x > 0) ? MinVelocity : 0 - MinVelocity;
	}
	// If this is more than the maximum velocity then
	// clamp at maximum velocity
	else if (abs(v.x) > MaxVelocity)
	{
		v.x = (vo.x > 0) ? MaxVelocity : 0 - MaxVelocity;
	}
	// If this is less than the minimum velocity then
	// clamp at minimum velocity
	if (abs(v.y) < MinVelocity)
	{
		v.y = (vo.y > 0) ? MinVelocity : 0 - MinVelocity;
	}
	// If this is more than the maximum velocity then
	// clamp at maximum velocity
	else if (abs(v.y) > MaxVelocity)
	{
		v.y = (vo.y > 0) ? MaxVelocity : 0 - MaxVelocity;
	}


	// Remember the previous velocity
	previous_velocity = v;

	// Adjust the position based on the new velocity
	position += v;

	// Overshoot logic doesn't work when you suddenly change direction

	// Now account for potential overshoot and clamp to target if necessary
	//if ((vo.x < 0 && position.x <= target.x) || (vo.x > 0 && position.x >= target.x))
	//	position.x = target.x;

	//if ((vo.y < 0 && position.y <= target.y) || (vo.y > 0 && position.y >= target.y))
	//	position.y = target.y;

	if (epsilon(position.x, target.x) && epsilon(position.y, target.y))
	{
		// Target has been reached!
	  //position.x = target.x;
	  //position.y = target.y;
	  //previous_velocity = 0;
	}
	return position;
}
