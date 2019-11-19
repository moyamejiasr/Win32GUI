#include "PictureBox.h"

PictureBox::PictureBox()
{
}

PictureBox::PictureBox(Control* parent, std::string name, RECT rect)
	: Control(parent, name, rect.right, rect.bottom)
{
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY;
	mType = WC_STATIC;
	create();
}

PictureBox::PictureBox(Control* parent, std::string name, int width, int height)
	: Control(parent, name, width, height)
{
	mStyle = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY;
	mType = WC_STATIC;
	create();
}

void PictureBox::setOnClick(f_onClick call)
{
	mOnClick = call;
}

void PictureBox::setOnDoubleClick(f_onDoubleClick call)
{
	mOnDoubleClick = call;
}

LRESULT PictureBox::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case STN_CLICKED:
		if (mOnClick != nullptr)
			mOnClick(this);
		break;
	case STN_DBLCLK:
		if (mOnDoubleClick != nullptr)
			mOnDoubleClick(this);
		break;
	}
	return true;
}