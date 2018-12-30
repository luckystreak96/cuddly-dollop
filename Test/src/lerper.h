#ifndef LERPER_H__
#define LERPER_H__

#include "vector3f.h"

class Lerper
{
public:
	Lerper();
	Vector3f Lerp(Vector3f position, Vector3f target);
private:
	// Returns the amount of movement at this staget of the lerp
	Vector3f LerpVelocity(Vector3f position, Vector3f target);
public:
	// The Lerp amount - the larger this is the faster the object will move
	// Don't set it > 0 and probably start with values in the range 0.01->0.1 
	float Amount;
	// The minimum velocity the object will mover per frame. Defaults to zero.
	// 1 is usually a good choice for 1:1 onscreen movement
	float MinVelocity;
	// The maximum velocity the object will move per frame
	float MaxVelocity;
	// The maximum amount by which the object can accelerate per frame (or
	// deccelerate when changing direction)
	float Acceleration;

	bool LowerVelocity64;
private:
	Vector3f previous_velocity;
};

#endif // !LERPER_H__
