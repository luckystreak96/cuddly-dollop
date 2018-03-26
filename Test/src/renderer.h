#ifndef RENDERER_H__
#define RENDERER_H__

//#include "GL/glew.h"
#include <vector>
#include <set>
#include <memory>
#include "graphicsComponent.h"
#include "FBO.h"
#include "post_processing_screen.h"
#include "postProcessingEffect.h"

class Renderer
{
public:
	static Renderer& GetInstance()
	{
		static Renderer instance;
		return instance;
	}
	// Setup FBO stuff
	void Setup();
	void ResetTextureSizes();


	// Draw everything and handle post-processing
	void Draw();
	void Add(GraphComp_ptr c);
	void Clear();
private:
	Renderer();
	void Sort();
	static inline bool PositionSort(Vector3f a, Vector3f b) { return a.z == b.z ? a.y > b.y : a.z > b.z; }
	static inline bool ComponentSort(GraphicsComponent* i, GraphicsComponent* j) { return PositionSort(i->GetPosRef(), j->GetPosRef()); }
private:
	std::vector<GraphicsComponent*> m_toDraw;
	std::vector<std::shared_ptr<PostProcessingEffect>> m_ppe;

	// Post processing variables
	int m_width;
	int m_height;
	std::shared_ptr<FBO> m_fbo;
	FBO m_bloom;
	FBO m_gaussV;
	FBO m_gaussH;
	float m_divisor = 2.0f;
	Post_Processing_Screen pps;
	bool apply;
};


#endif
