#include "post_processing_screen.h"

Post_Processing_Screen::Post_Processing_Screen(std::string modelName, std::string texPath, Vector3f bbsize, Vector3f centerSize) : GraphicsComponent(modelName, texPath)
{
	m_position = Vector3f();
//	m_size = bbsize;
//	m_normalSize = centerSize;
	m_texture_loaded = true;
}