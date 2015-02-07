#include "stdafx.h"
#include "HomeWindow.h"
#include "MainWindow.h"
#include "QAWindow.h"
#include "Text.h"
#include "pango/pangofc-font.h"

/**
 * The home window constructor.
 */
HomeWindow::HomeWindow() :
	m_StartButton(),
	m_QAButton(),
	m_AboutButton(),
	m_ExitButton()
{
	TextHelper* text_helper = TextHelper::getInstance();
	int button_width = 180, button_height = 70, button_font_size = 18;
	char* button_font_name = "Arial";

	// Set window basic properties: size and title.
	set_default_size(800, 600);
	set_title("Shded");

	// Add the button container.
	m_ButtonBox = Gtk::manage(new Gtk::VButtonBox(Gtk::BUTTONBOX_CENTER, 40));
	add(*m_ButtonBox);

	// Add the start button.
	m_ButtonBox->add(m_StartButton);
	m_StartButton.set_size_request(button_width, button_height);
	text_helper->addStyledLabel(m_StartButton, "Start Shded", button_font_name, button_font_size);
	m_StartButton.signal_clicked().connect(
		sigc::mem_fun(
			this,
			&HomeWindow::onStartButtonClicked
		)
	);

	// Add the Q&A button.
	m_ButtonBox->add(m_QAButton);
	m_QAButton.set_size_request(button_width, button_height);
	text_helper->addStyledLabel(m_QAButton, "Q&A!", button_font_name, button_font_size);
	m_QAButton.signal_clicked().connect(
		sigc::mem_fun(
			this,
			&HomeWindow::onQAButtonClicked
		)
	);

	// Add the about button.
	m_ButtonBox->add(m_AboutButton);
	m_AboutButton.set_size_request(button_width, button_height);
	text_helper->addStyledLabel(m_AboutButton, "About", button_font_name, button_font_size);
	m_AboutButton.signal_clicked().connect(
		sigc::mem_fun(
			this,
			&HomeWindow::onAboutButtonClicked
		)
	);

	// Add the exit button.
	m_ButtonBox->add(m_ExitButton);
	m_ExitButton.set_size_request(button_width, button_height);
	text_helper->addStyledLabel(m_ExitButton, "Exit", button_font_name, button_font_size);
	m_ExitButton.signal_clicked().connect(
		sigc::mem_fun(
			this,
			&HomeWindow::onExitButtonClicked
		)
	);

	show_all_children();
}

/**
 * The home window destructor.
 */
HomeWindow::~HomeWindow()
{
}

/**
 * Start button click event handler.
 */
void HomeWindow::onStartButtonClicked()
{
	MainWindow main_window;
	
	// Hide the home window.
	hide();
	// Show the main window.
	Gtk::Main::run(main_window);

	return;
}

/**
 * QA button click event handler.
 */
void HomeWindow::onQAButtonClicked()
{
	QAWindow qAWindow;

	hide();
	
	Gtk::Main::run(qAWindow);

	return;
}

/**
 * About button click event handler.
 */
void HomeWindow::onAboutButtonClicked()
{
	Gtk::MessageDialog messageDialog(
		*this, 
		"This application, as its name says, is a tool which lets users to experiment with "
		"pixel shaders. Its role is to offer a sandbox which makes the rendering of "
		"shaders much more easier for users who already have some knowledge about 3D rendering."
	);
	messageDialog.set_secondary_text("The application was created & designed by Bogdan Iftode, Breta Ionut-Marcel and Popescu Darius.");

	messageDialog.run();

	messageDialog.hide();
}

/**
 * Exit button click event handler.
 */
void HomeWindow::onExitButtonClicked()
{
	hide();
}