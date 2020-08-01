#include "PictureBox.h"

PictureBox::PictureBox()
{
}

PictureBox::PictureBox(Control* parent, HBITMAP img, RECT rect)
	: Control(parent, "PictureBox", rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY | SS_REALSIZECONTROL;
	mType = WC_STATIC;
	create();
	setBitmap(img);
}

PictureBox::PictureBox(Control* parent, HBITMAP img, int width, int height)
	: Control(parent, "PictureBox", width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY | SS_REALSIZECONTROL;
	mType = WC_STATIC;
	create();
	setBitmap(img);
}

void PictureBox::setOnClick(f_onClick call)
{
	mOnClick = call;
}

void PictureBox::setOnDoubleClick(f_onDoubleClick call)
{
	mOnDoubleClick = call;
}

void PictureBox::setSize(int width, int height)
{
	Control::setSize(width, height);
	// Fix to the static resize garbage creation
	redraw();
}

void PictureBox::setRect(RECT rect)
{
	Control::setRect(rect);
	// Fix to the static resize garbage creation
	redraw();
}

void PictureBox::setBitmap(HBITMAP img)
{
	if (mBitmap != nullptr)
		DeleteObject(mBitmap);
	mBitmap = img;
	if (mCreated)
		SendMessage(mHwnd, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)mBitmap);
}

void PictureBox::setScaleType(ScaleType st)
{
	if (st == ScaleType::FitPicture)
	{
		removeFlag(SS_REALSIZECONTROL);
		appendFlag(SS_CENTERIMAGE);
	}
	else
	{
		removeFlag(SS_CENTERIMAGE);
		appendFlag(SS_REALSIZECONTROL);
	}
}

LRESULT PictureBox::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
	case STN_CLICKED:
		if (mOnClick != nullptr)
			mOnClick(this, MouseKeys::LeftButton);
		break;
	case STN_DBLCLK:
		if (mOnDoubleClick != nullptr)
			mOnDoubleClick(this, MouseKeys::LeftButton);
		break;
	}
	return true;
}

LRESULT PictureBox::drawctl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetBkMode((HDC)wParam, TRANSPARENT);
	SetTextColor((HDC)wParam, mFtColor);
	return (LRESULT)mBkBrush;
}
