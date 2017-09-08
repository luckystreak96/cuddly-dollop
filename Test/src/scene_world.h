#ifndef SCENE_WORLD_H__
#define SCENE_WORLD_H__

#include <iostream>
#include "scene.h"
#include "shadowMapFBO.h"
#include "transform.h"
#include "array2d.h"
#include "elapsedTime.h"
#include "glutBackend.h"
#include "physics_2d.h"
#include "resource_manager.h"
#include "input_manager.h"
#include <gl/freeglut.h>
#include "basicEffect.h"
#include "bloomEffect.h"
#include "combineEffect.h"
#include "transparencyEffect.h"
#include "animation.h"
#include "define_gl.h"
#include "bloom.h"
#include "trail.h"
#include "graphicsComponent.h"
#include "entity.h"
#include "map_handler.h"
#include "soundManager.h"
#include "heightFog.h"

class SceneWorld : public Scene
{
public:
	static SceneWorld& GetInstance()
	{
		static SceneWorld instance;
		return instance;
	}

	bool Init();
	Scene* Act();
	void Draw();
	void Update();
	void Interact();
	void LoadAllResources();
	void UnloadAllResources();
	void ManageInput();
	void SetAudioPosition();
	
public:
	void RenderPass();

private:
	SceneWorld();
	Entity* m_test3 = NULL;
	Entity* m_player = NULL;
	Camera* m_camera = NULL;
	//Map* m_map = NULL;
	MapHandler* m_mapHandler = NULL;
	Transformation* m_World = NULL;
	std::vector<Entity*> m_celist;
	bool m_pause;
	bool m_resources_loaded;
	bool m_acceptInput;
	bool m_bloomEffect;
	float m_camAngle = 0.0f;
	int m_numFrames = 0;
	Trail m_trail;
	Bloom m_bloom;
	HeightFog m_fog;
};

#endif