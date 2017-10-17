#ifndef FONT_H__
#define FONT_H__

#include "vector3f.h"
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <math.h>
#include "resource_manager.h"
#include "graphicsComponent.h"
#include "mesh.h"
#include "physicsComponent.h"
#include "renderer.h"
#include "fontGraphicsComponent.h"

class Font
{
public:
	Font(bool sTatic = false, bool temporary = false, bool lightSpeed = false, std::string path = "res/fonts/basic.png");
	~Font();

public:
	virtual void SetText(std::string text, Vector3f location = Vector3f(0, 0, 0), bool centered = false, float xBoundry = -1);
	void ChangeLetter(unsigned int index, char newChar);
	virtual void Draw();
	virtual void Update(double elapsedTime);
	virtual void SetRender();
	void SetTextSpeed(double speed);
	FontGraphicsComponent* GetGraphics() { return m_graphics; }
	bool IsDead();
	virtual void SetScale(float xScale, float yScale);
	bool TextDisplayDone();

protected:
	void AddWordToMesh(std::string word, float x, float y);
	void CreateHash();
	void SetupMesh(float xBoundry = -1, float yBoundry = -1);
	unsigned int CharToCode(char c);

protected:
	const double MaxTime;
	const double m_timePerLetter;
	const float LetterSpacing;
	double m_elapsedTime;
	double m_textSpeed;
	double m_lifetime;
	double m_totalTime;

	float m_xScale;
	float m_yScale;
	float m_x;
	float m_y;

	bool m_static;
	bool m_temporary;
	bool m_lightSpeed;

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
