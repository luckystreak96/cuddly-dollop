#include "heightFog.h"
#include "glfwBackend.h"

HeightFog::HeightFog() : m_width(1), m_height(1)
{
	CombineEffect::GetInstance().SetIntensity(0.9f);
	pps.GetModelMat()->SetTranslation(0, 0, 0);
	pps.Update();
	//pps.LoadGLResources();
}

void HeightFog::Begin()
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

	//FBO
	m_fbo.bindFrameBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void HeightFog::End(Vector3f playerPos)
{
	m_fbo.unbindFrameBuffer();

	//END FBO

	//BEGIN FOG STAGE

	//HeightEffect::GetInstance().Enable();
	//HeightEffect::GetInstance().SetPlayerZ(playerPos.z);

	//m_fog.bindFrameBuffer();
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

	//HeightEffect::GetInstance().Enable();
	//HeightEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);
	//pps.Draw(false);

	//m_fog.unbindFrameBuffer();

	//END FOG STAGE

	//BEGIN GAUSSIAN HORIZONTAL BLUR

	pps.GetModelMat()->SetScale(Vector3f(1.0f / m_divisor, 1.0f / m_divisor, 1));

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glBindTexture(GL_TEXTURE_2D, m_fog.getColourTexture());
	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());
	BlurEffect::GetInstance().Enable();
	BlurEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);
	pps.Draw(false);

	m_gaussH.unbindFrameBuffer();

	//END GAUSSIAN HORIZONTAL BLUR

	//BEGIN GAUSSIAN VERTICAL BLUR

	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps.Draw(false);

	m_gaussV.unbindFrameBuffer();

	//END GAUSSIAN VERTICAL BLUR

	//BEGIN SECOND GAUSSIAN BLUR (FOR MAXIMUM BLURINESS)

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(true);
	pps.Draw(false);

	m_gaussH.unbindFrameBuffer();


	m_gaussV.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_gaussH.getColourTexture());
	BlurEffect::GetInstance().SetHorizontalBlur(false);
	pps.Draw(false);

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

	//Draw only buffer without combining
	//BasicEffect::GetInstance().Enable();
	//BasicEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, m_fog.getColourTexture());

	pps.Draw(false);
}

void HeightFog::ResetTextureSizes()
{
	m_fbo.resetTextures(m_width, m_height);
	m_gaussH.resetTextures((float)m_width / m_divisor, (float)m_height / m_divisor);
	m_gaussV.resetTextures(m_width / m_divisor, m_height / m_divisor);
	m_fog.resetTextures(m_width, m_height);
}
