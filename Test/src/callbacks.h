#ifndef CALLBACKS_H__
#define CALLBACKS_H__

class ICallBacks
{
    public:
        virtual void keyboardCB(unsigned char key, int x, int y) {}
        virtual void specialKeyboardCB(int key, int x, int y) {}
		virtual void keyboardUpCB(unsigned char key, int x, int y) {}
		virtual void specialKeyboardUpCB(int key, int x, int y) {}
		virtual void windowResizeCB(int width, int height) {}

        //virtual void PassiveMouseCB(int x, int y) {};

        virtual void renderSceneCB() {}

        //virtual void MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y) {};
};

#endif
