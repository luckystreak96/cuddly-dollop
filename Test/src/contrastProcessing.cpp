#include "contrastProcessing.h"
#include "glfwBackend.h"
#include "effectManager.h"

ContrastProcessing::ContrastProcessing() : PostProcessing()
{
}

void ContrastProcessing::Apply(Post_Processing_Screen* pps, FBO* fbo)
{
	//BEGIN CONTRAST STAGE

	m_contrast.BindFrameBuffer();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	EffectManager::GetInstance().Enable(E_Contrast);
	glBindTexture(GL_TEXTURE_2D, fbo->GetColourTexture());
	pps->Draw(false);

	m_contrast.UnbindFrameBuffer();

	//END CONTRAST STAGE

	// SET IT TO THE FRAMEBUFFER
	fbo->BindFrameBuffer();

	// Bind tex here or dont unbind it in fbo.unbind
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	EffectManager::GetInstance().Enable(E_Basic);
	glBindTexture(GL_TEXTURE_2D, m_contrast.GetColourTexture());
	pps->Draw(false);

	fbo->UnbindFrameBuffer();
}

void ContrastProcessing::ResetTextureSizes()
{
	m_contrast.ResetTextures(_width, _height);
}