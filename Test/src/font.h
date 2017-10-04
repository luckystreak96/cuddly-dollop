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
	virtual void SetText(std::string text, Vector3f location = Vector3f(0, 0, 0), bool centered = false);
	void ChangeLetter(unsigned int index, char newChar);
	virtual void Draw();
	virtual void Update(double elapsedTime);
	virtual void SetRender();
	void SetTextSpeed(double speed);
	FontGraphicsComponent* GetGraphics() { return m_graphics; }
	bool IsDead();
	void SetScale(float xScale, float yScale);
	bool TextDisplayDone();

protected:
	void CreateHash();
	void SetupMesh();
	unsigned int CharToCode(char c);

protected:
	const double MaxTime;
	const float LetterSpacing;
	double m_elapsedTime;
	double m_textSpeed;
	double m_lifetime;
	double m_totalTime;

	float m_xScale;

	bool m_static;
	bool m_temporary;

	FontGraphicsComponent* m_graphics = NULL;
	std::map<char, unsigned int> m_letters;
	Mesh m_mesh;
	//std::vector<Vertex> m_verts;
	const unsigned int m_lettersPerRow;
	const unsigned int m_lettersPerColumn;

	PhysicsComponent m_phys;
	Transformation trans;

	Vector3f m_basePosition;
	std::string m_texture;
	std::string m_message;
	std::string m_messageProgress;
};

#endif // !FONT_H__
