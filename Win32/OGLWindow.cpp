#include "OGLWindow.h"

OGLWindow::OGLWindow()
{
}

OGLWindow::OGLWindow(std::string name, int width, int height)
	:Window(nullptr, name, CS_HREDRAW | CS_VREDRAW | CS_OWNDC, width, height)
{
	mStyle = WS_OVERLAPPEDWINDOW;
	create();
	mDc = GetDC(mHwnd);
}

OGLWindow::OGLWindow(Control* parent, std::string name, int width, int height)
	: Window(parent, name, CS_HREDRAW | CS_VREDRAW | CS_OWNDC, width, height)
{
	mStyle = WS_OVERLAPPEDWINDOW | WS_CHILD;
	create();
	mDc = GetDC(mHwnd);
}

void OGLWindow::show()
{
	Control::show();
	/* Context Initialization must be done after visible */
	if (!mInitialized)
	{
		mInitialized = true;
		mPfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		mPfd.nVersion = 1;
		mPfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW;
		mPfd.iPixelType = PFD_TYPE_RGBA;
		mPfd.cColorBits = 24;
		mPfd.cDepthBits = 32;

		int lPfId = ChoosePixelFormat(mDc, &mPfd);
		SetPixelFormat(mDc, lPfId, &mPfd);
		mGlrc = wglCreateContext(mDc);
		wglMakeCurrent(mDc, mGlrc);
	}
}

void OGLWindow::swapBuffers()
{
	SwapBuffers(mDc);
}

HDC OGLWindow::hdc()
{
	return mDc;
}
