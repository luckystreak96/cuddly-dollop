#include "renderer.h"

Renderer::Renderer() : m_toDraw(std::vector<GraphicsComponent*>())
{}

void Renderer::Add(std::shared_ptr<GraphicsComponent> c)
{ 
	m_toDraw.push_back(c.get());
}

void Renderer::Clear() 
{
	m_toDraw.clear();
}


void Renderer::Sort()
{
	//Remove nulls
	m_toDraw.erase(std::remove(m_toDraw.begin(), m_toDraw.end(), (void*)NULL), m_toDraw.end());

	//Erase duplicates
	std::sort(m_toDraw.begin(), m_toDraw.end());
	m_toDraw.erase(std::unique(m_toDraw.begin(), m_toDraw.end()), m_toDraw.end());

	//Sort the components
	std::sort(m_toDraw.begin(), m_toDraw.end(), ComponentSort);
}

void Renderer::Draw()
{
	//Sort first
	Sort();

	//Draw everything
	for (auto x : m_toDraw)
		x->Draw();
}