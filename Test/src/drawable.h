#ifndef DRAWABLE_H__
#define DRAWABLE_H__

#include <iostream>
#include <string>
#include <math.h>
#include <cmath>
#include "mathutils.h"
#include "model.h"
#include "math.h"
#include "entity2.h"
#include "array2d.h"
#include "define.h"
#include "basicEffect.h"
#include "elapsedTime.h"
#include "resource_user.h"
#include "resource_manager.h"
#include "blur_framebuffer.h"

class Drawable : public Entity2, public ResourceUser
{
public:
	Drawable(Vector3f pos = Vector3f(0, 0, 0), std::string modelName = "TILE", std::string texPath = std::string("res/test.png"));
	void SetDefaults(std::string name = "TILE");//cancer
	std::string GetName();
	Transformation* GetModelMat() { return &m_modelMat; };
	void Drawable::DrawNoTexture();
	virtual void Draw();
	virtual void DrawShadowMap(Transformation& p) {};
	virtual void Update();
	virtual std::vector<Drawable*>* GetContainedObjects() { return NULL; };
	bool IsContainer() { return m_isContainer; };
	void SetAsContainer() { m_isContainer = true; };
	void Move();
	void MoveBB(Vector3f distance);
	void SetBoundingBox();
	void Drawable::SetBoundingBoxSize(Vector3f size, Vector3f numSquare = Vector3f(1, 1, 1));
	std::array<float, 6> GetBoundingBox();
	std::array<float, 6> GetMoveBoundingBox();
	Vector3f BBSize();
	void RelativePosition(Vector3f movementPos);
	void AbsolutePosition(Vector3f absolutePos, Vector3f useAxis = Vector3f(1, 1, 1));
	void SetSpecularStuff();
	std::vector<Vertex> GetVertices();
	std::vector<Vertex> GetTranslatedVertices();
	std::vector<GLuint> GetIndices();
	std::string GetTexture();
	int GetHighestIndex();
	void SetBuffers();
	void Drawable::ResetVBO();
	void Drawable::XCollide();
	void Drawable::YCollide();
	virtual void DesiredMove();
	//Wont be used in this 2d context
	//void SetMovementBB();
	void SetMovedBB();
	void SetTranslatedVertices();
	void CollisionMovement(Vector3f normal, float percent, bool secondCollision, Vector3f traject, int vertIndex, Vector3f poc);
	void SetGrounded(bool was_on_ground);
	bool LoadExternalResources();
	bool UnloadExternalResources();
	bool LoadGLResources();
	bool UnloadGLResources();
	static inline bool SortFunc(Drawable* d, Drawable* d2) { return d->Position().z < d2->Position().z; }

public:
	bool mustDraw = true;
	bool mustCollide = true;

protected:
	GLuint m_mlMatLoc;
	GLuint m_IBO;
	GLuint m_VBO;
	GLuint m_MBO;
	int m_MBO_instances = 1;
	std::vector<GLuint> m_indices;
	std::vector<Vertex> m_vertices;
	std::vector<Vertex> m_translatedVertices;
	std::vector<Vertex> m_originalVertices;
	std::vector<Vector3f> m_models;
	Vector3f m_size = Vector3f(-1, -1, -1);
	Vector3f m_normalSize = Vector3f(-1, -1, -1);;
	//Texture m_texture;
	std::string m_texture;
	float m_specularIntensity = 0.0f;
	float m_specularPower = 0.0f;
	bool m_was_on_ground = false;
	bool m_ground_check = false;
	bool m_isContainer = false;
	std::string m_modelName = "NONE"; //cancer
	bool m_external_loaded = false;
	bool m_GL_loaded = false;
	Transformation m_modelMat = Transformation();
	Vector3f m_rot = Vector3f(0, 0, 0);
	std::array<float, 6> m_boundingBox = std::array<float, 6>();
	std::array<float, 6> m_moveBoundingBox = std::array<float, 6>();
};

#endif
