#include "bloom.h"
#include "glfwBackend.h"
#include "effectManager.h"

Bloom::Bloom() : m_width(1), m_height(1)
{
	pps.GetModelMat()->SetTranslation(0, 0, 0);
	pps.Update();
}

void Bloom::Begin()
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
}

void Bloom::End(bool dark)
{
	m_fbo.unbindFrameBuffer();

	EffectManager::GetInstance().SetNoTranslateMode(true);

	//END FBO

	float size = OrthoProjInfo::GetRegularInstance().Size;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;

	//BEGIN BLOOM STAGE

	m_bloom.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

	GLuint program = dark ? BloomEffect::GetInstance().GetDark() : BloomEffect::GetInstance().GetNormal();
	EffectManager::GetInstance().Enable(E_Bloom, program);
	pps.Draw(false);

	m_bloom.unbindFrameBuffer();

	//END BLOOM STAGE

	//BEGIN GAUSSIAN HORIZONTAL BLUR

	pps.GetModelMat()->SetScale(Vector3f(((right * 2.f) / size) / 4.f, ((top * 2.f) / size) / 4.f, 1));
	pps.Update();

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTextureEXT(GL_TEXTURE_2D, m_bloom.getColourTexture());
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

	m_gaussH.unbindFrameBuffer();

	//END GAUSSIAN HORIZONTAL BLUR

	//BEGIN GAUSSIAN VERTICAL BLUR

	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTextureEXT(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps.Draw(false);

	m_gaussV.unbindFrameBuffer();

	//END GAUSSIAN VERTICAL BLUR

	//BEGIN SECOND GAUSSIAN BLUR (FOR MAXIMUM BLURINESS)

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTextureEXT(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(true);
	pps.Draw(false);

	m_gaussH.unbindFrameBuffer();


	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTextureEXT(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps.Draw(false);

	m_gaussV.unbindFrameBuffer();

	pps.GetModelMat()->SetScale(Vector3f((right * 2) / size, (top * 2) / size, 1));
	pps.Update();

	//END SECOND GAUSSIAN BLUR

	bool test = false;

	if (!test)
	{
		//Combine blur with actual frame
		EffectManager::GetInstance().Enable(E_Combine);
		float intensity = dark ? 0.7f : 0.2f;
		CombineEffect::GetInstance().SetIntensity(intensity);
		glActiveTexture(GL_TEXTURE1);
		glBindTextureEXT(GL_TEXTURE_2D, m_gaussV.getColourTexture());
		glActiveTexture(GL_TEXTURE0);
		glBindTextureEXT(GL_TEXTURE_2D, m_fbo.getColourTexture());
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

void Bloom::ResetTextureSizes()
{
	m_fbo.resetTextures(m_width, m_height);
	m_gaussH.resetTextures(m_width / (int)m_divisor, m_height / (int)m_divisor);
	m_gaussV.resetTextures(m_width / (int)m_divisor, m_height / (int)m_divisor);
	m_bloom.resetTextures(m_width, m_height);
}