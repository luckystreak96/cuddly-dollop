#ifndef ANIMATION_H__
#define ANIMATION_H__

#include <vector>
#include "vertex.h"
#include <time.h>

class Animation
{
public:
	Animation();
	static void AnimationCounter(float et);
	bool SetTileModelTC(std::vector<Vertex>* verts, bool forceUpdate = false);
protected:
	//ElapsedTime progress
	static int m_progress;
protected:
	//Number of millis between frames
	int m_delay = 160;
	//Number of desired frames
	int m_numFrames = 2;
	//Whether the sprites go down or right
	bool m_horizontal = true;
	//which row/column to start in
	int m_animation = 0;
	//Which frame we're at
	int m_sprite = 0;
	//The total image size
	int m_width = 2;
	int m_height = 4;
};

#endif