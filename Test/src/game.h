#ifndef GAME_H__
#define GAME_H__

#include <iostream>//duh
#include <cassert>//Asserts
#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>//duh

#include "textureatlas.h"
#include "scenes.h"
#include "input_manager.h"

class Game
{
public:
	Game();
	~Game();

	void renderSceneCB();
	void HandleInput();

	void MuteButton();

	bool init();
	void SetupTextureAtlas();
private:
	bool m_exit;
};

#endif
