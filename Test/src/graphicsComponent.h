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
#include "spatial_object.h"
#include "textured_object.h"

#include <iostream>
#include <string>
#include <math.h>
#include <cmath>

enum Direction { dir_Left, dir_Down, dir_Right, dir_Up };

class GraphicsComponent;
typedef std::shared_ptr<GraphicsComponent> GraphComp_ptr;

class GraphicsComponent : public IComponent, public SpatialObject, public TexturedObject
{
public:
	void ReceiveMessage(std::vector<std::string> msg);

	GraphicsComponent(std::string modelName = "TILE", std::string texPath = std::string("res/sprites/tiles/mushroom.png"));
	GraphicsComponent(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string texPath = std::string("res/sprites/tiles/mushroom.png"));
	~GraphicsComponent();
	void set_tex_coord_offsets(std::vector<Vector2f>* tex);
	void modify_tex_offset(int pos, Vector2f newoffset);
	void FullReset(std::vector<Vertex>* verts, std::vector<GLuint>* inds);
	void Construct();
	void SetDefaults(std::string name = "TILE");
	std::string GetName();
	Transformation* GetModelMat() { return &m_modelMat; };
	virtual void Draw(bool withTex = true);
	virtual void Update();//Needs pos from physics component
	void update_direction(Vector3f);
	std::vector<Vertex>* GetVertices();
	std::vector<GLuint> GetIndices();
	std::vector<Mat4f>& GetMModels();
	void InsertMModels(Transformation& t);
	// get_position is zero-counted
	void InsertMModels(Transformation& t, int position);
	void InsertMModels(Mat4f& mat, int position);
	void UpdateMModels();
	// Returns true if the position changed
	bool UpdateTranslation();
	GLuint GetMVBO() { return m_VBO; }
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
	bool LoadGLResources();
	bool UnloadGLResources();
	void SetNewBuffers(std::vector<Vertex>* verts, std::vector<GLuint>* inds);
	void ApplyModelToVertices();
	bool will_not_draw();
	static float GetProjectionRotation();

public:
	bool _must_update_vbo_ibo;
	bool _must_update_vbo;
	bool mustDraw = true;
	bool _instancedDraw;
	bool _instanced_tex_coord_draw;
	bool _mModelsNoReplace;
	bool _outline;

	static float _persRotation;
	static float _orthoRotation;

protected:
	GLsizei m_lastMModelSize;
	GLuint m_mlMatLoc;
	GLuint m_IBO;
	GLuint m_VBO;
	GLuint m_MMBO;
	GLuint m_TBO;
	GLuint m_VAO;
	Direction m_direction;
	int m_prev_dir;
	bool m_dir_determined;
	Direction m_lastInteraction;
	int m_MBO_instances = 1;
	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
	std::vector<Vector2f> m_texCoords;
	std::vector<Mat4f> m_mmodels;
	std::vector<Mat4f> m_test;
	Vector3f m_size = Vector3f(-1, -1, -1);
	Vector3f m_normalSize = Vector3f(-1, -1, -1);
	Vector3f m_scale;
	std::string m_modelName;
	bool m_GL_loaded = false;
	bool m_mmodelsUpdated;
	bool m_updateMModels;
	Transformation m_modelMat;
	Vector3f m_rot;

	static int counter;
};

#endif
