#ifndef DEFINE_GL_H__
#define DEFINE_GL_H__

#define IS_FULLSCREEN true

#define HD true

#ifdef HD
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1024
#else
#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 512
#endif

#define SHADOW_CUBE_SIZE 500
#define NUM_OF_LAYERS 6

#define ORTHO_BOTTOM -8
#define ORTHO_TOP 8
#define ORTHO_LEFT -15
#define ORTHO_RIGHT 15
#define ORTHO_NEAR 100
#define ORTHO_FAR -100

//#define RATIO (WINDOW_WIDTH / WINDOW_HEIGHT)
#define RATIO (32/16)

#endif