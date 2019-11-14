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
	if (!state)
		appendFlagEx(WS_EX_NOACTIVATE);
	else
		removeFlagEx(WS_EX_NOACTIVATE);
}

void Window::setAlpha(BYTE alpha)
{
	appendFlagEx(WS_EX_LAYERED);
	SetLayeredWindowAttributes(mHwnd, 0, alpha, LWA_ALPHA);
}

void Window::allowMaximize(bool state)
{
	if (state)
		appendFlag(WS_MAXIMIZEBOX);
	else
		removeFlag(WS_MAXIMIZEBOX);
}

void Window::allowMinimize(bool state)
{
	if (state)
		appendFlag(WS_MINIMIZEBOX);
	else
		removeFlag(WS_MINIMIZEBOX);
}

void Window::allowTitleBar(bool state)
{
	if (state)
		appendFlag(WS_POPUPWINDOW);
	else
		removeFlag(WS_POPUPWINDOW);
}

void Window::allowResize(bool state)
{
	if (state)
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

void Window::setMenuBar(ControlMenu* ctl)
{
	if (mMenuBar != nullptr)
	{
		DestroyMenu(mMenuBar);
		mMenuBar = nullptr;
	}
	if (ctl != nullptr)
		mMenuBar = ctl->getHMenu();
	SetMenu(mHwnd, mMenuBar);

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
	if (wParam == MK_RBUTTON)
		mPrevHover->showContextMenu();

	if (mOnMouseClick != nullptr)
		mOnMouseClick(this, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
}

void Window::callChildMouseClick(WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == WM_RBUTTONDOWN)
		mPrevHover->showContextMenu();

	if (mOnMouseClick != nullptr)
		switch (LOWORD(wParam))
		{
		case WM_LBUTTONDOWN:
			mOnMouseClick(this, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MK_LBUTTON);
			break;
		case WM_MBUTTONDOWN:
			mOnMouseClick(this, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MK_MBUTTON);
			break;
		case WM_RBUTTONDOWN:
			mOnMouseClick(this, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), MK_RBUTTON);
			break;
		}
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
	if (mPrevHover->mHwnd == child->mHwnd)
		return;

	if (child->mOnHover != nullptr)
		child->mOnHover(child, true);
	if (mPrevHover->mOnHover != nullptr)
		mPrevHover->mOnHover(mPrevHover, false);

	mPrevHover = child;
}

LRESULT Window::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_GETMINMAXINFO: /* Window Size Limits */
		setMinMaxInfo(lParam);
		break;
	case WM_SIZE: /* Window Size Changes */
		callResize(lParam);
		break;
	case WM_MOVE: /* Window moved on screen */
		callMove(lParam);
		break;
	case WM_MOUSEWHEEL: /* Window mouse wheel state changed */
		callMouseWheel(wParam);
		break;
	case WM_PARENTNOTIFY: /* Notify Window of child click actions */
		callChildMouseClick(wParam, lParam);
		break;
	case WM_LBUTTONDOWN: /* Window left cursor button click */
	case WM_RBUTTONDOWN: /* Window right cursor button click */
	case WM_MBUTTONDOWN: /* Window middle cursor button click */
		callMouseClick(wParam, lParam);
		break;
	case WM_ACTIVATE: /* Window focus state changed */
		callFocusChange(wParam);
		break;
	case WM_SETCURSOR: /* Window's control hover change detect by cursor */
		callHoverChange(mControls[(HWND)wParam]);
		break;
	case WM_COMMAND: /* Execute specific child command */
		if (mControls[(HWND)lParam] != nullptr)
			mControls[(HWND)lParam]->execute(uMsg, wParam, lParam);
		break;
	case WM_NCHITTEST: /* Handle possible child window dragNdrop */
		if (mDraggable)
		{
			LRESULT hit = DefWindowProc(mHwnd, uMsg, wParam, lParam);
			if (GetAsyncKeyState(VK_LBUTTON) & VK_LBUTTON && hit == HTCLIENT)
				return HTCAPTION;
		}
		break; /* Handle possible close message */
	case WM_CLOSE:
		close();
		return false;
	case WM_DESTROY: /* Window Execute close attempts */
		eraseWithChilds();
		if (mControls.size() == 0)
			PostQuitMessage(0);
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}