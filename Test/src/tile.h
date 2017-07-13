#ifndef TILE_H__
#define TILE_H__

#include <iostream>
#include <string>
#include "texture.h"
#include "model.h"
#include "math.h"
#include "drawable.h"

class Tile : public Drawable
{
public:
	Tile(Vector3f pos = Vector3f(), std::string modelName = "TILE", std::string texPath = "res/pattern.png", Vector3f bbsize = Vector3f(1, 1, 0), Vector3f centerSize = Vector3f(1, 1, 0));
	void Draw();
	void Update();
	void DrawShadowMap(Transformation& p);
private:
	void SetProperties();
};

#endif
