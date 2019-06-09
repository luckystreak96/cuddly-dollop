#ifndef FONT_H__
#define FONT_H__

#include "vector3f.h"
#include "mesh.h"

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <math.h>

class Font;
typedef std::shared_ptr<Font> Font_ptr;

class Font
{
public:
	Font(bool sTatic = false, bool temporary = false, bool lightSpeed = false, std::string path = "res/fonts/lowercase.png");
	~Font();

public:
	virtual void SetText(std::string text, Vector3f location = Vector3f(0, 0, -1), bool centered = false, float xBoundry = -1);
	void UpdateText(std::string text);
	void ChangeLetter(unsigned int index, uint32_t newChar);
	virtual void Update(double elapsedTime);
	virtual void SetRender();
	void SetTextVariables();
	void SetTextSpeed(double speed);
	std::shared_ptr<GraphicsComponent> GetGraphics() { return m_graphics; }
	bool IsDead();
	virtual void SetScale(float xScale, float yScale);
	bool TextDisplayDone();
	virtual void UpdateModel();
	void UpdateModel(Vector3f offset);
	void Reset();
	void SetOffset(Vector3f offset);
	void set_text_var_resolution(bool resolve);

public:
	bool _enabled;
	float _letterSpacing;
	std::string _text;

protected:
	void AddWordToMesh(std::string word, float x, float y);
	void CreateHash();
	void SetupMesh(float xBoundry = -1, float yBoundry = -1);
	inline unsigned int CharToCode(uint32_t c);

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
	bool m_set_text_vars;

	std::vector<Vector3f> m_letterPositions;
	std::map<uint32_t, unsigned int> m_letters;

	Mesh m_mesh;
	std::shared_ptr<GraphicsComponent> m_graphics;

	const unsigned int m_lettersPerRow;
	const unsigned int m_lettersPerColumn;

	Vector3f m_pos;
	Vector3f m_basic_pos;
	Transformation trans;

	Vector3f m_basePosition;
	Vector3f m_offset;
	std::string m_texture;
	std::vector<uint32_t> m_message;
	std::vector<uint32_t> m_messageProgress;
};

#endif // !FONT_H__
