#ifndef RENDERER_H__
#define RENDERER_H__

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "graphicsComponent.h"
#include "FBO.h"
#include "post_processing_screen.h"
#include "postProcessing.h"

#include <vector>
#include <set>
#include <memory>
#include <thread>

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
	void SwapBuffers(GLFWwindow* window);
	inline unsigned int Count() { return m_toDraw.size(); }

	template<typename PP>
	bool post_processing_present();
	template<typename PP>
	bool remove_post_processing();

	void toggle_bloom();
	void update_post_processing();

	bool get_bloom_state();
private:
	Renderer();
	void Sort();
	static inline bool PositionSort(Vector3f a, Vector3f b) { return a.z == b.z ? a.y > b.y : a.z > b.z; }
	static inline bool ComponentSort(GraphicsComponent* i, GraphicsComponent* j) { return PositionSort(i->GetPosRef(), j->GetPosRef()); }
private:
	std::vector<GraphicsComponent*> m_toDraw;
	std::vector<std::shared_ptr<PostProcessing>> m_ppe;

	std::thread m_swapThread;

	// Post processing variables
	int m_width;
	int m_height;
	bool m_force_update;
	FBO m_fbo;
	Post_Processing_Screen pps;
	bool m_bloom_enabled;
	bool apply;
};

template<typename PP>
bool Renderer::post_processing_present()
{
	bool present = false;
	for(auto x : m_ppe)
		if (dynamic_cast<PP*>(x.get()) != nullptr)
			present = true;

	return present;
}

template<typename PP>
bool Renderer::remove_post_processing(){
	int pos = -1;
    for(int i = 0; i < m_ppe.size(); i++)
	{
		if (dynamic_cast<PP*>(m_ppe[i].get()) != nullptr)
		{
			pos = i;
			break;
		}
	}

    if(pos != -1)
        m_ppe.erase(m_ppe.begin() + pos);
    else
		return false;

    return true;
}

#endif
