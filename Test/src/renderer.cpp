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
Renderer::Renderer() : m_toDraw(std::vector<GraphicsComponent*>()), m_width(1), m_height(1), apply(true)
{
	pps.GetModelMat()->SetTranslation(0, 0, 0);
	pps.Update();

	// TEST ERASE ME MAYBE
	//m_ppe.push_back(std::make_shared<NightTimeProcessing>(NightTimeProcessing()));
	m_ppe.push_back(std::shared_ptr<Bloom>(new Bloom()));
	//m_ppe.push_back(std::make_shared<ContrastProcessing>(ContrastProcessing()));
}

void Renderer::Setup()
{
	if (false && InputManager::GetInstance().FrameKeyStatus(A_Cancel, KeyStatus::KeyPressed))
		apply = !apply;

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
		if (m_width != current_window_width || m_height != current_window_height)
		{
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

		//Setup the tile to draw
		float size = OrthoProjInfo::GetRegularInstance().Size;
		float right = OrthoProjInfo::GetRegularInstance().Right;
		float top = OrthoProjInfo::GetRegularInstance().Top;

		pps.SetPhysics(Vector3f(right / size, top / size, 0), Vector3f());
		pps.GetModelMat()->SetScale(Vector3f((right * 2) / size, (top * 2) / size, 1));
		pps.Update();

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

void Renderer::Add(GraphComp_ptr c)
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

	if (!apply)
		return;

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