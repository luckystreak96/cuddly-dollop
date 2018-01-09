#ifndef ANIMATION_H__
#define ANIMATION_H__

#include <vector>
#include <map>
#include <string>
#include "vertex.h"
#include <time.h>

//enum Direction { dir_Up, dir_Right, dir_Down, dir_Left };
enum Anim_Enum { AE_Up, AE_Right, AE_Down, AE_Left, AE_Last };

struct AnimInfo
{
	AnimInfo(int pos, int numFrame) { position = pos; numFrames = numFrame; }

	// Which row the animation is in
	int position;
	int numFrames;
};

struct SpriteSheetData
{
	SpriteSheetData() {
		data.emplace(AE_Down, AnimInfo(AE_Down, 2));
		data.emplace(AE_Up, AnimInfo(AE_Up, 2));
		data.emplace(AE_Left, AnimInfo(AE_Left, 2));
		data.emplace(AE_Right, AnimInfo(AE_Right, 2));
	}
	std::map<Anim_Enum, AnimInfo> data;
};

class Animation
{
public:
	Animation();
	static void AnimationCounter(float et);
	void SetAnimation(Anim_Enum anim, std::string spritesheet);
	bool SetTileModelTC(std::vector<Vertex>* verts, bool forceUpdate = false);
	void SetDefaults();
	void SetWidthHeight(std::string texture);

	static void SetupAnimationMetaData();
	static SpriteSheetData GetMetaData(std::string spritesheet);

public:
	// Specifies if something other than walking is going on
	bool _specialAnimation;

protected:
	//ElapsedTime progress
	static int m_progress;
	static std::map<std::string, SpriteSheetData> m_metaData;
protected:
	//Number of millis between frames
	int m_delay;
	//Number of desired frames
	int m_numFrames;
	//Whether the sprites go down or right
	bool m_horizontal;
	//which row/column to start in
	int m_animation;
	//Which frame we're at
	int m_sprite;
	//The total image size
	int m_width;
	int m_height;
};

#endif