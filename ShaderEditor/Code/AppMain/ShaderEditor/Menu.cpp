#include "stdafx.h"
#include "Menu.h"

/**
 * The class constructor.
 */
MenuWidget::MenuWidget(MainWindow* window, Gtk::Frame* container, std::map<int, std::map<std::string, std::string>> items)
{
	// Initialise properties.
	this->m_Window = window;
	this->m_Container = container;
	this->m_Items = items;
	this->m_ActionGroup = Gtk::ActionGroup::create();
	this->m_UIManager = Gtk::UIManager::create();

	// Build the menu.
	this->buildMenu();
}

/**
 * The class destructor.
 */
MenuWidget::~MenuWidget()
{
}

/**
 * The menu builder method.
 */
void MenuWidget::buildMenu()
{
	Gtk::Widget* menuBar;
	std::string name;
	std::map<int, std::map<std::string, std::string>>::iterator iterator;
	Glib::ustring ui_string;

	// Build the action group and the UI from the given items.
	for (iterator = this->m_Items.begin(); iterator != this->m_Items.end(); iterator ++) {
		name = iterator->second["name"];
		this->m_ActionGroup->add(Gtk::Action::create(name, name));
		ui_string += "<toolitem action='" + name + "' />";
	}
	// Set action group.
	this->m_UIManager->insert_action_group(this->m_ActionGroup);
	
	// Load the defined UI.
	ui_string =
		"<ui>"
		"  <toolbar name='MenuBar'>"
		+ ui_string +
		"  </toolbar>"
		"</ui>";
	this->m_UIManager->add_ui_from_string(ui_string);

	// Fetch the menu bar and orient it vertically.
	menuBar = this->m_UIManager->get_widget("/MenuBar");
	menuBar->set_property("orientation", Gtk::ORIENTATION_VERTICAL);

	// Add the menu bar and the accel_group to the window.
	this->m_Window->add_accel_group(this->m_UIManager->get_accel_group());
	this->m_Container->add(*menuBar);
}