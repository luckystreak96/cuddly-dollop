#ifndef RENDERER_H__
#define RENDERER_H__

//#include <GL/glew.h>
#include <vector>
#include <set>
#include "graphicsComponent.h"

class Renderer
{
public:
	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}
	void Add(GraphicsComponent* c) { m_toDraw.push_back(c); }
	void Empty() { m_toDraw.empty(); }
	void Draw();
private:
	Renderer();
	void Sort();
	static inline bool ComponentSort(GraphicsComponent* i, GraphicsComponent* j) { return i->GetZ() > j->GetZ(); }
private:
	std::vector<GraphicsComponent*> m_toDraw;
};


#endif
