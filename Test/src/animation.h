#ifndef ANIMATION_H__
#define ANIMATION_H__

#include <vector>
#include "vertex.h"

class Animation
{
public:
	static void AnimationCounter(float et) { m_progress += et * 100; }
	void SetTileModelTC(std::vector<Vertex>* verts);
protected:
	//ElapsedTime progress
	static int m_progress;
protected:
	int m_delay = 10;
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
	int m_height = 1;
};

#endif