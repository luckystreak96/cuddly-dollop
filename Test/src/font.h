#ifndef FONT_H__
#define FONT_H__

#include "vector3f.h"
#include <map>
#include "resource_manager.h"
#include "graphicsComponent.h"
#include "mesh.h"
#include "physicsComponent.h"
#include "renderer.h"
#include "fontGraphicsComponent.h"

class Font
{
public:
	Font(bool sTatic = false, bool temporary = false, std::string path = "res/fonts/basic.png");
	~Font();

public:
	void SetText(std::string text, Vector3f location = Vector3f(0, 0, 0), bool centered = false);
	void ChangeLetter(unsigned int index, char newChar);
	void Draw();
	void Update(double elapsedTime);
	void SetRender();
	FontGraphicsComponent* GetGraphics() { return m_graphics; }
	bool IsDead() { return m_temporary && m_elapsedTime > m_lifetime; }

private:
	void CreateHash();
	void SetupMesh();
	unsigned int CharToCode(char c);

private:
	const double MaxTime = 30000;
	double m_elapsedTime;
	double m_textSpeed;
	double m_lifetime;
	double m_totalTime;

	float m_width = 0.5f;
	float m_height = 0.5f;
	float m_baseX = 0;
	float m_baseY = 0;

	bool m_static;
	bool m_temporary;

	FontGraphicsComponent* m_graphics = NULL;
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
	std::string m_messageProgress;
};

#endif // !FONT_H__
