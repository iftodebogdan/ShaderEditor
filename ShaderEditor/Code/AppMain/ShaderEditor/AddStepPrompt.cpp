#include "stdafx.h"
#include "AddStepPrompt.h"

/**
 * The class constructor implementation.
 */
AddStepPrompt::AddStepPrompt() :
	m_AddButton("Add step")
{
	this->set_border_width(15);
	this->set_size_request(150, 100);

	// Add the text entry.
	this->m_VBox.pack_start(this->m_Entry);

	// Add the button and assign an event handler for it.
	this->m_AddButton.signal_clicked().connect(sigc::mem_fun(this, &AddStepPrompt::OnAddButtonClicked));
	this->m_ButtonBox.add(this->m_AddButton);
	this->m_VBox.pack_start(this->m_ButtonBox);

	this->add(m_VBox);

	show_all_children();
}

/**
 * The class destructor.
 */
AddStepPrompt::~AddStepPrompt()
{
}

/**
 * Fetches the name of the user-defined item.
 */
std::string AddStepPrompt::getItemName()
{
	return this->m_ItemName;
}

/**
 * Click event handler for "Add Step" button.
 */
void AddStepPrompt::OnAddButtonClicked()
{
	this->m_ItemName = this->m_Entry.get_text();

	this->hide();
}

