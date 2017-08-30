#include "animation.h"

int Animation::m_progress = 0;

Animation::Animation()
{
	srand(time(NULL));
	m_delay = rand() % 20 + 160;
}

void Animation::AnimationCounter(float et)
{
	m_progress += (int)(et * 1000.0f);
}


bool Animation::SetTileModelTC(std::vector<Vertex>* verts, bool forceUpdate)
{
	int next = (m_progress % (m_delay * m_numFrames)) / m_delay;
	if (m_sprite == next && !forceUpdate)
		return false;

	m_sprite = next;

	float x_increment = 1.f / (float)m_width;
	float y_increment = 1.f / (float)m_height;
	float x = m_horizontal ? (float)(m_sprite) / (float)m_width : (float)m_animation * x_increment;
	float y = m_horizontal ? (float)m_animation * y_increment : (float)(m_sprite + 1) / (float)m_height;
	for (int i = 0; i < verts->size(); i++)
	{
		verts->at(i).tex.x = verts->at(i).tex.x == 0 ? x : x + x_increment;
		verts->at(i).tex.y = verts->at(i).tex.y == 0 ? y : y + y_increment;
	}
	return true;
}