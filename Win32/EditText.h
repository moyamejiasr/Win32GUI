#pragma once
#pragma warning (disable : 26495)

#include "TextControl.h"

class EditText : public TextControl
{
public:
	EditText();
	EditText(Control*, std::string, int = 0, int = 0);

	void setCharLimit(int);
protected:
	int mCharLimit;

	virtual void execute(UINT, WPARAM, LPARAM);
};