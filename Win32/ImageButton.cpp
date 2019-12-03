#include "ImageButton.h"

ImageButton::ImageButton()
{
}

ImageButton::ImageButton(Control* parent, std::string name, RECT rect)
	: Button(name, rect.right, rect.bottom)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	setLocation(rect.left, rect.top);
	mParent = parent;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY | SS_BITMAP;
	mType = WC_BUTTON;
	create();
}

ImageButton::ImageButton(Control* parent, std::string name, int width, int height)
	: Button(name, width, height)
{
	cmnControlInit(ICC_STANDARD_CLASSES);
	mParent = parent;
	mStyle = WS_CHILD | WS_VISIBLE | BS_NOTIFY | SS_BITMAP;
	mType = WC_BUTTON;
	create();
}

ImageButton::~ImageButton()
{
	if (mBitmap != nullptr)
	{
		DeleteObject(mBitmap);
		mBitmap = nullptr;
	}
}

void ImageButton::setBitmap(HBITMAP img)
{
	if (mBitmap != nullptr)
		DeleteObject(img);
	mBitmap = img;
	if (mCreated)
		SendMessage(mHwnd, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)img);
}
