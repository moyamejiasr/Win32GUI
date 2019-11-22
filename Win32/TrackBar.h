#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

using f_onPositionChanged = void(*)(Control*, int pos);

class TrackBar : public Control
{
public:
	TrackBar();
	TrackBar(Control*, RECT);
	TrackBar(Control*, int = 0, int = 0);

	void setOnPositionChanged(f_onPositionChanged);
	void setRange(int, int);
	void setSelectionRange(int = 0, int = 0);
	void setPosition(int);
	void setTickInterval(int);
	void setOrientation(Orientation);
	void setReversed(bool);
protected:
	f_onPositionChanged mOnPositionChanged;
	int mMin = 0, mMax = 100;
	int mSelMin = 0, mSelMax = 0;
	int mPosition = 0;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
};