#pragma once
#pragma warning (disable : 26495)

#ifndef _LEGACY
#pragma comment(linker, "\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#define _AFXDLL
#include <SDKDDKVer.h>
#include <afxwin.h>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "MenuBuilder.h"

class Control;
class Window;
class TextView;
class EditText;
class MultiEditText;
class Button;
class CheckBox;

using f_onHover = void(*)(Control*, bool);
using f_onClick = void(*)(Control*);
using f_onDoubleClick = void(*)(Control*);

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
	void setVisibile(bool);
	void setEnabled(bool);
	void setLocation(int, int);
	void setGlobalIcon(HICON);
	void setContextMenu(ContextMenu*);
	POINT getCursorPos();
	POINT getCursorScreenPos();
	HWND getHWnd();
	HICON getGlobalIcon();
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	int getClientWidth();
	int getClientHeight();
	RECT getRect();
	RECT getClientRect();
	bool hasFlag(DWORD);
	void appendFlag(DWORD);
	void removeFlag(DWORD);
	bool hasFlagEx(DWORD);
	void appendFlagEx(DWORD);
	void removeFlagEx(DWORD);
	virtual void setText(std::string);
	virtual void setSize(int, int);
	virtual void setRect(RECT);
	virtual std::string getText();
	bool isVisible();
	void join();
protected:
	friend class Window;
	virtual LRESULT execute(UINT, WPARAM, LPARAM);
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
	DWORD mStyle = NULL;
	DWORD mExStyle = NULL;
	HCURSOR mCursor;
	HBRUSH mBrush = GetSysColorBrush(COLOR_3DFACE);
	int mX, mY;
	int mWidth, mHeight;
	int mClientWidth, mClientHeight;
	HWND mHwnd;
	HMENU mId = NULL;
	HMENU mContextMenu = NULL;
	Control* mParent = nullptr;
	std::vector<Control*> mChildrens;

	Control();
	Control(std::string, int, int);
	bool globalClassInit();
	void eraseWithChilds();
	void updateClientRect();
	void updateWindowRect();
	void showContextMenu();
	std::string pullWindowText();
};