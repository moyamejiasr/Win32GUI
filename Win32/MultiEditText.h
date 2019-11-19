#pragma once
#pragma warning (disable : 26495)

#include "EditText.h"

class MultiEditText : public EditText
{
public:
	MultiEditText();
	MultiEditText(Control*, std::string, RECT);
	MultiEditText(Control*, std::string, int = 0, int = 0);
};