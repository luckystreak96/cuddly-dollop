#ifndef GRAPHICS_COMPONENT_H__
#define GRAPHICS_COMPONENT_H__

#include "iComponent.h"
#include <iostream>
#include <string>
#include <math.h>
#include <cmath>
#include "mathutils.h"
#include "model.h"
#include "math.h"
#include "array2d.h"
#include "define.h"
#include "basicEffect.h"
#include "elapsedTime.h"
#include "resource_user.h"
#include "resource_manager.h"

class GraphicsComponent : public IComponent, public ResourceUser
{
public:
	void ReceiveMessage(std::vector<std::string> msg);

	GraphicsComponent(std::string modelName = "TILE", std::string texPath = std::string("res/mushroom.png"));
	GraphicsComponent(std::vector<Vertex> verts, std::vector<GLuint> inds, std::string texPath = std::string("res/mushroom.png"));
	~GraphicsComponent();
	void Construct();
	void SetDefaults(std::string name = "TILE");
	std::string GetName();
	Transformation* GetModelMat() { return &m_modelMat; };
	virtual void Draw(bool withTex = true);
	virtual void Update();//Needs pos from physics component
	std::vector<Vertex> GetVertices();
	std::vector<Vertex> GetTranslatedVertices();
	std::vector<GLuint> GetIndices();
	std::string GetTexture();
	std::vector<Vector3f>* GetModels() { if (m_models == NULL) m_models = new std::vector<Vector3f>(); return m_models; }
	int GetHighestIndex();
	void SetBuffers();
	void ResetVBO();
	void SetTranslatedVertices(Vector3f pos, Vector3f vel);
	bool LoadExternalResources();
	bool UnloadExternalResources();
	bool LoadGLResources();
	bool UnloadGLResources();
	void SetPhysics(Vector3f pos, Vector3f vel) { m_pos = pos; m_vel = vel; m_modelMat.SetTranslation(pos); };
	//static inline bool SortFunc(GraphicsComponent* d, GraphicsComponent* d2) { return d->Position().z < d2->Position().z; }

public:
	bool mustDraw = true;

protected:
	GLuint m_mlMatLoc;
	GLuint m_IBO;
	GLuint m_VBO;
	GLuint m_MBO = 0;
	int m_MBO_instances = 1;
	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
	std::vector<Vertex> m_translatedVertices;
	std::vector<Vertex> m_originalVertices;
	std::vector<Vector3f>* m_models = NULL;
	Vector3f m_size = Vector3f(-1, -1, -1);
	Vector3f m_normalSize = Vector3f(-1, -1, -1);
	Vector3f m_pos = Vector3f();
	std::string m_texture;
	std::string m_modelName; //cancer
	bool m_external_loaded = false;
	bool m_GL_loaded = false;
	Transformation m_modelMat = Transformation();
	Vector3f m_vel = Vector3f();
	Vector3f m_rot = Vector3f(0, 0, 0);
};

#endif
