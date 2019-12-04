#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

using f_onClose = bool(*)(Window*);
using f_onResize = void(*)(Window*, int, int);
using f_onMove = void(*)(Window*, int, int);
using f_onMouseWheel = void(*)(Window*, int, int);
using f_onMouseClick = void(*)(Window*, int, int, int);

class Window : public Control
{
public:
	Window();
	Window(std::string, int = 320, int = 240);
	Window(Control*, std::string, int = 0, int = 0);

	bool close();
	void minimize();
	void maximize();
	void restore();
	void redrawMenu();
	void setOnClose(f_onClose);
	void setOnResize(f_onResize);
	void setOnMove(f_onMove);
	void setOnMouseWheel(f_onMouseWheel);
	void setOnMouseClick(f_onMouseClick);
	void setOnFocus(f_onFocus);
	void setCloseable(bool);
	void setTopMost(bool);
	void setDraggable(bool);
	void setFocusable(bool);
	void setAlpha(BYTE);
	void allowResize(bool);
	void allowMaximize(bool);
	void allowMinimize(bool);
	void allowTitleBar(bool);
	void setMinSize(int, int);
	void setMaxSize(int, int);
	void setMenu(HMENU);
protected:
	Window(Control*, std::string, DWORD, int = 320, int = 240);
	f_onClose mOnClose = nullptr;
	f_onResize mOnResize = nullptr;
	f_onMove mOnMove = nullptr;
	f_onMouseWheel mOnMouseWheel = nullptr;
	f_onMouseClick mOnMouseClick = nullptr;
	f_onFocus mOnFocus = nullptr;
	bool mCloseable = true;
	bool mTopMost = false;
	bool mDraggable = false;
	DWORD mMinWidth = 80, mMinHeight = 120;
	DWORD mMaxWidth = 1920, mMaxHeight = 1080;
	HMENU mMenuBar = nullptr;
	Control* mPrevHover = this;

	void setMinMaxInfo(LPARAM&);
	void callResize(LPARAM);
	void callMove(LPARAM);
	void callMouseWheel(WPARAM);
	void callMouseClick(DWORD, LPARAM);
	void callFocusChange(WPARAM);
	void callHoverChange(Control*);
	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};