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
	Tile(Vector3f pos = Vector3f(), std::string modelName = "TILE", std::string texPath = "res/pattern.png");
	void Draw();
	void DrawShadowMap(Transformation& p);
private:
	void SetProperties();
};

#endif
