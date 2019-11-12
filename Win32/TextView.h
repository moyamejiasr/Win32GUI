#pragma once
#pragma warning (disable : 26495)

#include "TextControl.h"

class TextView : public TextControl
{
public:
	TextView();
	TextView(Control*, std::string, int = 0, int = 0);
};