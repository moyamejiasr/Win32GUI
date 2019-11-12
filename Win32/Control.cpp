#include "Control.h"

thread_local WNDCLASSEX Control::mWndClass{};

thread_local std::string Control::mClassName;

thread_local std::unordered_map<HWND, Control*> Control::mControls;

thread_local HICON Control::mIcon = LoadIcon(NULL, IDI_APPLICATION);

Control* Control::child(int id)
{
	return mChildrens[id];
}

void Control::show()
{
	setVisibility(true);
}

void Control::hide()
{
	setVisibility(false);
}

void Control::enable()
{
	setEnabled(true);
}

void Control::disable()
{
	setEnabled(false);
}

void Control::redraw()
{
	if (mCreated)
		InvalidateRect(mHwnd, NULL, TRUE);
}

void Control::setOnHover(f_onHover call)
{
	mOnHover = call;
}

void Control::setVisibility(bool state)
{
	if (state)
	{
		mStyle = mStyle | WS_VISIBLE;
		if (mCreated)
			ShowWindow(mHwnd, SW_SHOW);
	}
	else
	{
		mStyle = mStyle & ~WS_VISIBLE;
		if (mCreated)
			ShowWindow(mHwnd, SW_HIDE);
	}

	
}

void Control::setEnabled(bool state)
{
	mEnabled = state;
	if (mCreated)
	{
		EnableWindow(mHwnd, mEnabled);
		for (auto const& child : mChildrens)
			EnableWindow(child->mHwnd, mEnabled);
	}
}

void Control::setLocation(int x, int y)
{
	mX = x;
	mY = y;

	if (mCreated)
	{
		SetWindowPos(mHwnd, NULL, x, y, 0, 0, SWP_NOSIZE);
		updateClientRect();
	}
}

void Control::setSize(int width, int height)
{
	mWidth = width;
	mHeight = height;

	if (mCreated)
	{
		SetWindowPos(mHwnd, NULL, 0, 0, width, height, SWP_NOMOVE);
		updateClientRect();
	}
}

void Control::setGlobalIcon(HICON icon)
{
	if (mIcon != nullptr)
		DestroyIcon(mIcon);
	mIcon = icon;

	if (mCreated)
		SetClassLong(mHwnd, GCL_HICON, (LONG)mIcon);
}

POINT Control::getCursorPos()
{
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(mHwnd, &p);
	return p;
}

POINT Control::getCursorScreenPos()
{
	POINT p;
	GetCursorPos(&p);
	return p;
}

HWND Control::getHwnd()
{
	return mHwnd;
}

HICON Control::getGlobalIcon()
{
	return mIcon;
}

int Control::getX()
{
	return mX;
}

int Control::getY()
{
	return mY;
}

int Control::getWidth()
{
	return mWidth;
}

int Control::getHeight()
{
	return mHeight;
}

int Control::getClientWidth()
{
	return mClientWidth;
}

int Control::getClientHeight()
{
	return mClientHeight;
}

void Control::setText(std::string txt)
{
	mText = txt;
	if (mCreated) {
		SetWindowText(mHwnd, txt.c_str());
	}
}

std::string Control::getText()
{
	return mText;
}

bool Control::isVisible()
{
	return mStyle & WS_VISIBLE;
}

void Control::join()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

Control::Control()
{
	globalClassInit();
}

Control::Control(std::string name, int width, int height)
{
	globalClassInit();
	mX = CW_USEDEFAULT;
	setText(name);
	setSize(width, height);
}

bool Control::create()
{
	if (mCreated)
		return true;

	mHwnd = CreateWindow(
		mType.c_str(),
		mText.c_str(),
		mStyle,
		mX, mY,
		mWidth, mHeight,
		mParent != nullptr ? mParent->mHwnd : NULL,
		mParent != nullptr ? (HMENU)mParent->mChildrens.size() : NULL,
		mWndClass.hInstance,
		NULL
	);
	if (mHwnd != NULL)
	{
		mCreated = true;
		mControls[mHwnd] = this;
		updateClientRect();
		if (mParent != nullptr)
		{
			mMenu = (HMENU)mParent->mChildrens.size();
			mParent->mChildrens.push_back(this);
		}
	}

#ifdef _DEBUG
	if (mHwnd != NULL)
		std::cout << "[*] Window " << mText << " created." << std::endl;
	else
		std::cout << "[*] Unable to create window " << mText << std::endl;
#endif
	return mHwnd != NULL;
}

void Control::destroy()
{
	if (!mCreated)
		return;
	mCreated = DestroyWindow(mHwnd);
}

bool Control::globalClassInit()
{
	if (!mWndClass.hInstance)
	{
		// Create unique class name
		mClassName = std::to_string(GetCurrentThreadId());
		// Set class data
		mWndClass.cbSize = sizeof(mWndClass);
		mWndClass.lpfnWndProc = WndProc;
		mWndClass.hInstance = GetModuleHandle(NULL);
		mWndClass.lpszClassName = mClassName.c_str();
		mWndClass.hIcon = mIcon;
		mWndClass.hbrBackground = mBrush;
		mWndClass.hCursor = LoadCursor(0, IDC_ARROW);
		if (!RegisterClassEx(&mWndClass))
		{
#ifdef _DEBUG
			std::cout << "[*] Unable to register WndClass" << std::endl;
#endif
			return false;
		}
#ifdef _DEBUG
		std::cout << "[*] WndClass registered successfully" << std::endl;
#endif
	}
#ifdef _DEBUG
	else std::cout << "[*] WndClass already registered" << std::endl;
#endif
	return true;
}

void Control::eraseWithChilds()
{
	for (auto const& child : mChildrens)
		mControls.erase(child->mHwnd);
	mControls.erase(mHwnd);
}

void Control::updateClientRect()
{
	RECT rect;
	if (GetClientRect(mHwnd, &rect))
	{
		mClientWidth = rect.right; 
		mClientHeight = rect.bottom;
	}
}

void Control::updateWindowRect()
{
	RECT rect;
	if (GetWindowRect(mHwnd, &rect))
	{
		mX = rect.left;
		mY = rect.top;
		mWidth = rect.right - rect.left;
		mHeight = rect.bottom - rect.top;
	}
}

void Control::appendFlag(DWORD flag)
{
	DWORD flags = GetWindowLong(mHwnd, GWL_STYLE);
	SetWindowLong(mHwnd, GWL_STYLE, flags | flag);
}

void Control::removeFlag(DWORD flag)
{
	DWORD flags = GetWindowLong(mHwnd, GWL_STYLE);
	SetWindowLong(mHwnd, GWL_STYLE, flags & ~flag);
}

void Control::appendFlagEx(DWORD flag)
{
	DWORD flags = GetWindowLong(mHwnd, GWL_EXSTYLE);
	SetWindowLong(mHwnd, GWL_EXSTYLE, flags | flag);
}

void Control::removeFlagEx(DWORD flag)
{
	DWORD flags = GetWindowLong(mHwnd, GWL_EXSTYLE);
	SetWindowLong(mHwnd, GWL_EXSTYLE, flags & ~flag);
}

std::string Control::pullWindowText()
{
	int len = GetWindowTextLength(mHwnd) + 1;
	TCHAR* buff = new TCHAR[len];
	GetWindowText(mHwnd, buff, len);
	std::string result = buff;
	delete[] buff;
	return result;
}

#include "Window.h"
#include "TextView.h"
#include "EditText.h"

void Control::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG
	std::cout << "undefined exe for " << lParam << std::endl;
#endif
}

LRESULT CALLBACK Control::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef Win32MSG
	std::cout << "MSG " << uMsg << std::endl;
#endif
	switch (uMsg)
	{
	/* Window Size Limits */
	case WM_GETMINMAXINFO:
		if (mControls[hwnd] != nullptr)
			((Window*)mControls[hwnd])->setMinMaxInfo(lParam);
		break;
	/* Window Size Changes */
	case WM_SIZE:
		if (mControls[hwnd] != nullptr)
			((Window*)mControls[hwnd])->callResize(lParam);
		break;
	/* Window moved on screen */
	case WM_MOVE:
		if (mControls[hwnd] != nullptr)
			((Window*)mControls[hwnd])->callMove(lParam);
		break;
	/* Window mouse wheel state changed */
	case WM_MOUSEWHEEL:
		if (mControls[hwnd] != nullptr)
			((Window*)mControls[hwnd])->callMouseWheel(wParam);
		break;
	/* Window cursor button click */
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		if (mControls[hwnd] != nullptr)
			((Window*)mControls[hwnd])->callMouseClick(wParam, lParam);
		break;
	/* Window focus state changed */
	case WM_ACTIVATE:
		if (mControls[hwnd] != nullptr)
			((Window*)mControls[hwnd])->callFocusChange(wParam);
		break;
	/* Window's control hover change detect by cursor */
	case WM_SETCURSOR:
		if (mControls[hwnd] != nullptr)
			((Window*)mControls[hwnd])->callHoverChange(mControls[(HWND)wParam]);
		break;
	/* Define hit area (good for drag&drop) */
	case WM_NCHITTEST:
		if (mControls[hwnd] != nullptr && ((Window*)mControls[hwnd])->mDraggable)
		{
			LRESULT hit = DefWindowProc(hwnd, uMsg, wParam, lParam);
			if (GetAsyncKeyState(VK_LBUTTON) & VK_LBUTTON && hit == HTCLIENT)
				return HTCAPTION;
		}
		break;
	case WM_COMMAND:
		if (mControls[(HWND)lParam] != nullptr)
			mControls[(HWND)lParam]->execute(uMsg, wParam, lParam);
		break;
	/* Window close attempts */
	case WM_CLOSE:
		if (mControls[hwnd] != nullptr)
			((Window*)mControls[hwnd])->close();
		return false;
	/* Window Execute close attempts */
	case WM_DESTROY:
		if (mControls[hwnd] != nullptr)
		{
			mControls[hwnd]->eraseWithChilds();
			if (mControls.size() == 0)
				PostQuitMessage(0);
		}
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}