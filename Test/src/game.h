#ifndef GAME_H__
#define GAME_H__

#include <iostream>//duh
#include <cassert>//Asserts

#include <GL/glew.h>//duh
#include <GL/freeglut.h>//duh

#include "callbacks.h"
#include "glutBackend.h"
#include "array2d.h"
#include "shadowMapFBO.h"
#include "define.h"
#include "scenes.h"
#include "input_manager.h"

class Game : public ICallBacks
{
    public:
        Game();
        ~Game();

        void renderSceneCB();
        void keyboardCB(unsigned char key, int x, int y);
        void specialKeyboardCB(int key, int x, int y);
		void keyboardUpCB(unsigned char key, int x, int y);
		void specialKeyboardUpCB(int key, int x, int y);
		void windowResizeCB(int w, int h);

        bool init(float width, float height);
        void run();
    private:
		float m_shadowMapSize;
		int m_numFrames;
		bool m_exit;
		bool m_isFullscreen = IS_FULLSCREEN;
};

#endif
