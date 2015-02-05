#ifndef ADDSTEPPROMPT_H
#define ADDSTEPPROMPT_H

#include <gtkmm.h>

/**
 * Handles the prompt which appears when adding a new rendering step.
 */
class AddStepPrompt : public Gtk::Window
{
public:
	// Class constructor and destructor.
	AddStepPrompt();
	virtual ~AddStepPrompt();

	// User-defined item name fetcher.
	std::string getItemName();

protected:
	Gtk::VBox m_VBox;
	Gtk::Entry m_Entry;
	Gtk::HButtonBox m_ButtonBox;
	Gtk::Button m_AddButton;
	std::string m_ItemName;

	// Click event handler for the 'Add' button. 
	void OnAddButtonClicked();
};

#endif