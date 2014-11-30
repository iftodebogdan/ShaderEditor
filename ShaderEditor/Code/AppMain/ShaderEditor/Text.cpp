#include "stdafx.h"
#include "Text.h"

/**
 * The text helper constructor.
 */
TextHelper::TextHelper()
{
}

/**
 * The text helper destructor.
 */
TextHelper::~TextHelper()
{
}

/**
 * The text helper unique instance fetcher.
 */
TextHelper* TextHelper::getInstance()
{
	static TextHelper* instance = new TextHelper();

	return instance;
}

/**
 * Method used to add customized labels on buttons.
 */
void TextHelper::addStyledLabel(Gtk::Button &button, char* label, char* font_family, int size)
{
	Pango::FontDescription font_desc(font_family);
	font_desc.set_size((int) Pango::SCALE * size);

	Gtk::Label* button_label = new Gtk::Label(label);
	button_label->modify_font(font_desc);
	
	button.add(*button_label);
}