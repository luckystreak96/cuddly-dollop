#ifndef ANIMATION_H__
#define ANIMATION_H__

#include <vector>
#include <map>
#include <string>
#include "vertex.h"
#include <time.h>

// To add a new one - put it before AE_MoveLeft -> Otherwise the _position will be wrong upon enum name use
enum Anim_Enum { AE_Left, AE_Down, AE_Right, AE_Up, AE_Attack, AE_Jump, AE_LeftMove, AE_DownMove, AE_RightMove, AE_UpMove, AE_Last };

struct AnimInfo
{
	AnimInfo(int pos, int start, int end, int delay = 160) { _position = pos; _start = start; _end = end; _delay = delay; }

	// Which row the animation is in
	int _position;
	// which sprite does it start at
	int _start;
	// which sprite does it end at
	int _end;
	// delay between frames of the animation
	int _delay;
};

struct SpriteSheetData
{
	SpriteSheetData() {
		data.emplace(AE_Down, AnimInfo(AE_Down, 0, 1, 400));
		data.emplace(AE_Up, AnimInfo(AE_Up, 0, 1, 400));
		data.emplace(AE_Left, AnimInfo(AE_Left, 0, 1, 400));
		data.emplace(AE_Right, AnimInfo(AE_Right, 0, 1, 400));
		data.emplace(AE_Jump, AnimInfo(AE_Right, 0, 1, 400));
		data.emplace(AE_DownMove, AnimInfo(AE_Down, 0, 1, 260));
		data.emplace(AE_UpMove, AnimInfo(AE_Up, 0, 1, 260));
		data.emplace(AE_LeftMove, AnimInfo(AE_Left, 0, 1, 260));
		data.emplace(AE_RightMove, AnimInfo(AE_Right, 0, 1, 260));
	}
	std::map<Anim_Enum, AnimInfo> data;
};

class Animation
{
public:
	Animation();
	static void AnimationCounter(float et);
	bool SetTileModelTC(std::vector<Vertex>* verts, bool forceUpdate = false);
	void SetDefaults();
	void SetWidthHeight(std::string texture);
	Anim_Enum GetMoveDirection(Anim_Enum direction);

	static void SetupAnimationMetaData();
	static SpriteSheetData GetMetaData(std::string spritesheet);

public:
	// true = we're specifying sprite changes
	bool _specialAnimation;
	// true = we choose the animation
	bool _forceAnimation;
	//Which frame we're at
	int _sprite;
	//which row/column to start in
	int _row;
	Anim_Enum _animation;
	Anim_Enum _default_animation;

protected:
	void SetAnimation(Anim_Enum anim, std::string spritesheet);

	//ElapsedTime progress
	static int m_progress;
	static std::map<std::string, SpriteSheetData> m_metaData;
	static std::vector<Vector2f> m_baseTexCoords;
protected:
	// m_progress to reset the animation
	int m_tracking;
	//Number of millis between frames
	int m_delay;
	//Number of desired frames
	int m_numFrames;
	//Whether the sprites go down or right
	bool m_horizontal;
	//The total image size
	int m_start;
	int m_width;
	int m_height;
};

#endif