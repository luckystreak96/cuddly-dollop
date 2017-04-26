#ifndef GLUTBACKEND_H__
#define GLUTBACKEND_H__

#include <Windows.h>
#include <iostream>
#include <GL/glew.h>
#include <gl/glext.h>
#include <gl/wglext.h>
#include <GL/freeglut.h>
#include "callbacks.h"

void GLUTBackendInit(int argc, char** argv, bool withDepth, bool withStencil);

bool GLUTBackendCreateWindow(unsigned int windowWidth, unsigned int windowHeight, bool isFullScreen, const char* title);

void GLUTBackendRun(ICallBacks* callBacks);

void GLUTBackendSwapBuffers();

void GLUTBackendLeaveMainLoop();

#endif
