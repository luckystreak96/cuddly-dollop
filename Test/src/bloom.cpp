#include "bloom.h"
#include "glfwBackend.h"
#include "effectManager.h"

Bloom::Bloom() : PostProcessingEffect()
{
}

void Bloom::Apply(Post_Processing_Screen* pps, FBO* fbo)
{
	fbo->unbindFrameBuffer();

	EffectManager::GetInstance().SetNoTranslateMode(true);

	//END FBO

	float size = OrthoProjInfo::GetRegularInstance().Size;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;

	pps->SetPhysics(Vector3f(right / size, top / size, 0), Vector3f());
	//pps->GetModelMat()->SetTranslation(0, 0, 0);
	pps->Update();

	//BEGIN BLOOM STAGE

	m_bloom.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	GLuint program = BloomEffect::GetInstance().GetNormal();
	EffectManager::GetInstance().Enable(E_Bloom, program);
	glBindTexture(GL_TEXTURE_2D, fbo->getColourTexture());
	pps->Draw(false);

	m_bloom.unbindFrameBuffer();

	//END BLOOM STAGE

	//BEGIN GAUSSIAN HORIZONTAL BLUR

	pps->GetModelMat()->SetScale(Vector3f((right * 2.f) / size / m_divisor, (top * 2.f) / size / m_divisor, 1));
	pps->SetPhysics(Vector3f(right / size / m_divisor, top / size / m_divisor, 0), Vector3f());
	pps->Update();

	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	EffectManager::GetInstance().Enable(E_Blur);
	//glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, m_bloom.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(true);
	// GLSL 1.1 requires to send texture size to the shader
	if (Effect::_efctGLVersion.x == 1)
	{
		int w, h;
		int miplevel = 0;
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &w);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &h);
		BlurEffect::GetInstance().SetTextureSize(w, h);
	}
	pps->Draw(false);

	m_gaussV.unbindFrameBuffer();

	//END GAUSSIAN HORIZONTAL BLUR

	//BEGIN GAUSSIAN VERTICAL BLUR

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps->Draw(false);

	m_gaussH.unbindFrameBuffer();

	//END GAUSSIAN VERTICAL BLUR

	//BEGIN SECOND GAUSSIAN BLUR (FOR MAXIMUM BLURINESS)

	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(true);
	pps->Draw(false);

	m_gaussV.unbindFrameBuffer();

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps->Draw(false);

	m_gaussH.unbindFrameBuffer();

	pps->GetModelMat()->SetScale(Vector3f((right * 2) / size, (top * 2) / size, 1));
	pps->SetPhysics(Vector3f(right / size, top / size, 0), Vector3f());
	pps->Update();

	//END SECOND GAUSSIAN BLUR

	bool test = true;

	if (!test)
	{
		//Combine blur with actual frame
		EffectManager::GetInstance().Enable(E_Combine);
		float intensity = 0.4f;
		CombineEffect::GetInstance().SetIntensity(intensity);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo->getColourTexture());
	}
	else
	{
		EffectManager::GetInstance().Enable(E_Basic);
		glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	}

	//Draw only base frame

	pps->Draw(false);

	EffectManager::GetInstance().SetNoTranslateMode(false);
}

void Bloom::ResetTextureSizes()
{
	m_gaussH.resetTextures(_width / (int)m_divisor, _height / (int)m_divisor);
	m_gaussV.resetTextures(_width / (int)m_divisor, _height / (int)m_divisor);
	m_bloom.resetTextures(_width, _height);
}