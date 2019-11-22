#include "ProgressBar.h"

ProgressBar::ProgressBar()
{
}

ProgressBar::ProgressBar(Control* parent, RECT rect)
	: Control(parent, "", rect.right, rect.bottom)
{
	cmnControlInit(ICC_PROGRESS_CLASS);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE;
	mType = PROGRESS_CLASS;
	create();
	setRange(mMin, mMax);
	setProgress(mPosition);
	setState(mState);
	setBarColor(mColor);
}

ProgressBar::ProgressBar(Control* parent, int width, int height)
	: Control(parent, "", width, height)
{
	cmnControlInit(ICC_PROGRESS_CLASS);
	mStyle = WS_CHILD | WS_VISIBLE;
	mType = PROGRESS_CLASS;
	create();
	setRange(mMin, mMax);
	setProgress(mPosition);
	setState(mState);
	setBarColor(mColor);
}

void ProgressBar::setRange(int vmin, int vmax)
{
	mMin = vmin;
	mMax = vmax;
	if (mCreated)
		SendMessage(mHwnd, PBM_SETRANGE, 0, MAKELPARAM(mMin, mMax));
}

void ProgressBar::setProgress(int val)
{
	mPosition = val;
	if (mCreated)
		SendMessage(mHwnd, PBM_SETPOS, (WPARAM)mPosition, 0);
}

void ProgressBar::setMarquee(int val)
{
	if (mCreated)
	{
		if (val)
			appendFlag(PBS_MARQUEE);
		else
			removeFlag(PBS_MARQUEE);
		SendMessage(mHwnd, PBM_SETMARQUEE, val, val);
	}
}

void ProgressBar::setBarColor(COLORREF color)
{
	mColor = color;
	if (mCreated)
		SendMessage(mHwnd, PBM_SETBARCOLOR, 0, mColor);
}

void ProgressBar::setState(ProgressState state)
{
	mState = state;
	if (mCreated)
		SendMessage(mHwnd, PBM_SETSTATE, mState, 0);
}

LRESULT ProgressBar::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}
