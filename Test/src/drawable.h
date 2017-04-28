#ifndef DRAWABLE_H__
#define DRAWABLE_H__

#include <iostream>
#include <string>
#include <math.h>
#include <cmath>
#include "mathutils.h"
#include "texture.h"
#include "model.h"
#include "math.h"
#include "entity.h"
#include "array2d.h"
#include "define.h"
#include "pointLightShadowEffect.h"
#include "basicEffect.h"
#include "shadowMapEffect.h"
#include "elapsedTime.h"
#include "resource_user.h"
#include "resource_manager.h"

class Drawable : public Entity, public ResourceUser
{
    public:
		Drawable(Vector3f pos = Vector3f(0, 0, 0), std::string modelName = "NONE", std::string texPath = std::string("res/test.png"));
        void SetDefaults(std::string name = "TILE");//cancer
		std::string GetName();
		virtual void Draw();
		virtual void DrawShadowMap(Transformation& p) {};
        void Update();
		void Move();
		void MoveBB(Vector3f distance);
		void SetBoundingBox();
		Array2d<float> GetBoundingBox();
		Array2d<float> GetMoveBoundingBox();
		Vector3f BBSize();
		void RelativePosition(Vector3f movementPos);
		void AbsolutePosition(Vector3f absolutePos);
		void SetSpecularStuff();
		std::vector<Vertex> GetVertices();
		std::vector<Vertex> GetTranslatedVertices();
		std::vector<GLuint> GetIndices();
		std::string GetTexture();
		int GetHighestIndex();
		void SetBuffers();
		virtual void DesiredMove();
		void SetMovementBB();
		void SetTranslatedVertices();
		void CollisionMovement(Vector3f normal, float percent, bool secondCollision, Vector3f traject, int vertIndex, Vector3f poc);
		void SetGrounded(bool was_on_ground);
		bool LoadExternalResources();
		bool UnloadExternalResources();
		bool LoadGLResources();
		bool UnloadGLResources();
		static inline bool SortFunc(Drawable* d, Drawable* d2) { return d->Position().z > d2->Position().z; }

public:
		bool mustDraw = true;
		bool mustCollide = true;

    protected:
        GLuint m_mlMatLoc;
        GLuint m_IBO;
        GLuint m_VBO;
        std::vector<GLuint> m_indices;
        std::vector<Vertex> m_vertices;
		std::vector<Vertex> m_translatedVertices;
		Vector3f m_size = Vector3f();
		//Texture m_texture;
		std::string m_texture;
		float m_specularIntensity = 0.0f;
		float m_specularPower = 0.0f;
		bool m_was_on_ground = false;
		bool m_ground_check = false;
		std::string m_modelName = "NONE"; //cancer
		bool m_external_loaded = false;
		bool m_GL_loaded = false;
        Transformation m_modelMat = Transformation();
        Vector3f m_rot = Vector3f(0, 0, 0);
		Array2d<float> m_boundingBox = Array2d<float>(6, 1);
		Array2d<float> m_moveBoundingBox = Array2d<float>(6, 1);
};

#endif
