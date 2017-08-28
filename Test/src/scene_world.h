#ifndef SCENE_WORLD_H__
#define SCENE_WORLD_H__

#include <iostream>
#include "scene.h"
#include "actor.h"
#include "shadowMapFBO.h"
#include "transform.h"
#include "drawable.h"
#include "map.h"
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
	void LoadAllResources();
	void UnloadAllResources();
	void ManageInput();
	
public:
	void RenderPass();

private:
	SceneWorld();
	GraphicsComponent* m_test = NULL;
	Entity* m_test2 = NULL;
	Entity* m_test3 = NULL;
	Actor* m_player = NULL;
	Camera* m_camera = NULL;
	Map* m_map = NULL;
	Transformation* m_World = NULL;
	std::vector<Drawable*>* m_objList = NULL;
	std::vector<Drawable*> m_clist;//collision list
	std::vector<Entity*> m_celist;
	bool m_pause;
	bool m_resources_loaded;
	bool m_acceptInput;
	bool m_bloomEffect;
	float m_camAngle = 0.0f;
	int m_numFrames = 0;
	Trail m_trail;
	Bloom m_bloom;
};

#endif