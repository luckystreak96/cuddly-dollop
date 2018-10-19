#ifndef GRAPHICS_COMPONENT_H__
#define GRAPHICS_COMPONENT_H__

#include <GL/glew.h>//keep this up here to avoid "included gl.h before glew.h" error
#include "iComponent.h"
#include "mathutils.h"
#include "vertex.h"
#include "math.h"
#include "array2d.h"
#include "define.h"
#include "basicEffect.h"
#include "elapsedTime.h"
#include "resource_user.h"
#include "resource_manager.h"

#include <iostream>
#include <string>
#include <math.h>
#include <cmath>

enum Direction { dir_Left, dir_Down, dir_Right, dir_Up };

class GraphicsComponent;
typedef std::shared_ptr<GraphicsComponent> GraphComp_ptr;

class GraphicsComponent : public IComponent, public ResourceUser
{
public:
	void ReceiveMessage(std::vector<std::string> msg);

	GraphicsComponent(std::string modelName = "TILE", std::string texPath = std::string("res/sprites/tiles/mushroom.png"));
	GraphicsComponent(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string texPath = std::string("res/sprites/tiles/mushroom.png"));
	~GraphicsComponent();
	void FullReset(std::vector<Vertex>* verts, std::vector<GLuint>* inds);
	void Construct();
	void SetDefaults(std::string name = "TILE");
	std::string GetName();
	Transformation* GetModelMat() { return &m_modelMat; };
	virtual void Draw(bool withTex = true);
	virtual void Update();//Needs pos from physics component
	std::vector<Vertex>* GetVertices();
	std::vector<GLuint> GetIndices();
	std::string GetTexture();
	virtual void SetTexture(std::string newTex);
	std::vector<Mat4f>& GetMModels();
	void InsertMModels(Transformation& t);
	// Position is zero-counted
	void InsertMModels(Transformation& t, int position);
	void InsertMModels(Mat4f& mat, int position);
	void UpdateMModels();
	// Returns true if the position changed
	bool UpdateTranslation();
	Vector3f& GetPosRef() { return m_pos; }
	GLuint GetMVBO() { return m_VBO; }
	Vector3f GetPos();
	Direction GetDirection();
	void SetDirection(Direction dir);
	void SetDirection(GraphComp_ptr graph);
	void SetScale(float x, float y, float z);
	void SetScale(Vector3f scale);
	int GetHighestIndex();
	void ClearMModels();
	void SetBuffers();
	void SetupVAO();
	void SetColorAll(Vector3f color = Vector3f(1, 1, 1), float alpha = 1.0f);
	void ResetVBO();
	bool LoadExternalResources();
	bool UnloadExternalResources();
	bool LoadGLResources();
	bool UnloadGLResources();
	void SetPhysics(Vector3f pos, Vector3f vel = Vector3f()) { m_pos = pos; m_vel = vel;/* m_modelMat.SetTranslation(pos);*/ };
	void SetNewBuffers(std::vector<Vertex>* verts, std::vector<GLuint>* inds);
	void ApplyModelToVertices();
	static float GetProjectionRotation();

public:
	bool _must_update_vbo_ibo;
	bool _must_update_vbo;
	bool mustDraw = true;
	bool _instancedDraw;
	bool _mModelsNoReplace;
	bool _outline;

	static float _persRotation;
	static float _orthoRotation;

protected:
	GLsizei m_lastMModelSize;
	GLuint m_mlMatLoc;
	GLuint m_IBO;
	GLuint m_VBO;
	GLuint m_MMBO = 0;
	GLuint m_VAO;
	Direction m_direction;
	Direction m_lastInteraction;
	int m_MBO_instances = 1;
	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
	//std::vector<Vertex> m_originalVertices;
	std::vector<Mat4f> m_mmodels;
	std::vector<Mat4f> m_test;
	Vector3f m_size = Vector3f(-1, -1, -1);
	Vector3f m_normalSize = Vector3f(-1, -1, -1);
	Vector3f m_pos;
	Vector3f m_scale;
	std::string m_texture;
	std::string m_modelName;
	bool m_external_loaded = false;
	bool m_GL_loaded = false;
	bool m_mmodelsUpdated;
	bool m_updateMModels;
	Transformation m_modelMat;
	Vector3f m_vel;
	Vector3f m_rot;

	static int counter;
};

#endif
