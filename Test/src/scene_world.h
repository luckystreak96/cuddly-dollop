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
#include "physics_vertex.h"
#include "resource_manager.h"
#include "input_manager.h"
#include <gl/freeglut.h>
#include "basicEffect.h"

struct CameraDirection
{
	GLenum CubemapFace;
	Vector3f Target;
	Vector3f Up;
};

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
	void ShadowMapPass();
	void RenderPass();

private:
	SceneWorld();
	Actor* m_player = NULL;
	Camera* m_camera = NULL;
	Map* m_map = NULL;
	Transformation* m_World = NULL;
	std::vector<Drawable*>* m_objList = NULL;
	float m_shadowMapSize;
	bool m_pause;
	bool m_resources_loaded;
	bool m_acceptInput;
	float m_camAngle = 0.0f;
};

#endif