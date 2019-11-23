#include "FixedComboBox.h"

FixedComboBox::FixedComboBox()
{
}

FixedComboBox::FixedComboBox(Control* parent, RECT rect)
	:ComboBox(parent, CBS_DROPDOWNLIST, rect.right, rect.bottom)
{
	setLocation(rect.left, rect.top);
	create();
}

FixedComboBox::FixedComboBox(Control* parent, int width, int height)
	:ComboBox(parent, CBS_DROPDOWNLIST, width, height)
{
	create();
}
