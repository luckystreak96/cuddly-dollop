#ifndef FONT_H__
#define FONT_H__

#include "vector3f.h"
#include <map>
#include "resource_manager.h"
#include "graphicsComponent.h"
#include "mesh.h"
#include "physicsComponent.h"
#include "renderer.h"

class Font
{
public:
	Font(std::string path = "res/fonts/basic.png");
	~Font();

public:
	void SetText(std::string text, Vector3f location = Vector3f(0, 0, 0), bool centered = false);
	void ChangeLetter(unsigned int index, char newChar);
	void Draw();
	void SetRender() { Renderer::GetInstance().Add(m_graphics); }

private:
	void CreateHash();
	void SetupMesh();
	unsigned int CharToCode(char c);

private:
	float m_width = 0.5f;
	float m_height = 0.5f;
	float m_baseX = 0;
	float m_baseY = 0;

	GraphicsComponent* m_graphics = NULL;
	std::map<char, unsigned int> m_letters;
	Mesh m_mesh;
	//std::vector<Vertex> m_verts;
	unsigned int m_lettersPerRow = 16;
	unsigned int m_lettersPerColumn = 16;

	PhysicsComponent m_phys;
	Transformation trans;

	Vector3f m_position = Vector3f();
	Vector3f m_basePosition = Vector3f();
	std::string m_texture;
	std::string m_message;
};

#endif // !FONT_H__
