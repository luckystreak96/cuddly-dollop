#include "glutBackend.h"

#define FPS 60

static ICallBacks* s_callBacks = NULL;
static bool s_depth = false;

static void RenderSceneCB()
{
    s_callBacks->renderSceneCB();
}

static void KeyboardCB(unsigned char c, int x, int y)
{
    s_callBacks->keyboardCB(c, x, y);
}

static void SpecialKeyboardCB(int key, int x, int y)
{
    s_callBacks->specialKeyboardCB(key, x, y);
}

static void KeyboardUpCB(unsigned char c, int x, int y)
{
	s_callBacks->keyboardUpCB(c, x, y);
}

static void SpecialKeyboardUpCB(int key, int x, int y)
{
	s_callBacks->specialKeyboardUpCB(key, x, y);
}

static void WindowResizeCB(int x, int y)
{
	s_callBacks->windowResizeCB(x, y);
}

static void InitializeGlutCallbacks(){
    //Setting the main loop function for the window
    glutDisplayFunc(RenderSceneCB);//...re-iterates when the window state changes(ex:minimize, maximize,...)
    glutIdleFunc(RenderSceneCB);//..re-iterates constantly, whether you do shit or not
    glutSpecialFunc(SpecialKeyboardCB);
    glutKeyboardFunc(KeyboardCB);
	glutKeyboardUpFunc(KeyboardUpCB);
	glutSpecialUpFunc(SpecialKeyboardUpCB);
	glutReshapeFunc(WindowResizeCB);
}


void GLUTBackendInit(int argc, char** argv, bool withDepth, bool withStencil)
{
    //Initialize glut with whatever command line args are desired
    glutInit(&argc, argv);

    unsigned int displayMode = GLUT_DOUBLE|GLUT_RGBA;
    if(withDepth)
        displayMode |= GLUT_DEPTH;
    if(withStencil)
        displayMode |= GLUT_STENCIL;

    //Double buffering and RGBA color buffer
    glutInitDisplayMode(displayMode);

    //Return main loop if the window closes
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

bool GLUTBackendCreateWindow(unsigned int windowWidth, unsigned int windowHeight, bool isFullScreen, const char* title)
{
    //Basic Parameters
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(5, 5);

    glutCreateWindow(title);

    if(isFullScreen)
        glutFullScreen();

    glutInitContextVersion(3, 3);
    glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);

    //Must be initialized after glut
    GLenum res = glewInit();
    if(res != GLEW_OK)
    {
        std::cout << "Error: " << glewGetErrorString(res) << std::endl;
        return 1;
    }

    //glEnable(GL_LINE_SMOOTH);
	
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(1);

    printf("GL version: %s\n", glGetString(GL_VERSION));

    return true;
}

void GLUTBackendRun(ICallBacks* callBacks)
{
    if(!callBacks)
    {
        std::cout << "ICallback not initialized: GLUTBackendRun(ICallbacks* callBacks)" << std::endl;
        return;
    }

    InitializeGlutCallbacks();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    s_callBacks = callBacks;

    glutMainLoop();
}

void GLUTBackendSwapBuffers()
{
    glutSwapBuffers();
}

void GLUTBackendLeaveMainLoop()
{
    glutLeaveMainLoop();
}
