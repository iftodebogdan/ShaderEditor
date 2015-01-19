#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <gtkmm.h>

/**
 * Class which defines the content of a notebook page.
 */
class PageWidget : public Gtk::VBox
{
public:
	// The class constructor.
	PageWidget(
		Gtk::Notebook* notebook,
		std::string name,
		std::string filename,
		std::map<std::string,Gtk::VBox*>* pages
	);
	// The class destructor.
	virtual ~PageWidget();

	// Page name getter method;
	std::string							getName();

protected:
	Gtk::Notebook*						m_Notebook;
	std::string							m_Name;
	std::string							m_Filename;
	std::map<std::string, Gtk::VBox*>*	m_Pages;
	Gtk::ScrolledWindow					m_ScrolledWindow;
	Gtk::TextView						m_Textview;
	Gtk::HButtonBox						m_ButtonBox;
	Gtk::Button							m_QuitButton;
	Gtk::Button							m_SaveQuitButton;

	// Method which builds the content of the page.
	void								buildPage();
	// Quit and Save & Quit buttons click callback.
	void								onQuitButtonPressed(bool save);
};

#endif