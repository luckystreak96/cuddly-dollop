#include "renderer.h"

#include "glfwBackend.h"
#include "effectManager.h"
#include "blurEffect.h"
#include "combineEffect.h"
#include "bloomEffect.h"

#include "input_manager.h"

// MAKE IT BE A CENTERED_TILE AND PLACE IT CORRECTLY OR ITLL BE DUMB AF
Renderer::Renderer() : m_toDraw(std::vector<GraphicsComponent*>()), m_width(1), m_height(1), m_divisor(4.0f), apply(false)
{
	pps.GetModelMat()->SetTranslation(0, 0, 0);
	pps.Update();
}

void Renderer::Setup()
{
	if (InputManager::GetInstance().FrameKeyStatus(A_Cancel, KeyStatus::KeyPressed))
		apply = !apply;

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
	}

	//Setup the tile to draw
	float size = OrthoProjInfo::GetRegularInstance().Size;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;
	pps.GetModelMat()->SetScale(Vector3f((right * 2) / size, (top * 2) / size, 1));
	pps.Update();

	//FBO
	m_fbo.bindFrameBuffer();
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glStencilMask(0x00);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	//EffectManager::GetInstance().SetNoTranslateMode(true);
}

void Renderer::ResetTextureSizes()
{
	m_fbo.resetTextures(m_width, m_height);
	m_gaussH.resetTextures(m_width / m_divisor, m_height / m_divisor);
	m_gaussV.resetTextures(m_width / m_divisor, m_height / m_divisor);
	m_bloom.resetTextures(m_width, m_height);
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

	if (apply)
	{

	m_fbo.unbindFrameBuffer();

	EffectManager::GetInstance().SetNoTranslateMode(true);

	//END FBO

	float size = OrthoProjInfo::GetRegularInstance().Size;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;

	pps.SetPhysics(Vector3f(right / size, top / size, 0), Vector3f());
	//pps.GetModelMat()->SetTranslation(0, 0, 0);
	pps.Update();

	//BEGIN BLOOM STAGE

	m_bloom.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

	GLuint program = BloomEffect::GetInstance().GetNormal();
	EffectManager::GetInstance().Enable(E_Bloom, program);
	pps.Draw(false);

	m_bloom.unbindFrameBuffer();

	//END BLOOM STAGE

	//BEGIN GAUSSIAN HORIZONTAL BLUR

	pps.GetModelMat()->SetScale(Vector3f((right * 2.f) / size / m_divisor, (top * 2.f) / size / m_divisor, 1));
	pps.SetPhysics(Vector3f(right / size / m_divisor, top / size / m_divisor, 0), Vector3f());
	pps.Update();

	m_fbo.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_bloom.getColourTexture());
	EffectManager::GetInstance().Enable(E_Blur);
	// GLSL 1.1 requires to send texture size to the shader
	if (Effect::_efctGLVersion.x == 1)
	{
		int w, h;
		int miplevel = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
		BlurEffect::GetInstance().SetTextureSize(w, h);
	}
	pps.Draw(false);

	m_fbo.unbindFrameBuffer();

	//END GAUSSIAN HORIZONTAL BLUR

	//BEGIN GAUSSIAN VERTICAL BLUR

	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps.Draw(false);

	m_gaussV.unbindFrameBuffer();

	//END GAUSSIAN VERTICAL BLUR

	//BEGIN SECOND GAUSSIAN BLUR (FOR MAXIMUM BLURINESS)

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(true);
	pps.Draw(false);

	m_gaussH.unbindFrameBuffer();


	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps.Draw(false);

	m_gaussV.unbindFrameBuffer();

	pps.GetModelMat()->SetScale(Vector3f((right * 2) / size, (top * 2) / size, 1));
	pps.SetPhysics(Vector3f(right / size, top / size, 0), Vector3f());
	pps.Update();

	//END SECOND GAUSSIAN BLUR

	bool test = true;

	if (!test)
	{
		//Combine blur with actual frame
		EffectManager::GetInstance().Enable(E_Combine);
		float intensity = 0.2f;
		CombineEffect::GetInstance().SetIntensity(intensity);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());
	}
	else
	{
		EffectManager::GetInstance().Enable(E_Basic);
		glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());
	}

	//Draw only base frame

	pps.Draw(false);

	EffectManager::GetInstance().SetNoTranslateMode(false);
	}
}