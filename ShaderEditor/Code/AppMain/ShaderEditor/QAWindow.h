#ifndef QAWINDOW_H
#define QAWINDOW_H

#include <gtkmm.h>

/**
 * Class which handles the presentation of the Q&A Window.
 */
class QAWindow : public Gtk::Window
{
public:
	// The constructor and the destructor.
	QAWindow();
	virtual ~QAWindow();

protected:
	// The components of the window.
	Gtk::VBox			m_MainContainer;

	Gtk::Label			m_WindowTitle;
	
	Gtk::HBox			m_TitleContainer;
	Gtk::Label			m_TitleLabel;
	Gtk::Entry			m_TitleEntry;
	
	Gtk::HBox			m_ContentContainer;
	Gtk::Label			m_ContentLabel;
	Gtk::Entry			m_ContentEntry;

	Gtk::HButtonBox		m_SubmitBackButtonContainer;
	Gtk::Button			m_BackButton;
	Gtk::Button			m_SubmitButton;

	// Helper method, used to execute system commands.
	std::string QAWindow::execCommand(char* cmd);
	// Button click event handlers.
	void onSubmit();
	void onBack();
};

#endif