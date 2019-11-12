#include "Button.h"

Button::Button()
{
}

Button::Button(Control* parent, std::string name, int width, int height)
	: TextControl(name, width, height)
{
	mXMargin = 18; mYMargin = 12;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY;
	mParent = parent;
	mType = WC_BUTTON;
	create();
	autoSize();
}

void Button::setOnClick(f_onClick call)
{
	mOnClick = call;
}

void Button::setOnDoubleClick(f_onDoubleClick call)
{
	mOnDoubleClick = call;
}

Button::Button(std::string name, int width, int height)
	: TextControl(name, width, height)
{
}

void Button::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case BN_CLICKED:
		if (mOnClick != nullptr)
			mOnClick(this);
		break;
	case BN_DBLCLK:
		if (mOnDoubleClick != nullptr)
			mOnDoubleClick(this);
		break;
	}
}
