#include "OGLFrame.h"

OGLFrame::OGLFrame()
{
}

OGLFrame::OGLFrame(Control* parent, RECT rect)
	: Control(parent, "", rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE | CS_OWNDC | CS_VREDRAW | CS_HREDRAW | SS_NOTIFY;
	mType = WC_STATIC;
	create();
	initGL();
}

OGLFrame::OGLFrame(Control* parent, int width, int height)
	: Control(parent, "", width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE | CS_OWNDC | CS_VREDRAW | CS_HREDRAW | SS_NOTIFY;
	mType = WC_STATIC;
	create();
	initGL();
}

void OGLFrame::swapb()
{
	SwapBuffers(mDC);
}

void OGLFrame::setOnClick(f_onClick call)
{
	mOnClick = call;
}

void OGLFrame::setOnDoubleClick(f_onDoubleClick call)
{
	mOnDoubleClick = call;
}

void OGLFrame::initGL()
{
	mDC = GetDC(mHwnd);
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.cStencilBits = 8;

	SetPixelFormat(mDC, 1, &pfd);
	mGlrc = wglCreateContext(mDC);
	wglMakeCurrent(mDC, mGlrc);
}

LRESULT OGLFrame::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case STN_CLICKED:
		if (mOnClick != nullptr)
			mOnClick(this);
		break;
	case STN_DBLCLK:
		if (mOnDoubleClick != nullptr)
			mOnDoubleClick(this);
		break;
	}
	return true;
}