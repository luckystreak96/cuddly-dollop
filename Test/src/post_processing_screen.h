#ifndef POST_PROCESSING_SCREEN_H__
#define POST_PROCESSING_SCREEN_H__

#include <iostream>
#include <string>
#include "texture.h"
#include "model.h"
#include "math.h"
#include "drawable.h"


class Post_Processing_Screen : public Drawable
{
public:
	Post_Processing_Screen(Vector3f pos = Vector3f(), std::string modelName = "SCREEN", std::string texPath = "", Vector3f bbsize = Vector3f(1, 1, 0), Vector3f centerSize = Vector3f(1, 1, 0));
	void Draw();
	void Update();
	void DrawShadowMap(Transformation& p);
private:
	void SetProperties();
};

#endif
