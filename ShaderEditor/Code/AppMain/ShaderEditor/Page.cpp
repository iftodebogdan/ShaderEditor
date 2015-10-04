#include "stdafx.h"
#include "Page.h"
#include "MainWindow.h"
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
	m_SaveQuitButton("Save & Quit"),
	m_CompileButton("Compile"),
	m_RunButton("Run"),
	m_Label_Shader("Vertex Shader"),
	m_Label_Pixel("Pixel Shader")
{
	this->m_Notebook = notebook;
	this->m_Name = name;
	this->m_Filename = filename;
	this->m_Filename_Shader = filename + "_Shader";
	this->m_Filename_Pixel = filename + "_Pixel";
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
	std::ifstream* fileShader;
	std::ifstream* filePixel;

	// Define the background color of the text editor.
	color.set_green(52565);
	color.set_red(52565);
	color.set_blue(52565);

	// Set the shader text editor color and border.
	this->m_Textview_Shader.set_border_width(5);
	this->m_Textview_Shader.modify_base(Gtk::STATE_NORMAL, color);

	// Set the content for shader textview, if available.
	fileShader = new std::ifstream(this->m_Filename_Shader);
	if (fileShader->good()) {
		Glib::RefPtr<Gtk::TextBuffer> textBuffer = this->m_Textview_Shader.get_buffer();
		Gtk::TextBuffer::iterator iterator;

		iterator = textBuffer->get_iter_at_offset(0);
		textBuffer->insert(
			iterator,
			static_cast<std::stringstream const&> (std::stringstream() << fileShader->rdbuf()).str()
		);

		fileShader->close();
	}

	// Set the pixel text editor color and border.
	this->m_Textview_Pixel.set_border_width(5);
	this->m_Textview_Pixel.modify_base(Gtk::STATE_NORMAL, color);

	// Set the content for pixel textview, if available.
	filePixel = new std::ifstream(this->m_Filename_Pixel);
	if (filePixel->good()) {
		Glib::RefPtr<Gtk::TextBuffer> textBuffer = this->m_Textview_Pixel.get_buffer();
		Gtk::TextBuffer::iterator iterator;

		iterator = textBuffer->get_iter_at_offset(0);
		textBuffer->insert(
			iterator,
			static_cast<std::stringstream const&> (std::stringstream() << filePixel->rdbuf()).str()
		);

		filePixel->close();
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
	this->m_CompileButton.signal_clicked().connect(
		sigc::bind(
		sigc::mem_fun(
		this,
		&PageWidget::onCompileButtonPressed
		),
		true
		)
		);
	this->m_RunButton.signal_clicked().connect(
		sigc::bind(
		sigc::mem_fun(
		this,
		&PageWidget::onRunButtonPressed
		),
		true
		)
		);

	// Add the buttons into the button box.
	//this->m_ButtonBox.pack_start(this->m_QuitButton, Gtk::PACK_SHRINK);
	//this->m_ButtonBox.pack_end(this->m_SaveQuitButton, Gtk::PACK_SHRINK);
	//this->m_ButtonBox.pack_start(this->m_RunButton, Gtk::PACK_SHRINK);
	this->m_ButtonBox.pack_end(this->m_CompileButton, Gtk::PACK_SHRINK);
	this->m_ButtonBox.set_border_width(15);

	// Build the shader scrolled window, which will serve as a support for the shader text view.
	this->m_ScrolledWindow_Shader.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	this->m_ScrolledWindow_Shader.set_border_width(10);
	this->m_ScrolledWindow_Shader.add(this->m_Textview_Shader);

	// Build the pixel scrolled window, which will server as a support for the pixel text view.
	this->m_ScrolledWindow_Pixel.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	this->m_ScrolledWindow_Pixel.set_border_width(10);
	this->m_ScrolledWindow_Pixel.add(this->m_Textview_Pixel);

	// Build the shader VBox.
	this->m_VBox_Shader.pack_start(this->m_Label_Shader, Gtk::PACK_SHRINK);
	this->m_Label_Shader.get_allocation().set_height(5);
	this->m_VBox_Shader.pack_end(this->m_ScrolledWindow_Shader);

	// Build the pixel VBox.
	this->m_VBox_Pixel.pack_start(this->m_Label_Pixel, Gtk::PACK_SHRINK);
	this->m_Label_Pixel.get_allocation().set_height(5);
	this->m_VBox_Pixel.pack_end(this->m_ScrolledWindow_Pixel);

	// Add the scrolled windows corresponding to the text views into the horizontal box container.
	this->m_HBox.pack_start(this->m_VBox_Shader);
	this->m_HBox.pack_end(this->m_VBox_Pixel);

	// Add the scrolled windows and the button box into the main containers.
	this->pack_start(this->m_HBox);
	this->pack_end(this->m_ButtonBox, Gtk::PACK_SHRINK);

	// Show the child widgets.
	this->show_all();
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
		std::ofstream fileShader(this->m_Filename_Shader);
		std::ofstream filePixel(this->m_Filename_Pixel);
		Glib::RefPtr<Gtk::TextBuffer> textBuffer;

		// Save the contents of the shader textview.
		textBuffer = this->m_Textview_Shader.get_buffer();
		fileShader<<textBuffer->get_text();

		fileShader.close();

		// Save the contents of the pixel textview.
		textBuffer = this->m_Textview_Pixel.get_buffer();
		filePixel<<textBuffer->get_text();

		filePixel.close();
	}

	(*this->m_Pages)[this->m_Name] = NULL;
	this->m_Notebook->remove(*this);
}

void PageWidget::onCompileButtonPressed(bool)
{
	for (unsigned int i = 0; i < (unsigned int)m_Notebook->get_n_pages(); i++)
	{
		std::string currPage = ((PageWidget*)m_Notebook->get_nth_page(i))->getName();
		for (unsigned int p = 0; p < MenuWidget::m_arrShaderPassDesc.size(); p++)
			if (currPage == MenuWidget::m_arrShaderPassDesc[p].name)
			{
				MenuWidget::m_arrShaderPassDesc[p].vertexShader.src =
					((PageWidget*)m_Notebook->get_nth_page(i))->m_Textview_Shader.get_buffer()->get_text();
				MenuWidget::m_arrShaderPassDesc[p].pixelShader.src =
					((PageWidget*)m_Notebook->get_nth_page(i))->m_Textview_Pixel.get_buffer()->get_text();
			}
	}
	((MainWindow*)(get_ancestor(MainWindow::get_type())))->CompileShader();
}

void PageWidget::onRunButtonPressed(bool)
{
	std::cout << "IMPLEMENT ME THREE!" << std::endl;
}