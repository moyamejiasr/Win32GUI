#pragma once
#pragma warning (disable : 26495)
#pragma comment(lib, "Opengl32.lib")

#include "Control.h"

class OGLFrame : public Control
{
public:
	OGLFrame();
	OGLFrame(Control*, RECT);
	OGLFrame(Control*, int = 0, int = 0);

	void swapb();
	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
protected:
	f_onClick mOnClick;
	f_onDoubleClick mOnDoubleClick;
	HDC mDC = nullptr;
	HGLRC mGlrc = nullptr;

	void initGL();
	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};