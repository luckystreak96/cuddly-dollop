#include "trail.h"
#include "glfwBackend.h"

Trail::Trail() : m_width(1), m_height(1)
{
	CombineEffect::GetInstance().SetIntensity(0.9f);
	pps.LoadGLResources();

	m_frames = std::vector<FBO>();
	for (int i = 0; i < m_numFrames; i++)
		m_frames.push_back(FBO(1, 1, FBO::DEPTH_TEXTURE));
}

void Trail::Begin()
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

	m_frameCounter++;

	if (m_frameCounter >= m_numFrames)
		m_frameCounter = 0;

	//Setup the tile to draw
	pps.Update();
	pps.GetModelMat()->SetTranslation(0, 0, 0);

	//FBO
	m_fbo.bindFrameBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Trail::End()
{
	m_fbo.unbindFrameBuffer();

	//END FBO

	glDisable(GL_DEPTH_TEST);

	//BEGIN TRANSPARENCY STAGE

	m_trans.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

	TransparencyEffect::GetInstance().Enable();
	TransparencyEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);
	pps.Draw(false);

	m_trans.unbindFrameBuffer();

	//END TRANSPARENCY STAGE

	//BEGIN GAUSSIAN HORIZONTAL BLUR

	pps.GetModelMat()->SetScale(Vector3f(1.0f / m_divisor, 1.0f / m_divisor, 1));

	m_gaussH.bindFrameBuffer();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, m_trans.getColourTexture());
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

	//DRAW TO CURRENT FRAME

	m_frames.at(m_frameCounter).bindFrameBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BasicEffect::GetInstance().Enable();
	BasicEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);
	static Transformation t;
	t.SetOrthoProj(&OrthoProjInfo::GetRegularInstance());
	t.SetTranslation(OrthoProjInfo::GetRegularInstance().Left, OrthoProjInfo::GetRegularInstance().Bottom, 0);
	BasicEffect::GetInstance().SetWorldPosition(&t.GetWOTrans().m[0][0]);


	glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());

	pps.Draw(false);

	m_frames.at(m_frameCounter).unbindFrameBuffer();

	//END DRAW TO CURRENT FRAME

	//Combine blur with actual frame
	CombineEffect::GetInstance().Enable();
	CombineEffect::GetInstance().SetModelPosition(&pps.GetModelMat()->GetWorldTrans().m[0][0]);


	m_fbo.bindFrameBuffer();
	for (int i = 0; i < m_numFrames - 1; i++)
	{
		glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
		glBindTexture(GL_TEXTURE_2D, m_frames.at(i).getColourTexture());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

		pps.Draw(false);
	}
	m_fbo.unbindFrameBuffer();

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_frames.at(m_numFrames - 1).getColourTexture());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

	pps.Draw(false);

	BasicEffect::GetInstance().Enable();
	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());
	pps.Draw(false);


	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_gaussV.getColourTexture());
	//glBindTexture(GL_TEXTURE_2D, m_frames.at(m_frameCounter).getColourTexture());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fbo.getColourTexture());

	pps.Draw(false);

	glEnable(GL_DEPTH_TEST);
}

void Trail::ResetTextureSizes()
{
	m_fbo.resetTextures(m_width, m_height);
	m_gaussH.resetTextures((int)((float)m_width / m_divisor), (int)((float)m_height / m_divisor));
	m_gaussV.resetTextures((int)((float)m_width / m_divisor), (int)(float)(m_height / m_divisor));
	m_trans.resetTextures(m_width, m_height);
	for (auto x : m_frames)
		x.resetTextures(m_width, m_height);
}