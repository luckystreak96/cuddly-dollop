#ifndef DEFINE_H__
#define DEFINE_H__

//#define HD
#define IS_FULLSCREEN true

#ifdef HD
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#else
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 512
#endif

#define SHADOW_CUBE_SIZE 500
#define NUM_OF_LAYERS 6


#define DEBUG 1

#define SKIP_FRAMES 1
#define EPSILON 0.00390625f
#define COLLISION_BUFFER_SPACE 0.1f
#define BIG_EPSILON 0.018f

//AABB -- Axis-Aligned Bounding Box
enum AABB {Left = 0, Right = 1, Down = 2, Up = 3, Close = 4, Far = 5};
enum Report_Type {Collision = 0};
enum Ent_Props {P_Slope_High_N, P_Slope_High_E, P_Slope_High_S, P_Slope_High_W, P_Player};

namespace Globals
{
	extern bool DEBUG_DRAW_NORMALS;
}

#endif
