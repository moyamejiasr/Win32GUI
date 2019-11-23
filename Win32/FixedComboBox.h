#pragma once
#pragma warning (disable : 26495)

#include "ComboBox.h"

class FixedComboBox : public ComboBox
{
public:
	FixedComboBox();
	FixedComboBox(Control*, RECT);
	FixedComboBox(Control*, int = 0, int = 25);
};