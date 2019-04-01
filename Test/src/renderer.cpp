#include "renderer.h"

#include "glfwBackend.h"
#include "effectManager.h"
#include "blurEffect.h"
#include "combineEffect.h"
#include "bloomEffect.h"
#include "bloom.h"
#include "nighttimeProcessing.h"
#include "contrastProcessing.h"

#include "input_manager.h"

// MAKE IT BE A CENTERED_TILE AND PLACE IT CORRECTLY OR ITLL BE DUMB AF
Renderer::Renderer() : m_toDraw(std::vector<GraphicsComponent*>()), m_width(1), m_height(1), apply(true), m_bloom_enabled(true), m_force_update(true)
{
	//Setup the tile to draw
	float size = OrthoProjInfo::GetRegularInstance().Size;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;

	pps.set_position(Vector3f(right, top, 0));
	pps.GetModelMat()->SetScale(Vector3f((right * 2) / size, (top * 2) / size, 1));
	pps.Update();

	//pps.GetModelMat()->SetTranslation(0, 0, 0);
	//pps.Update();

	// TEST ERASE ME MAYBE
	//m_ppe.push_back(std::make_shared<NightTimeProcessing>(NightTimeProcessing()));
	//m_ppe.push_back(std::make_shared<ContrastProcessing>(ContrastProcessing()));
	update_post_processing();
}

void Renderer::SwapBuffers(GLFWwindow* window)
{
	//if (m_swapThread.joinable())
	//	m_swapThread.join();
	//m_swapThread = std::thread(glfwSwapBuffers, window);
	glfwSwapBuffers(window);
}


void Renderer::Setup()
{
	//if (m_swapThread.joinable())
	//	m_swapThread.join();
	//if (InputManager::GetInstance().FrameKeyStatus(A_Cancel, KeyStatus::KeyPressed))
	//	apply = !apply;

	glStencilMask(0xFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);
	if (apply)
	{
		int w, h;
		glfwGetWindowSize(GLFWManager::m_window, &w, &h);
		int current_window_width = w;
		int current_window_height = h;

		//If the window size changes, the fbo texture sizes need to adjust
		if (m_width != current_window_width || m_height != current_window_height || m_force_update)
		{
			m_force_update = false;

			m_width = current_window_width;
			m_height = current_window_height;
			ResetTextureSizes();
			for (auto& x : m_ppe)
			{
				x->_width = m_width;
				x->_height = m_height;
				x->ResetTextureSizes();
			}
		}

		//FBO
		m_fbo.BindFrameBuffer();
		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);
	}

}

void Renderer::ResetTextureSizes()
{
	m_fbo.ResetTextures(m_width, m_height);
}

void Renderer::Add(std::shared_ptr<GraphicsComponent> c)
{
	if (c->will_not_draw())
		return;
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

	//if (ElapsedTime::GetInstance().OneSecond())
	//	std::cout << "Drawing " << m_toDraw.size() << " GraphicsComponents" << std::endl;
}

void Renderer::Draw()
{
	//Sort first
	Sort();

	//if (!apply)
	//	EffectManager::GetInstance().Enable(E_Gamma);
	EffectManager::GetInstance().Enable(E_Basic);

	//Draw everything
	for (auto x : m_toDraw)
	{
		x->Draw();
	}

	//if (!apply)
	//	return;

	m_fbo.UnbindFrameBuffer();

	EffectManager::GetInstance().SetNoTranslateMode(true);

	//END FBO

	// SEND THE IMAGE THROUGH THE PPE, AND THEY SHOULD ALL PUT THEIR RESULT IN M_FBO
	for (auto& x : m_ppe)
		x->Apply(&pps, &m_fbo);

	EffectManager::GetInstance().Enable(E_Gamma);
	glBindTexture(GL_TEXTURE_2D, m_fbo.GetColourTexture());
	pps.Draw(false);
	EffectManager::GetInstance().Enable(E_Basic);
	glBindTexture(GL_TEXTURE_2D, m_fbo.GetColourTexture());
	EffectManager::GetInstance().SetNoTranslateMode(false);
}

void Renderer::toggle_bloom() {
	m_bloom_enabled = !m_bloom_enabled;
	update_post_processing();
}

void Renderer::update_post_processing() {
	if(m_bloom_enabled)
	{
//		if (!post_processing_present<Bloom>())
//			m_ppe.push_back(std::shared_ptr<Bloom>(new Bloom()));
	}
	else
	{
	    remove_post_processing<Bloom>();
	}

	m_force_update = true;
}

bool Renderer::get_bloom_state() {
    return m_bloom_enabled;
}

void Renderer::init() {
	Camera cam;

	OrthoProjInfo* o = &OrthoProjInfo::GetRegularInstance();
	cam._transform->SetOrthoProj(o);

	EffectManager::GetInstance().SetAllTilePositions(OrthoProjInfo::GetRegularInstance().Size);

	auto m_backupTrans = cam._transform->GetTranslation();

	Vector3f temp = m_backupTrans;

	// Floor so we dont move the camera half a pixel and fuck up the graphics
	temp.x = floorf(temp.x);
	temp.y = floorf(temp.y);
	cam._transform->SetTranslation(temp);

	EffectManager::GetInstance().SetWorldTrans(&cam._transform->GetWOTransCentered().m[0][0], &cam._transform->GetWOTransNoTranslate().m[0][0]);
    EffectManager::GetInstance().ResetWorldUpdateFlag();

	EffectManager::GetInstance().Enable(E_Basic);

	Renderer::GetInstance().Setup();
}
