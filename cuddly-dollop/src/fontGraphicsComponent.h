#ifndef FONT_GRAPHICS_COMPONENT
#define FONT_GRAPHICS_COMPONENT

#include "graphics_component.h"

class FontGraphicsComponent : public GraphicsComponent 
{
public:
	FontGraphicsComponent() : GraphicsComponent() {}
	FontGraphicsComponent(std::string model, std::string path = "res/sprites/special/dialogue.png");
	//FontGraphicsComponent(std::vector<Vertex>* verts, std::vector<GLuint>* inds, std::string texPath = std::string("res/sprites/entities/mushroom_guy.png"));
	void Draw(bool withTex = true);
	void Update();
	void SetStatic(bool sTatic);
	void SetScale(Vector3f scale);
	Vector3f GetScale();

private:
	bool m_static = false;
	static Transformation m_staticityTrans;
};

#endif // !FONT_GRAPHICS_COMPONENT
