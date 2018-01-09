#include "animation.h"
#include "resource_manager.h"

int Animation::m_progress = 0;
std::map<std::string, SpriteSheetData> Animation::m_metaData = std::map<std::string, SpriteSheetData>();

void Animation::SetupAnimationMetaData()
{
	SpriteSheetData ssd;
	m_metaData.emplace("entity_ghost.png", ssd);
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
	m_delay = 160;
	m_numFrames = 2;
	m_horizontal = true;
	m_animation = AE_Down;
	m_sprite = 0;
	m_width = 2;
	m_height = 4;
	_specialAnimation = false;
}

// Choose which animation to go for
void Animation::SetAnimation(Anim_Enum anim, std::string spritesheet)
{
	if ((int)anim < m_height)
	{
		m_animation = (int)GetMetaData(spritesheet).data.at(anim).position;
		m_numFrames = (int)GetMetaData(spritesheet).data.at(anim).numFrames;
	}
}

bool Animation::SetTileModelTC(std::vector<Vertex>* verts, bool forceUpdate)
{
	const std::vector<Vector2f> vecs = {
	Vector2f(0, 0),
	Vector2f(1, 0),
	Vector2f(0, 1),
	Vector2f(1, 1)
	};

	int next = (m_progress % (m_delay * m_numFrames)) / m_delay;
	if (m_sprite == next && !forceUpdate)
		return false;

	m_sprite = next;

	float x_increment = 1.f / (float)m_width;
	float y_increment = 1.f / (float)m_height;
	float x = m_horizontal ? (float)(m_sprite) / (float)m_width : (float)m_animation * x_increment;
	float y = m_horizontal ? (float)m_animation * y_increment : (float)(m_sprite + 1) / (float)m_height;

	float halfPixX = 0.15f / (32.0f * (float)m_width);
	float halfpixY = 0.15f / (32.0f * (float)m_height);

	for (int i = 0; i < verts->size(); i++)
	{
		verts->at(i).tex.x = vecs[i].x == 0 ? x + halfPixX : x + x_increment - halfPixX;
		verts->at(i).tex.y = vecs[i].y == 0 ? y + halfpixY : y + y_increment - halfpixY;
	}
	return true;
}
