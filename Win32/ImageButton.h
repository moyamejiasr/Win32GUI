#pragma once
#pragma warning (disable : 26495)

#include "Button.h"

class ImageButton : public Button
{
public:
	ImageButton();
	ImageButton(Control*, std::string, RECT);
	ImageButton(Control*, std::string, int = 0, int = 0);
	~ImageButton();

	void setBitmap(HBITMAP);
protected:
	HBITMAP mBitmap = nullptr;
};