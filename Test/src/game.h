#ifndef GAME_H__
#define GAME_H__

#include <iostream>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include "GL/glew.h"

#include "textureatlas.h"
#include "input_manager.h"
#include "vector3f.h"

class Game
{
public:
	Game();
	~Game();

	void renderSceneCB();
	void HandleInput();

	void MuteButton();

	bool init(Vector2f version);
	void SetupTextureAtlas();
private:
	bool m_exit;
};

#endif
