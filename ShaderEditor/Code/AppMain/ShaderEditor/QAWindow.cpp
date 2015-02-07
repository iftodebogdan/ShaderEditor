#include "stdafx.h"
#include "QAWindow.h"
#include <string>
#include <iostream>
#include <stdio.h>
#include "Text.h"
#include "HomeWindow.h"

/**
 * The class constructor.
 */
QAWindow::QAWindow() :
	m_TitleLabel("Title: "),
	m_ContentLabel("Content: "),
	m_SubmitButton("Ask!"),
	m_BackButton("Go Back"),
	m_WindowTitle("Ask a ShDEV!")
{
	TextHelper* textHelper = TextHelper::getInstance();

	// Set window main properties.
	this->set_size_request(400, 300);
	this->set_title("Ask a ShDEV!");
	this->set_border_width(20);

	textHelper->changeLabelFont(&this->m_WindowTitle, "Arial", 16);

	// Set title container properties and attach its components.
	this->m_TitleContainer.set_border_width(5);
	this->m_TitleContainer.pack_start(this->m_TitleLabel);
	this->m_TitleContainer.pack_end(this->m_TitleEntry);

	// Set content container properties and attach its components.
	this->m_ContentContainer.set_border_width(5);
	this->m_ContentContainer.pack_start(this->m_ContentLabel);
	this->m_ContentContainer.pack_end(this->m_ContentEntry);

	// Attach event handlers to the submit & back buttons.
	this->m_SubmitButton.signal_clicked().connect(
		sigc::mem_fun(
			this,
			&QAWindow::onSubmit
		)
	);
	this->m_BackButton.signal_clicked().connect(
		sigc::mem_fun(
			this,
			&QAWindow::onBack
		)
	);

	// Add the buttons to their container.
	this->m_SubmitBackButtonContainer.set_border_width(10);
	this->m_SubmitBackButtonContainer.pack_start(this->m_SubmitButton);
	this->m_SubmitBackButtonContainer.pack_end(this->m_BackButton);

	// Add the components to the main container.
	this->m_MainContainer.pack_start(this->m_WindowTitle);
	this->m_MainContainer.pack_start(this->m_TitleContainer);
	this->m_MainContainer.pack_start(this->m_ContentContainer);
	this->m_MainContainer.pack_start(this->m_SubmitBackButtonContainer);

	// Add the main the container to the window.
	this->add(m_MainContainer);

	show_all_children();
}

/**
 * The class destructor.
 */
QAWindow::~QAWindow()
{
}

/**
 * Method used to perform system commands and fetch their output.
 */
std::string QAWindow::execCommand(char* cmd) 
{
    FILE* pipe = _popen(cmd, "r");
	char buffer[128];
    std::string result = "";

	if (!pipe) {
		return "ERROR";
	}
    
	while(!feof(pipe)) {
    	if (fgets(buffer, 128, pipe) != NULL) {
    		result += buffer;
		}
    }

    _pclose(pipe);

    return result;
}

/**
 * Submit button handler.
 * Posts the user question to an external API, accessed via an ugly hack
 * (an external PHP script executed with a system command) and fetches
 * the most appropiate (hopefully) answer.
 */
void QAWindow::onSubmit()
{
	TextHelper* textHelper = TextHelper::getInstance();
	std::string title, content, command, bestAnswer;
	char* encodedCommand;

	// Fetch title and content.
	title = this->m_TitleEntry.get_text();
	content = this->m_ContentEntry.get_text();

	// Compose and execute it.
	command = "php ..\\Code\\Libraries\\Q2A-API\\main.php postQuestion \"" + title + "\" \"" + content + "\"";
	encodedCommand = (char*) command.c_str();
	bestAnswer = this->execCommand(encodedCommand);

	// Show the answer to the user.
	Gtk::MessageDialog answer(*this, bestAnswer, true);
	answer.run();

	answer.hide();
}

/**
 * Back button handler.
 */
void QAWindow::onBack()
{
	HomeWindow homeWindow;

	this->hide();

	Gtk::Main::run(homeWindow);

	return;
}