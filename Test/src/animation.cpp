#include "animation.h"

int Animation::m_progress = 0;

void Animation::SetTileModelTC(std::vector<Vertex>* verts)
{
	m_sprite = (m_progress % (m_delay * m_numFrames)) / m_delay;
	float x_increment = 1.f / (float)m_width;
	float y_increment = 1.f / (float)m_height;
	float x = m_horizontal ? (float)(m_sprite) / (float)m_width : (float)m_animation * x_increment;
	float y = m_horizontal ? (float)m_animation * y_increment : (float)(m_sprite + 1) / (float)m_height;
	for(int i = 0; i < verts->size(); i++)
	{
		verts->at(i).tex.x = verts->at(i).tex.x == 0 ? x : x + x_increment;
		verts->at(i).tex.y = verts->at(i).tex.y == 0 ? y : y + y_increment;
	}
}
