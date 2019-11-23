#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

enum ProgressState
{
	Normal = PBST_NORMAL,
	Failed = PBST_ERROR,
	Paused = PBST_PAUSED
};

class ProgressBar : public Control
{
public:
	ProgressBar();
	ProgressBar(Control*, RECT);
	ProgressBar(Control*, int = 0, int = 0);

	void setRange(int, int);
	void setProgress(int);
	void setMarquee(int = 30);
	void setBarColor(COLORREF);
	void setState(ProgressState);
protected:
	int mMin = 0, mMax = 100;
	int mPosition = 0;
	COLORREF mColor = CLR_DEFAULT;
	ProgressState mState = Normal;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};