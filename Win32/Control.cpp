#include "Control.h"

thread_local WNDCLASSEX Control::mWndClass{};
thread_local std::string Control::mClassName;
thread_local bool Control::mLoopAlive = true;
thread_local std::unordered_map<HWND, Control*> Control::mControls;
thread_local int Control::mLapse = 16;
thread_local HICON Control::mIcon = LoadIcon(NULL, IDI_APPLICATION);
thread_local f_onRender Control::mOnRender = nullptr;

Control* Control::child(int id)
{
	return mChildrens[id];
}

void Control::show()
{
	setVisibile(true);
}

void Control::hide()
{
	setVisibile(false);
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

void Control::setGlobalSleepTime(int value)
{
	mLapse = value;
}

void Control::setOnRender(f_onRender call)
{
	mOnRender = call;
}

void Control::setOnHover(f_onHover call)
{
	mOnHover = call;
}

void Control::setOnMenuClick(f_onMenuClick call)
{
	mOnMenuClick = call;
}

void Control::setVisibile(bool state)
{
	// Visibility has bugs when done with
	// flag changes instead of ShowWindow
	if (state)
		mStyle = mStyle | WS_VISIBLE;
	else
		mStyle = mStyle & ~WS_VISIBLE;

	if (mCreated)
		ShowWindow(mHwnd, state ? SW_SHOW : SW_HIDE);
	
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

void Control::setRect(RECT rect)
{
	mX = rect.left;
	mY = rect.top;
	mWidth = rect.right;
	mHeight = rect.bottom;

	if (mCreated)
	{
		SetWindowPos(mHwnd, NULL, mX, mY, mWidth, mHeight, NULL);
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

void Control::setContextMenu(HMENU ctl)
{
	mContextMenu = ctl;
}

void Control::setTextSize(int size)
{
	mLogFont.lfHeight = size;
	updateFont();
}

void Control::setItalic(bool state)
{
	mLogFont.lfItalic = state;
	updateFont();
}

void Control::setUnderline(bool state)
{
	mLogFont.lfUnderline = state;
	updateFont();
}

void Control::setStrikeout(bool state)
{
	mLogFont.lfStrikeOut = state;
	updateFont();
}

void Control::setFont(std::string str)
{
	int nLen = 32 > str.size() ? 31 : str.size();
	std::copy(str.begin(), str.end(), mLogFont.lfFaceName);
	mLogFont.lfFaceName[nLen] = '\0';
}

void Control::setTextColor(COLORREF value)
{
	mFtColor = value;
}

void Control::setBackground(HBRUSH brush)
{
	mBkBrush = brush;
}

void Control::setOldStyle(bool state)
{
	mOldStyle = state;
	if (!mCreated)
		return;
	if (mOldStyle)
		SetWindowTheme(mHwnd, L" ", L" ");
	else
		SetWindowTheme(mHwnd, L"Explorer", NULL);
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

HWND Control::hwnd()
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

RECT Control::getRect()
{
	return RECT{ 
		mX, mY, 
		mWidth, mHeight 
	};
}

RECT Control::getClientRect()
{
	return RECT{
		0, 0,
		mClientWidth, mClientHeight
	};
}

bool Control::hasFlag(DWORD flag)
{
	return mStyle & flag;
}

void Control::appendFlag(DWORD flag)
{
	mStyle |= flag;
	if (mCreated)
		SetWindowLong(mHwnd, GWL_STYLE, mStyle);
}

void Control::removeFlag(DWORD flag)
{
	mStyle &= ~flag;
	if (mCreated)
		SetWindowLong(mHwnd, GWL_STYLE, mStyle);
}

bool Control::hasFlagEx(DWORD flag)
{
	return mExStyle & flag;
}

void Control::appendFlagEx(DWORD flag)
{
	mExStyle |= flag;
	if (mCreated)
		SetWindowLong(mHwnd, GWL_EXSTYLE, mExStyle);
}

void Control::removeFlagEx(DWORD flag)
{
	mExStyle &= ~flag;
	if (mCreated)
		SetWindowLong(mHwnd, GWL_EXSTYLE, mExStyle);
}

void Control::setText(std::string txt)
{
	mText = txt;
	if (mCreated)
		SetWindowText(mHwnd, mText.c_str());
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

void Control::ljoin()
{
	MSG msg;
	std::chrono::time_point<std::chrono::system_clock> t =
		std::chrono::system_clock::now();
	while (mLoopAlive)
	{
		t += std::chrono::milliseconds(mLapse);
		std::this_thread::sleep_until(t);
		
		if (mOnRender)
			mOnRender(t.time_since_epoch().count());

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

Control::Control()
{
}

Control::Control(Control* parent, std::string name, int width, int height)
{
	globalClassInit();
	mBkBrush = mWndClass.hbrBackground;
	mParent = parent;
	setText(name);
	setSize(width, height);
}

Control::Control(Control* parent, DWORD flags, std::string name, int width, int height)
{
	globalClassInit(flags);
	mBkBrush = mWndClass.hbrBackground;
	mParent = parent;
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
		updateFont();
		updateClientRect();
		if (mParent != nullptr)
		{
			mId = (HMENU)mParent->mChildrens.size();
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

bool Control::globalClassInit(DWORD flags)
{
	NONCLIENTMETRICS metrics{};
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
	mLogFont = metrics.lfMessageFont;

	if (mWndClass.hInstance)
		return true;
	// Create unique class name
	mClassName = std::to_string(GetCurrentThreadId());
	// Set class data
	mWndClass.style = flags;
	mWndClass.cbSize = sizeof(mWndClass);
	mWndClass.lpfnWndProc = WndProc;
	mWndClass.hInstance = GetModuleHandle(NULL);
	mWndClass.lpszClassName = mClassName.c_str();
	mWndClass.hIcon = mIcon;
	mWndClass.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	mWndClass.hCursor = LoadCursor(0, IDC_ARROW);
	if (!RegisterClassEx(&mWndClass))
	{
#ifdef _DEBUG
		std::cout << "[*] Unable to register WndClass" << std::endl;
#endif
		exit(-1);
	}
#ifdef _DEBUG
	std::cout << "[*] WndClass registered successfully" << std::endl;
#endif
	return true;
}

bool Control::cmnControlInit(DWORD flag)
{
	INITCOMMONCONTROLSEX icex{};
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = flag;
	return InitCommonControlsEx(&icex);
}

void Control::eraseWithChilds()
{
	for (auto const& child : mChildrens)
		mControls.erase(child->mHwnd);
	mControls.erase(mHwnd);
}

bool Control::ctlExists(DWORD param)
{
	return mControls.find((HWND)param) != mControls.end();
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

void Control::updateFont()
{
	if (mFont != nullptr)
		DeleteObject(mFont);
	mFont = CreateFontIndirect(&mLogFont);

	if (mCreated) {
		SendMessage(mHwnd, WM_SETFONT, (WPARAM)mFont, TRUE);
		redraw();
	}
}

void Control::showContextMenu(HWND window)
{
	if (mContextMenu != nullptr)
	{
		POINT p = getCursorScreenPos();
		TrackPopupMenu(mContextMenu, NULL, p.x, p.y, NULL, window, NULL);
	}
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

LRESULT Control::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

LRESULT Control::drawctl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)DefWindowProc(mHwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Control::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef _DEBUG_MSG
	std::cout << uMsg << std::endl;
#endif
	if (mControls.find(hwnd) != mControls.end())
		return mControls[hwnd]->execute(uMsg, wParam, lParam);
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}