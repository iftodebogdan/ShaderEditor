#ifndef TEXTHELPER_H
#define TEXTHELPER_H

#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <pango/pangofc-font.h>

/**
 * Helper used to manipulate text elements, such as labels.
 */
class TextHelper
{
public:
	// Unique instance fetcher.
	static TextHelper* getInstance();

	// Method used to add a customized label on a button.
	void addStyledLabel(Gtk::Button &button, char* label, char* font_family, int size);

private:
	TextHelper();
	virtual ~TextHelper();

	// The unique instance of the helper.
	static TextHelper* instance;
};

#endif