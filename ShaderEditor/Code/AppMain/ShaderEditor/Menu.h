#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include "MainWindow.h"
#include <gtkmm.h>

/**
 * Class used for building the menu from the workspace frame.
 */
class MenuWidget
{
public:
	// The class constructor.
	MenuWidget(MainWindow* window, Gtk::Frame* container, std::map<int, std::map<std::string, std::string>> items);
	// The class destructor.
	virtual ~MenuWidget();

protected:
	MainWindow* m_Window;
	Gtk::Frame* m_Container;
	std::map<int, std::map<std::string, std::string>> m_Items;
	Glib::RefPtr<Gtk::ActionGroup> m_ActionGroup;
	Glib::RefPtr<Gtk::UIManager> m_UIManager;

	// The menu builder method.
	void buildMenu();
};

#endif