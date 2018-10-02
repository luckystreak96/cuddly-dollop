#include "bloom.h"
#include "glfwBackend.h"
#include "effectManager.h"

#include "gameData.h"

// The poor GPU can be saved by increasing m_divisor in postProcessing.cpp
// However, it makes some complicated business cuz the textures become too small, as is visible when you try it.

Bloom::Bloom() : PostProcessing()
{
	m_divisor = 4.0f;
}

Bloom::~Bloom()
{
}

void Bloom::Apply(Post_Processing_Screen* pps, FBO* fbo)
{
	//BEGIN BLOOM STAGE

	// Disable depth test because the framebuffers do weird stuff with depth
	glDisable(GL_DEPTH_TEST);

	m_bloom.BindFrameBuffer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLuint program = BloomEffect::GetInstance().GetNormal();
	EffectManager::GetInstance().Enable(E_Bloom, program);
	glBindTexture(GL_TEXTURE_2D, fbo->GetColourTexture());
	pps->Draw(false);

	ApplyBlur(pps, 2);

	//END BLOOM STAGE

	bool test = false;

	if (!test)
	{
		//Combine blur with actual frame
		EffectManager::GetInstance().Enable(E_Combine);
		float intensity = 0.25f;// 0.15f;
		CombineEffect::GetInstance().SetIntensity(intensity);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_gaussH.GetColourTexture());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo->GetColourTexture());
	}
	else
	{
		EffectManager::GetInstance().Enable(E_Basic);
		glBindTexture(GL_TEXTURE_2D, m_bloom.GetColourTexture());
	}

	//Draw only base frame
	fbo->BindFrameBuffer();
	pps->Draw(false);
	fbo->UnbindFrameBuffer();

	glEnable(GL_DEPTH_TEST);

	// for a cool effect
	//glBlendFunc(GL_ONE, GL_ONE);

	//EffectManager::GetInstance().SetNoTranslateMode(false);
}

void Bloom::ApplyBlur(Post_Processing_Screen* pps, int numTimes)
{
	// apply divisor because the bloom texture is reduced
	glViewport(0, 0, _width / m_divisor, _height / m_divisor);
	EffectManager::GetInstance().Enable(E_Blur);

	for (int i = 0; i < numTimes; i++)
	{
		m_gaussV.BindFrameBuffer();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, i == 0 ? m_bloom.GetColourTexture() : m_gaussH.GetColourTexture());
		// GLSL 1.1 requires to send texture size to the shader
		if (Effect::_efctGLVersion.x == 1)
		{
			int w, h;
			int miplevel = 0;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
			BlurEffect::GetInstance().SetTextureSize(w, h);
		}
		BlurEffect::GetInstance().SetHorizontalBlur(true);
		pps->Draw(false);

		//m_gaussV.UnbindFrameBuffer();

		m_gaussH.BindFrameBuffer();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, m_gaussV.GetColourTexture());
		// GLSL 1.1 requires to send texture size to the shader
		if (Effect::_efctGLVersion.x == 1)
		{
			int w, h;
			int miplevel = 0;
			glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
			BlurEffect::GetInstance().SetTextureSize(w, h);
		}
		BlurEffect::GetInstance().SetHorizontalBlur(false);
		pps->Draw(false);
	}

	glViewport(0, 0, _width, _height);
}


void Bloom::ResetTextureSizes()
{
	//int right = OrthoProjInfo::GetRegularInstance().Right;
	if (std::get<int>(GameData::Options.at("effect_quality")) == 0)
		m_divisor = 4.f;
	//m_divisor = OrthoProjInfo::GetRegularInstance().Size / 32;
	m_gaussH.ResetTextures(_width / (int)m_divisor, _height / (int)m_divisor);
	m_gaussV.ResetTextures(_width / (int)m_divisor, _height / (int)m_divisor);
	m_bloom.ResetTextures(_width, _height);
}