#include "TextView.h"

TextView::TextView()
{
}

TextView::TextView(Control* parent, std::string name, int width, int height)
	: TextControl(name, width, height)
{
	mXMargin = 0; mYMargin = 0;
	mStyle = WS_CHILD | WS_VISIBLE;
	mParent = parent;
	mType = WC_STATIC;
	create();
	autoSize();
}
