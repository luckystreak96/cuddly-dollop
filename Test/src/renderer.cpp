#include "renderer.h"

Renderer::Renderer() : m_toDraw(std::vector<GraphicsComponent*>())
{}

void Renderer::Add(GraphicsComponent* c) 
{ 
	m_toDraw.push_back(c);
	if (m_toDraw.size() > 6)
		int lol = 5;
}

void Renderer::Empty() 
{
	//m_toDraw.empty();
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

	if (m_toDraw.size() > 6)
		int lol = 5;
}

void Renderer::Draw()
{
	//Sort first
	Sort();

	//Draw everything
	for (auto x : m_toDraw)
	{
		if (x->GetMVBO() > 300)
			int lol = 5;
		x->Draw();
	}
}