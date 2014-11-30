#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>

/**
 * The home window of the application.
 */
class HomeWindow : public Gtk::Window
{
public:
	HomeWindow();
	virtual ~HomeWindow();

protected:
	// The button container.
	Gtk::ButtonBox* m_ButtonBox;
	
	// The buttons.
	Gtk::Button m_StartButton;
	Gtk::Button m_QAButton;
	Gtk::Button m_AboutButton;
	Gtk::Button m_ExitButton;

	// The button actions event handlers.
	void onStartButtonClicked();
	void onQAButtonClicked();
	void onAboutButtonClicked();
	void onExitButtonClicked();
};

#endif