#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

enum ScaleType
{
	FitPicture,
	FitControl
};

class PictureBox : public Control
{
public:
	PictureBox();
	PictureBox(Control*, HBITMAP, RECT);
	PictureBox(Control*, HBITMAP, int = 0, int = 0);

	void setOnClick(f_onClick);
	void setOnDoubleClick(f_onDoubleClick);
	virtual void setSize(int, int);
	virtual void setRect(RECT);
	void setBitmap(HBITMAP);
	void setScaleType(ScaleType);
protected:
	f_onClick mOnClick;
	f_onDoubleClick mOnDoubleClick;
	HBITMAP mBitmap = nullptr;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
	virtual LRESULT drawctl(UINT, WPARAM, LPARAM);
};