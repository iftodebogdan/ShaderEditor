#include "stdafx.h"
#include "Page.h"
#include <iostream>
#include <fstream>

/**
 * The class constructor.
 */
PageWidget::PageWidget(
	Gtk::Notebook* notebook,
	std::string name,
	std::string filename,
	std::map<std::string, Gtk::VBox*>* pages
)
	: m_QuitButton("Quit"),
	m_SaveQuitButton("Save & Quit")
{
	this->m_Notebook = notebook;
	this->m_Name = name;
	this->m_Filename = filename;
	this->m_Pages = pages;

	this->buildPage();
}

/**
 * The class destructor.
 */
PageWidget::~PageWidget()
{
}

/**
 * The page builder method.
 */
void PageWidget::buildPage()
{
	Gdk::Color color;
	std::ifstream* file;

	// Define the background color of the text editor.
	color.set_green(52565);
	color.set_red(52565);
	color.set_blue(52565);

	// Set the text editor color and border.
	this->m_Textview.set_border_width(5);
	this->m_Textview.modify_base(Gtk::STATE_NORMAL, color);

	file = new std::ifstream(this->m_Filename);
	if (file->good()) {
		Glib::RefPtr<Gtk::TextBuffer> textBuffer = this->m_Textview.get_buffer();
		Gtk::TextBuffer::iterator iterator;

		iterator = textBuffer->get_iter_at_offset(0);
		textBuffer->insert(
			iterator,
			static_cast<std::stringstream const&> (std::stringstream() << file->rdbuf()).str()
		);

		file->close();
	}
	
	// Add click event listeners for Quit & Save-Quit buttons.
	this->m_QuitButton.signal_clicked().connect(
		sigc::bind(
			sigc::mem_fun(
				this,
				&PageWidget::onQuitButtonPressed
			),
			false
		)
	);
	this->m_SaveQuitButton.signal_clicked().connect(
		sigc::bind(
			sigc::mem_fun(
				this,
				&PageWidget::onQuitButtonPressed
			),
			true
		)
	);

	// Add the buttons into the button box.
	this->m_ButtonBox.pack_start(this->m_QuitButton, Gtk::PACK_SHRINK);
	this->m_ButtonBox.pack_end(this->m_SaveQuitButton, Gtk::PACK_SHRINK);
	this->m_ButtonBox.set_border_width(15);

	// Build the scrolled window, which will server as a support for the text view.
	this->m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	this->m_ScrolledWindow.set_border_width(10);
	this->m_ScrolledWindow.add(this->m_Textview);

	// Add the scrolled window and the button box into the main container.
	this->pack_start(this->m_ScrolledWindow);
	this->pack_start(this->m_ButtonBox);
}

/**
 * Name getter method.
 */
std::string PageWidget::getName()
{
	return this->m_Name;
}

/**
 * Quit and Save-Quit buttons callback.
 */
void PageWidget::onQuitButtonPressed(bool save)
{
	if (save) {
		std::ofstream file(this->m_Filename);
		Glib::RefPtr<Gtk::TextBuffer> textBuffer = this->m_Textview.get_buffer();

		file<<textBuffer->get_text();

		file.close();
	}

	(*this->m_Pages)[this->m_Name] = NULL;
	this->m_Notebook->remove(*this);
}