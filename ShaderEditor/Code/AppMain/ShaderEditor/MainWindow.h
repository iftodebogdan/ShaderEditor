#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/window.h>
#include <gtkmm/paned.h>
#include <gtkmm/frame.h>
#include <gtkmm/drawingarea.h>

class MainWindow : public Gtk::Window
{
public:
			MainWindow();
	virtual	~MainWindow();
	
protected:
	// Called when the window has been created
	void OnCreate();
	// Called every frame
	bool OnUpdate();

	//Child widgets:
	Gtk::VPaned			m_VPaned;
	Gtk::HPaned			m_HPanedTop;
	Gtk::HPaned			m_HPanedBottom;
	Gtk::Frame			m_WorkspaceFrame;
	Gtk::Frame			m_EditorFrame;
	Gtk::Frame			m_PreviewFrame;
	Gtk::DrawingArea	m_PreviewDrawingArea;
	Gtk::Frame			m_OutputFrame;
};

#endif