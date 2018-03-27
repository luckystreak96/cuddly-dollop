#include "contrastProcessing.h"
#include "glfwBackend.h"
#include "effectManager.h"

ContrastProcessing::ContrastProcessing() : PostProcessing()
{
}

void ContrastProcessing::Apply(Post_Processing_Screen* pps, FBO* fbo)
{
	EffectManager::GetInstance().SetNoTranslateMode(true);

	float size = OrthoProjInfo::GetRegularInstance().Size;
	float right = OrthoProjInfo::GetRegularInstance().Right;
	float top = OrthoProjInfo::GetRegularInstance().Top;

	pps->SetPhysics(Vector3f(right / size, top / size, 0), Vector3f());
	pps->GetModelMat()->SetScale(Vector3f((right * 2.f) / size, (top * 2.f) / size, 1));
	pps->Update();

	//BEGIN BLOOM STAGE

	m_contrast.BindFrameBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	EffectManager::GetInstance().Enable(E_Contrast);
	glBindTexture(GL_TEXTURE_2D, fbo->GetColourTexture());
	pps->Draw(false);

	m_contrast.UnbindFrameBuffer();

	//END BLOOM STAGE

	fbo->BindFrameBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	EffectManager::GetInstance().Enable(E_Basic);
	glBindTexture(GL_TEXTURE_2D, m_contrast.GetColourTexture());
	pps->Draw(false);

	fbo->UnbindFrameBuffer();


	bool test = false;

	if (!test)
	{
		EffectManager::GetInstance().Enable(E_Basic);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbo->GetColourTexture());
	}
	else
	{
		EffectManager::GetInstance().Enable(E_Basic);
		glBindTexture(GL_TEXTURE_2D, m_contrast.GetColourTexture());
	}

	//Draw only base frame
	pps->Draw(false);

	EffectManager::GetInstance().SetNoTranslateMode(false);
}

void ContrastProcessing::ResetTextureSizes()
{
	m_contrast.ResetTextures(_width, _height);
}