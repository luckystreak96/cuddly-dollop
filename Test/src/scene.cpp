#include "scene.h"
#include "mat4f.h"
#include "map_handler.h"
#include "input_manager.h"
#include "soundManager.h"
#include "effectManager.h"
#include "entity.h"

SceneGenData Scene::NextScene = SceneGenData();
bool Scene::_pause = false;
int Scene::_numFrames = 0;

Scene::Scene()
{
	Camera::_currentCam = &m_camera;
}

void Scene::Brb()
{
	NextScene = SceneGenData();
}

void Scene::Resume()
{
	Camera::_currentCam = &m_camera;
	//m_camera._mapsize = m_mapHandler->GetMapSize();
	//OrthoProjInfo::GetRegularInstance().changed = true;
}

void Scene::ManageInput()
{
	InputManager::GetInstance().SetupFrameKeys();

	if (InputManager::GetInstance().FrameKeyStatus(A_Pause, KeyPressed, 100))
		_pause = !_pause;

	if (InputManager::GetInstance().FrameKeyStatus('O', KeyPressed, 100))
		_numFrames++;
}


SceneGenData Scene::Act()
{
	SceneGenData result;
	result.id = 0;
	result.sceneType = ST_World;
	ManageInput();

	//RENDER SETUP WITH FRAME BY FRAME
	if (!_pause || _numFrames > 0)
	{
		//If we pause to slowly pass frames...
		if (_numFrames > 0)
		{
			_numFrames--;
			//...then set the elapsedtime to the desired amount (in fps)
			ElapsedTime::GetInstance().SetBufferElapsedTime(60.f);
		}
		result = Update();
		m_camera.Update();
	}

	//DRAW
	Draw();

	switch(_type)
	{
	case ST_Battle:
		if (!m_fade.IsDone() || result.id == 0)
		{
			result.id = 0;
			result.scene = NULL;
			return result;
		}
		if (result.scene)
			result.scene->Resume();
		break;
	case ST_World:
		if (!m_fade.IsDone() || result.id == 0 && result.sceneType != ST_Battle)
		{
			result.id = 0;
			return result;
		}
		Brb();
		break;
	}

	return result;
}

void Scene::SetAudioPosition()
{
	SoundManager::GetInstance().SetListenerPosition();
}

void Scene::SetOrthoStuffs()
{
	// Adjust the shaders and camera to new screen size
	if (OrthoProjInfo::GetRegularInstance().changed)
	{
		OrthoProjInfo* o = &OrthoProjInfo::GetRegularInstance();
		m_camera._transform->SetOrthoProj(o);
		//m_camera._transform->SetTranslation(-o->Right / o->Size, -o->Top / o->Size, 0);

		EffectManager::GetInstance().SetAllTilePositions(OrthoProjInfo::GetRegularInstance().Size);

		OrthoProjInfo::GetRegularInstance().changed = false;
	}

	m_backupTrans = m_camera._transform->GetTranslation();

	Vector3f temp = m_backupTrans;
	temp.x *= OrthoProjInfo::GetRegularInstance().Size;
	temp.y *= OrthoProjInfo::GetRegularInstance().Size;

	// Floor so we dont move the camera half a pixel and fuck up the graphics
	temp.x = floorf(temp.x);
	temp.y = floorf(temp.y);
	m_camera._transform->SetTranslation(temp);

	EffectManager::GetInstance().SetWorldTrans(&m_camera._transform->GetWOTransCentered().m[0][0], &m_camera._transform->GetWOTransNoTranslate().m[0][0]);
	EffectManager::GetInstance().ResetWorldUpdateFlag();
}

void Scene::DrawBegin()
{
	// Allow modifications to stencil buffer
	glStencilMask(0xFF);
	// Clear stencil buffer to all 0's
	glClearStencil(0x00);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// Don't allow modifications to stencil buffer
	glStencilMask(0x00);

	// Setup world trans and change for effects
	SetOrthoStuffs();

	EffectManager::GetInstance().Enable(E_Basic);

	// Enable fade
	if (!m_fade.IsDone())
		m_fade.Begin();

	Renderer::GetInstance().Setup();
}

void Scene::DrawEnd()
{
	// Draw
	Renderer::GetInstance().Draw();

	// Set translation back to real value, not adjusted by tilesize (64)
	m_camera._transform->SetTranslation(m_backupTrans);
}
