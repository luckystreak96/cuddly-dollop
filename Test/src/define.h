#ifndef DEFINE_H__
#define DEFINE_H__

#define DEBUG 1

#define SKIP_FRAMES 1
#define EPSILON 0.00390625f
#define COLLISION_BUFFER_SPACE 0.1f
#define BIG_EPSILON 0.018f
#define STEP_HEIGHT 0.5f
#define STAND_HEIGHT 1.0f

//AABB -- Axis-Aligned Bounding Box
enum AABB {Left = 0, Right = 1, Down = 2, Up = 3, Close = 4, Far = 5};
enum Report_Type {Collision = 0};
enum Ent_Props {P_Slope_High_N, P_Slope_High_E, P_Slope_High_S, P_Slope_High_W, P_Player};

namespace Globals
{
	extern bool DEBUG_DRAW_NORMALS;
	extern bool DEBUG_DRAW_TILE_OUTLINES;
}

#endif
