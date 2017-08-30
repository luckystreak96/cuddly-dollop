#include "bloom.h"

Bloom::Bloom() : m_width(1), m_height(1)
{
	CombineEffect::GetInstance().SetIntensity(0.9f);
	pps.LoadGLResources();
}

void Bloom::Begin()
{
	int current_window_width = glutGet(GLUT_WINDOW_WIDTH);
	int current_window_height = glutGet(GLUT_WINDOW_HEIGHT);

	//If the window size changes, the fbo texture sizes need to adjust
	if (m_width != current_window_width || m_height != current_window_height)
	{
		m_width = current_window_width;
		m_height = current_window_height;
		ResetTextureSizes();
	}

	//Setup the tile to draw
	pps.Update();
	pps.GetModelMat()->SetTranslation(0, 0, 0);

	//FBO
	m_fbo.bindFrameBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Bloom::End()
{
	m_fbo.unbindFrameBuffer();

	//END FBO

	//BEGIN BLOOM STAGE

	m_bloom.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

	BloomEffect::GetInstance().Enable();
	BloomEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);
	pps.DrawNoTexture();

	m_bloom.unbindFrameBuffer();

	//END BLOOM STAGE

	//BEGIN GAUSSIAN HORIZONTAL BLUR

	pps.GetModelMat()->SetScale(Vector3f(1.0f / m_divisor, 1.0f / m_divisor, 1));

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_bloom.getColourTexture());
	BlurEffect::GetInstance().Enable();
	BlurEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);
	pps.DrawNoTexture();

	m_gaussH.unbindFrameBuffer();

	//END GAUSSIAN HORIZONTAL BLUR

	//BEGIN GAUSSIAN VERTICAL BLUR

	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps.DrawNoTexture();

	m_gaussV.unbindFrameBuffer();

	//END GAUSSIAN VERTICAL BLUR

	//BEGIN SECOND GAUSSIAN BLUR (FOR MAXIMUM BLURINESS)

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(true);
	pps.DrawNoTexture();

	m_gaussH.unbindFrameBuffer();


	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps.DrawNoTexture();

	m_gaussV.unbindFrameBuffer();

	pps.GetModelMat()->SetScale(Vector3f(1.0f, 1.0f, 1));

	//END SECOND GAUSSIAN BLUR
	
	//Combine blur with actual frame
	CombineEffect::GetInstance().Enable();
	CombineEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

	pps.DrawNoTexture();
}

void Bloom::ResetTextureSizes()
{
	m_fbo.resetTextures(m_width, m_height);
	m_gaussH.resetTextures(m_width / m_divisor, m_height / m_divisor);
	m_gaussV.resetTextures(m_width / m_divisor, m_height / m_divisor);
	m_bloom.resetTextures(m_width, m_height);
}