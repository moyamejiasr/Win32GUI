#pragma once
#pragma warning (disable : 26495)

#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define _AFXDLL
#include <SDKDDKVer.h>
#include <afxwin.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

class Control;
class Window;
class TextView;
class EditText;
class Button;
class CheckBox;

using f_onHover = void(*)(Control*, bool);

class Control
{
public:
	Control* child(int);
	bool create();
	void destroy();
	void show();
	void hide();
	void enable();
	void disable();
	void redraw();
	void setOnHover(f_onHover);
	void setVisibility(bool);
	void setEnabled(bool);
	void setLocation(int, int);
	void setSize(int, int);
	void setGlobalIcon(HICON);
	POINT getCursorPos();
	POINT getCursorScreenPos();
	HWND getHwnd();
	HICON getGlobalIcon();
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	int getClientWidth();
	int getClientHeight();
	virtual void setText(std::string);
	virtual std::string getText();
	bool isVisible();
	void join();
protected:
	friend class Window;
	virtual void execute(UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
	static thread_local std::string mClassName;
	static thread_local WNDCLASSEX mWndClass;
	static thread_local std::unordered_map<HWND, Control*> mControls;
	static thread_local HICON mIcon;
	f_onHover mOnHover;
	bool mCreated = false;
	bool mEnabled = true;
	std::string mType;
	std::string mText;
	DWORD mStyle;
	DWORD mExStyle;
	HCURSOR mCursor;
	HBRUSH mBrush = GetSysColorBrush(COLOR_3DFACE);
	int mX, mY;
	int mWidth, mHeight;
	int mClientWidth, mClientHeight;
	HWND mHwnd;
	HMENU mMenu;
	Control* mParent;
	std::vector<Control*> mChildrens;

	Control();
	Control(std::string, int, int);
	bool globalClassInit();
	void eraseWithChilds();
	void updateClientRect();
	void updateWindowRect();
	void appendFlag(DWORD);
	void removeFlag(DWORD);
	void appendFlagEx(DWORD);
	void removeFlagEx(DWORD);
	std::string pullWindowText();
};