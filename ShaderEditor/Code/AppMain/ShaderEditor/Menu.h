#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <iostream>
#include <gtkmm.h>
#include "Page.h"

/**
 * Class used for building the menu from the workspace frame.
 */
class MenuWidget
{
public:
	// The class constructors.
	MenuWidget();
	MenuWidget(
		Gtk::Window*										window,
		Gtk::Box*											container,
		std::map<int, std::map<std::string, std::string>>	items,
		Gtk::Notebook*										notebook
	);
	// The class destructor.
	virtual ~MenuWidget();

	// The menu builder method.
	void buildMenu();
	void setItems(std::map<int, std::map<std::string, std::string>>	items);

protected:
	Gtk::Window*										m_Window;
	Gtk::Box*											m_Container;
	std::map<int, std::map<std::string, std::string>>	m_Items;
	Glib::RefPtr<Gtk::ActionGroup>						m_ActionGroup;
	Glib::RefPtr<Gtk::UIManager>						m_UIManager;
	Gtk::Notebook*										m_Notebook;
	std::map<std::string, Gtk::VBox*>					m_Pages;
	Gtk::Widget*										m_MenuBar;

	// The menu item action callback.
	void onMenuAccess(std::string name, std::string filename);
	// Helper method used for fetching the index of page, given its name.
	int MenuWidget::getPageIndexByName(std::string name);
};

#endif