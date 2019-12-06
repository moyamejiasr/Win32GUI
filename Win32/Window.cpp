#include "Window.h"

Window::Window()
{
}

Window::Window(std::string name, int width, int height)
	: Control(nullptr, name, width, height)
{
	mStyle = WS_OVERLAPPEDWINDOW;
	mType = mWndClass.lpszClassName;
	create();
}

Window::Window(Control* parent, std::string name, int width, int height)
	: Control(parent, name, width, height)
{
	mStyle = WS_CHILD | WS_OVERLAPPEDWINDOW;
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

void Window::redrawMenu()
{
	DrawMenuBar(mHwnd);
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
		appendFlag(WS_OVERLAPPEDWINDOW);
	else
		removeFlag(WS_OVERLAPPEDWINDOW);
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

void Window::setMenu(HMENU ctl)
{
	mMenuBar = ctl;
	SetMenu(mHwnd, mMenuBar);

}

Window::Window(Control* parent, std::string name, DWORD flags, int width, int height)
	:Control(parent, flags, name, width, height)
{
	mType = mWndClass.lpszClassName;
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
	mClientRect.right = LOWORD(lParam);
	mClientRect.bottom = HIWORD(lParam);
	updateWindowRect();
	if (mOnResize != nullptr)
		mOnResize(this, mClientRect.right, mClientRect.bottom);
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

void Window::callMouseClick(DWORD wParam, LPARAM lParam)
{
	if (wParam < WM_LBUTTONDOWN)
		return; // Discard child Windows WM_CREATE messages

	if (mOnMouseClick != nullptr) // TODO CHECK wParam types
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
	case WM_CONTEXTMENU:
		if (ctlExists(wParam))
		{
			mControls[(HWND)wParam]->showContextMenu(mHwnd);
			mPrevHover = mControls[(HWND)wParam]; // Set menu caller
		}
		break;
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
		callMouseClick(wParam, lParam);
		break;
	case WM_LBUTTONDOWN: /* Window left cursor button click */
	case WM_RBUTTONDOWN: /* Window right cursor button click */
	case WM_MBUTTONDOWN: /* Window middle cursor button click */
	case WM_XBUTTONDOWN: /* Window extra cursor button click */
		callMouseClick(uMsg, lParam);
		break;
	case WM_ACTIVATE: /* Window focus state changed */
		callFocusChange(wParam);
		break;
	case WM_SETCURSOR: /* Window's control hover change detect by cursor */
		if (ctlExists(wParam))
			callHoverChange(mControls[(HWND)wParam]);
		break;
	case WM_COMMAND: /* Execute specific child command */
		if (HIWORD(wParam) == 0 && lParam == 0) // If Menu Click
		{
			if (mPrevHover && mPrevHover->mOnMenuClick) // Check if callback present
			{
				mPrevHover->mOnMenuClick(mPrevHover, LOWORD(wParam));
			}
		}
		else if (ctlExists(lParam)) // Otherwise command
			return mControls[(HWND)lParam]->execute(uMsg, wParam, lParam);
		break;
	case WM_NOTIFY: /* Extenden commands */
		if (ctlExists((DWORD)((LPNMHDR)lParam)->hwndFrom))
			return mControls[((LPNMHDR)lParam)->hwndFrom]->cnotify(uMsg, wParam, lParam);
		break;
	case WM_VSCROLL: /* On TrackBar Scroll set */
	case WM_HSCROLL: /* On TrackBar Scroll set */
		if (ctlExists(lParam))
			return mControls[(HWND)lParam]->execute(uMsg, wParam, lParam);
		break;
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORLISTBOX:
		if (ctlExists(lParam))
			return mControls[(HWND)lParam]->drawctl(uMsg, wParam, lParam);
		break;
	case WM_ERASEBKGND:
		if (mBkBrush)
		{
			RECT rect;
			GetClientRect(mHwnd, &rect);
			FillRect((HDC)(wParam), &rect, mBkBrush);
			return TRUE;
		}
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
		eraseWithChilds(this);
		if (mControls.size() == 0)
		{
			PostQuitMessage(0);
			mLoopAlive = false;
		}
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}