#ifndef FONT_GRAPHICS_COMPONENT
#define FONT_GRAPHICS_COMPONENT

#include "graphicsComponent.h"

class FontGraphicsComponent : public GraphicsComponent 
{
public:
	FontGraphicsComponent() : GraphicsComponent() {}
	FontGraphicsComponent(std::string model = "DIALOGUE_BOX", std::string path = "res/sprites/special/dialogue.png");
	FontGraphicsComponent(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string texPath = std::string("res/sprites/entities/mushroom_guy.png"));
	void Draw(bool withTex = true);
	void SetStatic(bool sTatic);
	void SetScale(Vector3f scale);

private:
	bool m_static = false;
	static Transformation m_staticityTrans;
};

#endif // !FONT_GRAPHICS_COMPONENT
