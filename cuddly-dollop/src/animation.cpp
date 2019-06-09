#include "animation.h"
#include "resource_manager.h"

int Animation::m_progress = 0;
std::map<std::string, SpriteSheetData> Animation::m_metaData = std::map<std::string, SpriteSheetData>();
std::vector<Vector2f> Animation::m_baseTexCoords = {
	Vector2f(0, 0),
	Vector2f(1, 0),
	Vector2f(0, 1),
	Vector2f(1, 1)
};

void Animation::SetupAnimationMetaData()
{
	SpriteSheetData ssd;
	ssd.data.emplace(AE_Attack, AnimInfo(AE_Attack, 0, 1));
	m_metaData.emplace("res/sprites/entities/entity_ghost.png", ssd);

	// Slime has a jump animation
	ssd.data.at(AE_Jump) = AnimInfo(AE_Jump, 0, 3, 260);
	ssd.data.at(AE_Jump)._start = 0;
	ssd.data.at(AE_Jump)._end = 3;
	m_metaData.emplace("res/sprites/entities/entity_slime.png", ssd);

	int moveDelay = 420;
	// Girl has walk animation
	ssd.data.at(AE_LeftMove)._start = 2;
	ssd.data.at(AE_LeftMove)._end = 5;
	ssd.data.at(AE_LeftMove)._delay = moveDelay;
	ssd.data.at(AE_RightMove)._start = 2;
	ssd.data.at(AE_RightMove)._end = 5;
	ssd.data.at(AE_RightMove)._delay = moveDelay;
	ssd.data.at(AE_UpMove)._start = 2;
	ssd.data.at(AE_UpMove)._end = 5;
	ssd.data.at(AE_UpMove)._delay = moveDelay;
	ssd.data.at(AE_DownMove)._start = 2;
	ssd.data.at(AE_DownMove)._end = 5;
	ssd.data.at(AE_DownMove)._delay = moveDelay;
	ssd.data.at(AE_Jump) = AnimInfo(AE_Jump, 0, 3, 260);
	ssd.data.at(AE_Jump)._start = 0;
	ssd.data.at(AE_Jump)._end = 3;
	// Stand delay is slower
	int delay = 800;
	ssd.data.at(AE_Down)._delay = delay;
	ssd.data.at(AE_Up)._delay = delay;
	ssd.data.at(AE_Left)._delay = delay;
	ssd.data.at(AE_Right)._delay = delay;
	m_metaData.emplace("res/sprites/entities/entity_girl.png", ssd);
}

SpriteSheetData Animation::GetMetaData(std::string spritesheet)
{
	if (m_metaData.count(spritesheet))
	{
		return m_metaData[spritesheet];
	}
	else
	{
		return SpriteSheetData();
	}
}

Animation::Animation()
{
	SetDefaults();

	//m_delay = rand() % 20 + 160;
	srand(clock());
	m_delay = rand() % 140 + 260;
}

void Animation::SetWidthHeight(std::string tex)
{
	Texture* t = ResourceManager::GetInstance().GetTexture(tex);
	if (t != NULL)
	{
		m_width = t->_width / 32;
		m_height = t->_height / 32;
	}
}

void Animation::AnimationCounter(float et)
{
	m_progress += (int)(et * 1000.0f);
}

void Animation::SetDefaults()
{
	m_delay = 260;
	m_numFrames = 2;
	m_horizontal = true;
	_row = AE_Down;
	_animation = (Anim_Enum)_row;
	m_tracking = 0;
	_sprite = 0;
	m_start = 0;
	m_width = 2;
	m_height = 4;
	_specialAnimation = false;
	_forceAnimation = false;
	_default_animation = AE_Last;
}

// Choose which animation to go for
void Animation::SetAnimation(Anim_Enum anim, std::string spritesheet)
{
	//if ((int)anim < m_height)
	//{
		auto data = GetMetaData(spritesheet).data;
		_row = (int)data.at(anim)._position;
		_animation = anim;
		m_numFrames = abs(data.at(anim)._end - data.at(anim)._start) + 1;
		m_start = data.at(anim)._start;
		m_delay = data.at(anim)._delay;
		m_tracking = m_progress;
	//}
}

bool Animation::SetTileModelTC(std::vector<Vertex>* verts, bool forceUpdate)
{
	int next = ((m_progress - m_tracking) % (m_delay * m_numFrames)) / m_delay + m_start;
	if (_sprite == next && !forceUpdate && !_specialAnimation)
		return false;

	if (!_specialAnimation)
		_sprite = next;

	float x_increment = 1.f / (float)m_width;
	float y_increment = 1.f / (float)m_height;
	float x = m_horizontal ? (float)(_sprite) / (float)m_width : (float)(m_width - (_row + 1)) * x_increment;
	float y = m_horizontal ? (float)(m_height - (_row + 1)) * y_increment : (float)(_sprite + 1) / (float)m_height;
	//x = 1 - x;
	//y = 1 - y;

	float halfPixX = 0.005f / (32.0f * (float)m_width);
	float halfpixY = 0.005f / (32.0f * (float)m_height);

	for (int i = 0; i < verts->size(); i++)
	{
		verts->at(i).tex.x = m_baseTexCoords[i % 4].x == 0 ? x + halfPixX : x + x_increment - halfPixX;
		verts->at(i).tex.y = m_baseTexCoords[i % 4].y == 0 ? y + halfpixY : y + y_increment - halfpixY;
	}
	return true;
}


Anim_Enum Animation::GetMoveDirection(Anim_Enum direction)
{
	if (direction == AE_Left)
		return AE_LeftMove;
	else if (direction == AE_Right)
		return AE_RightMove;
	else if (direction == AE_Up)
		return AE_UpMove;
	else if (direction == AE_Down)
		return AE_DownMove;
	else
		return AE_Down;
}
