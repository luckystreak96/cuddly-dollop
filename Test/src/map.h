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

class Map : public Drawable
{
    public:
        Map();
        ~Map();
        Map(const std::string& filePath);
		void Draw();
		void DrawShadowMap(Transformation& p);
        void Update(double elapsedTime);
		Array3d<Tile*> Tiles();
		unsigned int Size();
		void SetupMesh();

    private:
        Vector3f m_pos = Vector3f(0, 0, 0);//camera movement and rotation needs to be handled in the gWorld matrix in game.cpp
        Vector3f m_rot = Vector3f(0, 0, 0);
		Array3d<Tile*> m_tiles = Array3d<Tile*>(5, 5, 3);
		Mesh m_mesh;
};

#endif
