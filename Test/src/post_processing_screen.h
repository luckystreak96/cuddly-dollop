#ifndef POST_PROCESSING_SCREEN_H__
#define POST_PROCESSING_SCREEN_H__

#include <iostream>
#include <string>
#include "texture.h"
#include "model.h"
#include "math.h"
#include "vector3f.h"
#include <vector>
#include "graphicsComponent.h"


class Post_Processing_Screen : public GraphicsComponent
{
public:
	// I use SCREEN so that GraphicsComponent stops moving the scene by 0.5f
	Post_Processing_Screen(std::string modelName = "SCREEN", std::string texPath = "", Vector3f bbsize = Vector3f(1, 1, 0), Vector3f centerSize = Vector3f(0, 0, 0));
};

#endif
