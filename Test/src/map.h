#ifndef MAP_H__
#define MAP_H__

#include <iostream>
#include <string>
#include "texture.h"
#include "shadowMapEffect.h"
#include "pointLightShadowEffect.h"
#include "model.h"
#include "array3d.h"
#include "math.h"
#include "gameObjects.h"
#include "mesh.h"
#include <vector>

class Map : public Drawable
{
    public:
        Map();
        ~Map();
        Map(const std::string& filePath);
		void Draw();
		void DrawShadowMap(Transformation& p);
        void Update();
		void DesiredMove();
		std::vector<Drawable*>* GetContainedObjects();
		std::vector<Drawable*> Tiles();
		unsigned int Size();
		void SetupMesh();

    private:
        Vector3f m_pos = Vector3f(0, 0, 0);//camera movement and rotation needs to be handled in the gWorld matrix in game.cpp
        Vector3f m_rot = Vector3f(0, 0, 0);
		std::vector<Drawable*> m_tiles = std::vector<Drawable*>();
		Mesh m_mesh;
};

#endif
