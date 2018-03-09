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

void Scene::Brb()
{
	NextScene = SceneGenData();
}

void Scene::Resume()
{
	Camera::Mapsize = m_mapHandler->GetMapSize();
	OrthoProjInfo::GetRegularInstance().changed = true;
}

void Scene::ManageInput()
{
	InputManager::GetInstance().SetupFrameKeys();

	if (InputManager::GetInstance().FrameKeyStatus(A_Pause, AnyRelease, 100))
		_pause = !_pause;

	if (InputManager::GetInstance().FrameKeyStatus('O', AnyRelease, 100))
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
		m_World->SetOrthoProj(o);
		m_World->SetTranslation(-o->Right / o->Size, -o->Top / o->Size, 0);

		EffectManager::GetInstance().SetAllTilePositions(OrthoProjInfo::GetRegularInstance().Size);

		if (m_celist.count(Camera::Target))
			for (int i = 0; i < 30; i++)
				Camera::Follow(m_celist.at(Camera::Target)->Physics()->Position(), m_World.get());

		OrthoProjInfo::GetRegularInstance().changed = false;
	}

	m_backupTrans = m_World->GetTranslation();

	Vector3f temp = m_backupTrans;
	temp.x *= OrthoProjInfo::GetRegularInstance().Size;
	temp.y *= OrthoProjInfo::GetRegularInstance().Size;

	// Floor so we dont move the camera half a pixel and fuck up the graphics
	temp.x = floorf(temp.x);
	temp.y = floorf(temp.y);
	m_World->SetTranslation(temp);

	EffectManager::GetInstance().SetWorldTrans(&m_World->GetWOTrans().m[0][0], &m_World->GetWOTransNoTranslate().m[0][0]);
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
}

void Scene::DrawEnd()
{
	// Draw
	Renderer::GetInstance().Draw();

	// Set translation back to real value, not adjusted by tilesize (64)
	m_World->SetTranslation(m_backupTrans);
}
