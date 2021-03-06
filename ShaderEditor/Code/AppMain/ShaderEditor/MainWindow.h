#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Menu.h"
#include <gtkmm/window.h>
#include <gtkmm/paned.h>
#include <gtkmm/frame.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/notebook.h>
#include "Renderer.h"
using namespace LibRendererDll;

class MainWindow : public Gtk::Window
{
public:
			MainWindow();
	virtual	~MainWindow();

	void CompileShader();
	void RunShader();
	
protected:
	// Called when the window has been created
	void OnCreate();
	// Called every frame
	bool OnUpdate();
	// Called on mouse events
	void CenterPointerInWidget(Gtk::Widget* widget);
	bool OnButtonPress(GdkEventButton* ev);
	bool OnButtonRelease(GdkEventButton* ev);
	bool OnMouseMove(GdkEventMotion* ev);
	bool OnKeyPress(GdkEventKey* ev);
	bool OnKeyRelease(GdkEventKey* ev);
	void OnAddStepButtonClicked();
	void OnRemoveStepButtonClicked();
	bool m_bLeftClick, m_bRightClick;
	Vec3f m_vMove;
	float m_fSpeedFactor;
	struct Camera
	{
		Vec3f vPos;
		Matrix44f mRot;
	} m_tCamera;
	std::map<int, std::map<std::string, std::string>> m_MenuItems;

	// Child widgets:
	Gtk::VPaned			m_VPaned;
	Gtk::HPaned			m_HPanedTop;
	Gtk::HPaned			m_HPanedBottom;
	Gtk::Frame			m_WorkspaceFrame;
	Gtk::VBox			m_MenuBox;
	Gtk::VBox			m_MenuContainer;
	Gtk::Button			m_AddStepButton;
	Gtk::Button			m_RemoveStepButton;
	Gtk::Frame			m_EditorFrame;
	Gtk::Frame			m_PreviewFrame;
	Gtk::DrawingArea	m_PreviewDrawingArea;
	Gtk::Frame			m_OutputFrame;
	Gtk::Label			m_OutputLabel;

	// Components
	MenuWidget*			m_MenuWidget;
	Gtk::Notebook		m_Notebook;

	VertexBuffer*		m_pVB;
	Texture*			m_pTex1;
	Texture*			m_pTex2;
	ShaderTemplate*		m_pVertexShader;
	ShaderTemplate*		m_pPixelShader;
	ShaderInput*		m_pVSInput;
	ShaderInput*		m_pPSInput;
	RenderTarget*		m_pRT;
};

#endif