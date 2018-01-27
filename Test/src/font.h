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

class Font;
typedef std::shared_ptr<Font> Font_ptr;

class Font
{
public:
	Font(bool sTatic = false, bool temporary = false, bool lightSpeed = false, std::string path = "res/fonts/lowercase.png");
	~Font();

public:
	virtual void SetText(std::string text, Vector3f location = Vector3f(0, 0, -1), bool centered = false, float xBoundry = -1);
	void ChangeLetter(unsigned int index, uint32_t newChar);
	virtual void Draw();
	virtual void Update(double elapsedTime);
	virtual void SetRender();
	void SetTextVariables();
	void SetTextSpeed(double speed);
	std::shared_ptr<FontGraphicsComponent> GetGraphics() { return m_graphics; }
	bool IsDead();
	virtual void SetScale(float xScale, float yScale);
	bool TextDisplayDone();
	virtual void UpdateModel();
	void Reset();

public:
	bool _enabled;
	float _letterSpacing;

protected:
	void AddWordToMesh(std::string word, float x, float y);
	void CreateHash();
	void SetupMesh(float xBoundry = -1, float yBoundry = -1);
	unsigned int CharToCode(uint32_t c);

protected:
	const double MaxTime;
	const double m_timePerLetter;
	double m_elapsedTime;
	double m_textSpeed;
	double m_lifetime;
	double m_totalTime;

	float m_xScale;
	float m_yScale;
	float m_x;
	float m_y;
	float m_xBndry;

	bool m_static;
	bool m_temporary;
	bool m_lightSpeed;
	bool m_centered;

	std::shared_ptr<FontGraphicsComponent> m_graphics = NULL;
	std::vector<Vector3f> m_letterPositions;
	std::map<uint32_t, unsigned int> m_letters;
	Mesh m_mesh;
	//std::vector<Vertex> m_verts;
	const unsigned int m_lettersPerRow;
	const unsigned int m_lettersPerColumn;

	PhysicsComponent m_phys;
	Transformation trans;

	Vector3f m_basePosition;
	std::string m_texture;
	std::string m_text;
	std::vector<uint32_t> m_message;
	std::vector<uint32_t> m_messageProgress;
};

#endif // !FONT_H__
