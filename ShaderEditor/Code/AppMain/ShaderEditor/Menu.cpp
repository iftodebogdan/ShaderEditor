#include "stdafx.h"
#include "Menu.h"

/**
 * The default class constructor.
 */
MenuWidget::MenuWidget()
{
}

/**
 * The real class constructor.
 */
MenuWidget::MenuWidget(
	Gtk::Window* window,
	Gtk::Box* container,
	std::map<int, std::map<std::string, std::string>> items,
	Gtk::Notebook* notebook
) {
	// Initialise properties.
	this->m_Window = window;
	this->m_Container = container;
	this->m_Items = items;
	this->m_ActionGroup = Gtk::ActionGroup::create();
	this->m_UIManager = Gtk::UIManager::create();
	this->m_Notebook = notebook;

	// Build the menu.
	this->buildMenu();
}

/**
 * The class destructor.
 */
MenuWidget::~MenuWidget()
{
	this->m_Container->remove(*this->m_MenuBar);
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

		// On menu button click, we want to open a new tab into the notebook.
		this->m_ActionGroup->add(
			Gtk::Action::create(name, name),
			sigc::bind(sigc::mem_fun(*this, &MenuWidget::onMenuAccess), name, iterator->second["file"])
		);

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
	this->m_Container->pack_start(*menuBar);

	// Store the menu bar.
	this->m_MenuBar = menuBar;
}

/**
 * Menu item click callback.
 * Adds a new page into the notebook, and marks it into the pages structure,
 * to not add it again on the next access of the current menu item.
 */
void MenuWidget::onMenuAccess(std::string name, std::string filename)
{
	int pageIndex;
	PageWidget* pageWidget;
	
	// If the page with the given name was already added, do not add it again.
	// Instead, set it as the current page.
	if (this->m_Pages[name] != NULL) {
		pageIndex = this->getPageIndexByName(name);
		// If the page cannot be found, stop the function execution.
		if (pageIndex == -1) {
			return;
		}

		this->m_Notebook->set_current_page(pageIndex);

		return;
	}

	// Build the page and attach it to the notebook.
	pageWidget = new PageWidget(this->m_Notebook, name, filename, &this->m_Pages);
	pageIndex = this->m_Notebook->append_page(*pageWidget, name);
	
	// Save the page.
	this->m_Pages[name] = pageWidget;

	// Show the notebook tabs and set the new page as the current one.
	this->m_Notebook->show_all();
	this->m_Notebook->set_current_page(pageIndex);
}

/**
 * Helper method used to fetch the page index, given its name.
 */
int MenuWidget::getPageIndexByName(std::string name)
{
	int i, nrPages;
	PageWidget* widget;
	
	nrPages = this->m_Notebook->get_n_pages();
	for (i = 0; i < nrPages; i++) {
		widget = (PageWidget*) this->m_Notebook->pages()[i].get_child();
		if (name == widget->getName()) {
			return i;
		}
	}

	return -1;
}

/**
 * Menu items setter.
 */
void MenuWidget::setItems(std::map<int, std::map<std::string, std::string>>	items)
{
	this->m_Items = items;
}