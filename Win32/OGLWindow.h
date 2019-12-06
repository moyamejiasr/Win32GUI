#pragma once
#pragma warning (disable : 26495)

#pragma comment(lib, "opengl32.lib")

#include "Window.h"

class OGLWindow : public Window
{
public:
	OGLWindow();
	OGLWindow(std::string, int = 320, int = 240);
	OGLWindow(Control*, std::string, int = 0, int = 0);

	virtual void show();
	void swapBuffers();
	HDC hdc();
protected:
	bool mInitialized = false;
	PIXELFORMATDESCRIPTOR mPfd{ 0 };
	HDC mDc;
	HGLRC mGlrc;
};