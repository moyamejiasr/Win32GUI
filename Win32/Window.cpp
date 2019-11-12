#include "Window.h"

Window::Window()
{
}

Window::Window(std::string name, int width, int height)
	: Control(name, width, height)
{
	mStyle = WS_OVERLAPPEDWINDOW;
	mBrush = GetSysColorBrush(COLOR_3DFACE);
	mType = mWndClass.lpszClassName;
	create();
}

Window::Window(Control* parent, std::string name, int width, int height)
{
	mStyle = WS_CHILD | WS_OVERLAPPEDWINDOW;
	mBrush = GetSysColorBrush(COLOR_3DFACE);
	mParent = parent;
	mType = mWndClass.lpszClassName;
	create();
}

bool Window::close()
{
	if (!mCloseable || (mOnClose != nullptr && !mOnClose(this)))
		return false;
#ifdef _DEBUG
	std::cout << "[*] Window " << mHwnd << " to be destroyed" << std::endl;
#endif
	DestroyWindow(mHwnd);
	return true;
}

void Window::minimize()
{
	if (mCreated)
		ShowWindow(mHwnd, SW_MINIMIZE);
}

void Window::maximize()
{
	if (mCreated)
		ShowWindow(mHwnd, SW_MAXIMIZE);
}

void Window::restore()
{
	if (mCreated)
		ShowWindow(mHwnd, SW_RESTORE);
}

void Window::setOnClose(f_onClose call)
{
	mOnClose = call;
}

void Window::setOnResize(f_onResize call)
{
	mOnResize = call;
}

void Window::setOnMove(f_onMove call)
{
	mOnMove = call;
}

void Window::setOnMouseWheel(f_onMouseWheel call)
{
	mOnMouseWheel = call;
}

void Window::setOnMouseClick(f_onMouseClick call)
{
	mOnMouseClick = call;
}

void Window::setOnFocus(f_onFocus call)
{
	mOnFocus = call;
}

void Window::setCloseable(bool state)
{
	mCloseable = state;
}

void Window::setTopMost(bool state)
{
	mTopMost = state;
	if (mCreated)
	{
		HWND flag = HWND_NOTOPMOST;
		if (mTopMost)
			flag = HWND_TOPMOST;
		SetWindowPos(mHwnd, flag, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void Window::setDraggable(bool state)
{
	mDraggable = state;
}

void Window::setFocusable(bool state)
{
	mFocusable = state;
	if (mCreated)
	{
		if (!mFocusable)
			appendFlagEx(WS_EX_NOACTIVATE);
		else
			removeFlagEx(WS_EX_NOACTIVATE);
	}
}

void Window::setAlpha(BYTE alpha)
{
	appendFlagEx(WS_EX_LAYERED);
	SetLayeredWindowAttributes(mHwnd, 0, alpha, LWA_ALPHA);
}

void Window::allowMaximize(bool state)
{
	mMaximizeBox = state;
	if (mCreated)
		if (mMaximizeBox)
			appendFlag(WS_MAXIMIZEBOX);
		else
			removeFlag(WS_MAXIMIZEBOX);
}

void Window::allowMinimize(bool state)
{
	mMinimizeBox = state;
	if (mCreated)
		if (mMinimizeBox)
			appendFlag(WS_MINIMIZEBOX);
		else
			removeFlag(WS_MINIMIZEBOX);
}

void Window::allowTitleBar(bool state)
{
	mTitleBar = state;
	if (mCreated)
		if (mTitleBar)
			appendFlag(WS_POPUPWINDOW);
		else
			removeFlag(WS_POPUPWINDOW);
}

void Window::allowResize(bool state)
{
	mResizable = state;
	if (mCreated)
		if (mResizable)
			appendFlag(WS_THICKFRAME);
		else
			removeFlag(WS_THICKFRAME);
}

void Window::setMinSize(int width, int height)
{
	mMinWidth = width;
	mMinHeight = height;
}

void Window::setMaxSize(int width, int height)
{
	mMaxWidth = width;
	mMaxHeight = height;
}

void Window::setMinMaxInfo(LPARAM& lParam)
{
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMinTrackSize.x = mMinWidth;
	lpMMI->ptMinTrackSize.y = mMinHeight;
	lpMMI->ptMaxTrackSize.x = mMaxWidth;
	lpMMI->ptMaxTrackSize.y = mMaxHeight;
}

void Window::callResize(LPARAM lParam)
{
	mClientWidth = LOWORD(lParam);
	mClientHeight = HIWORD(lParam);
	updateWindowRect();
	if (mOnResize != nullptr)
		mOnResize(this, mClientWidth, mClientHeight);
}

void Window::callMove(LPARAM lParam)
{
	mX = LOWORD(lParam);
	mY = HIWORD(lParam);
	if (mOnMove != nullptr)
		mOnMove(this, mX, mY);
}

void Window::callMouseWheel(WPARAM wParam)
{
	if (mOnMouseWheel != nullptr)
		mOnMouseWheel(this, HIWORD(wParam), LOWORD(wParam));
}

void Window::callMouseClick(WPARAM wParam, LPARAM lParam)
{
	if (mOnMouseClick != nullptr)
		mOnMouseClick(this, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
}

void Window::callFocusChange(WPARAM wParam)
{
	if (mOnFocus != nullptr)
		if (LOWORD(wParam) == WA_INACTIVE)
			mOnFocus(this, false);
		else
			mOnFocus(this, true);
}

void Window::callHoverChange(Control* child)
{
	if (mPrevHover == child)
		return;

	if (child->mOnHover != nullptr)
		child->mOnHover(child, true);
	if (mPrevHover->mOnHover != nullptr)
		mPrevHover->mOnHover(mPrevHover, false);

	mPrevHover = child;
}
