#include "TrackBar.h"

TrackBar::TrackBar()
{
	cmnControlInit(ICC_BAR_CLASSES);
}

TrackBar::TrackBar(Control* parent, RECT rect)
	: Control(parent, "", rect.right, rect.bottom)
{
	cmnControlInit(ICC_BAR_CLASSES);
	setLocation(rect.left, rect.top);
	mStyle = WS_CHILD | WS_VISIBLE | TBS_NOTICKS;
	mType = TRACKBAR_CLASS;
	create();
	setRange(0, 100);
	setPosition(0);
	// Hide legacy dots focus on select
	SendMessage(mHwnd, WM_UPDATEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), 0);
}

TrackBar::TrackBar(Control* parent, int width, int height)
	: Control(parent, "", width, height)
{
	cmnControlInit(ICC_BAR_CLASSES);
	mStyle = WS_CHILD | WS_VISIBLE | TBS_NOTICKS;
	mType = TRACKBAR_CLASS;
	create();
	setRange(0, 100);
	setPosition(0);
	// Hide legacy dots focus on select
	SendMessage(mHwnd, WM_UPDATEUISTATE, MAKEWPARAM(UIS_SET, UISF_HIDEFOCUS), 0);
}

void TrackBar::setOnPositionChanged(f_onPositionChanged call)
{
	mOnPositionChanged = call;
}

void TrackBar::setRange(int vmin, int vmax)
{
	mMin = vmin;
	mMax = vmax;
	if (mCreated)
		SendMessage(mHwnd, TBM_SETRANGE,
		(WPARAM)TRUE, (LPARAM)MAKELONG(mMin, mMax));
}

void TrackBar::setSelectionRange(int vmin, int vmax)
{
	mSelMin = vmin;
	mSelMax = vmax;
	if (mSelMin == 0 && mSelMax == 0)
	{
		removeFlag(TBS_ENABLESELRANGE);
		return;
	}
	appendFlag(TBS_ENABLESELRANGE);
	if (mCreated)
		SendMessage(mHwnd, TBM_SETSEL,
		(WPARAM)FALSE,(LPARAM)MAKELONG(mSelMin, mSelMax));
}

void TrackBar::setPosition(int val)
{
	mPosition = val;
	if (mCreated)
		SendMessage(mHwnd, TBM_SETPOS,
			(WPARAM)TRUE,
			(LPARAM)mPosition);
}

void TrackBar::setTickInterval(int val)
{
	if (val == 0)
	{
		removeFlag(TBS_AUTOTICKS);
		appendFlag(TBS_NOTICKS);
		return;
	}
	
	appendFlag(TBS_AUTOTICKS);
	removeFlag(TBS_NOTICKS);
	if (mCreated)
	{
		// Draw ticks each x
		SendMessage(mHwnd, TBM_SETTICFREQ,
			(LPARAM)val, 0);
		// Move selection on key press x points
		SendMessage(mHwnd, TBM_SETPAGESIZE,
			0, (LPARAM)val);
	}
}

void TrackBar::setOrientation(Orientation val)
{
	switch (val)
	{
	case Orientation::Horizontal:
		removeFlag(TBS_VERT);
		appendFlag(TBS_HORZ);
		break;
	case Orientation::Vertical:
		removeFlag(TBS_HORZ);
		appendFlag(TBS_VERT);
		break;
	}
}

void TrackBar::setReversed(bool state)
{
	if (state)
		appendFlag(TBS_REVERSED);
	else
		removeFlag(TBS_REVERSED);
}

LRESULT TrackBar::execute(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case TB_ENDTRACK:
		mPosition = SendMessage(mHwnd, TBM_GETPOS, 0, 0);
		if (mPosition > mSelMax)
			setPosition(mSelMax);
		else if (mPosition < mSelMin)
			setPosition(mSelMin);
		if (mOnPositionChanged)
			mOnPositionChanged(this, mPosition);
		break;
	}
	return DefWindowProc(mHwnd, uMsg, wParam, lParam);
}
