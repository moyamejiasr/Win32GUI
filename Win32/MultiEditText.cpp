#include "MultiEditText.h"

MultiEditText::MultiEditText()
{
}

MultiEditText::MultiEditText(Control* parent, std::string name, RECT rect)
	: EditText(parent, name, rect.right, rect.bottom)
{
	mParent = parent;
	mStyle |= ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL;
	create();
}


MultiEditText::MultiEditText(Control* parent, std::string name, int width, int height)
	:EditText(name, width, height)
{
	mParent = parent;
	mStyle |= ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL;
	create();
}
