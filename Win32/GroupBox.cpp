#include "GroupBox.h"

GroupBox::GroupBox()
{
}

GroupBox::GroupBox(Control* parent, std::string name, RECT rect)
	: Control(parent, name, rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_NOTIFY;
	mType = WC_BUTTON;
	create();
}

GroupBox::GroupBox(Control* parent, std::string name, int width, int height)
	: Control(parent, name, width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE | BS_GROUPBOX | BS_NOTIFY;
	mType = WC_BUTTON;
	create();
}

LRESULT GroupBox::drawctl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetBkMode((HDC)wParam, TRANSPARENT);
	SetTextColor((HDC)wParam, mFtColor);
	return (LRESULT)mBkBrush;
}
