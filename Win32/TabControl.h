#pragma once
#pragma warning (disable : 26495)

#include "Control.h"

class TabControl : public Control
{
public:
	TabControl();
	TabControl(Control*, RECT);
	TabControl(Control*, int = 0, int = 0);

	void appendPage(std::string);
	void appendPage(std::string, int);
	void setImageList(ImageList*);
	void setBottomTabs(bool);
protected:
	std::vector<TCITEM> mPages;

	virtual LRESULT execute(UINT, WPARAM, LPARAM);
	virtual LRESULT drawctl(UINT, WPARAM, LPARAM);
};