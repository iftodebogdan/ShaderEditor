#include "stdafx.h"

#include "Menu.h"
#include "MainWindow.h"
#include "Renderer.h"
#include <gdk/gdkwin32.h>

using namespace LibRendererDll;

MainWindow::MainWindow() : m_Notebook()
{
	// Set window properties
	set_title("ShdEd");
	set_border_width(10);
	set_default_size(800, 600);

	// Menu items list. Defined for example purposes.
	std::map<int, std::map<std::string, std::string>> items;
	items[0]["name"] = "New";
	items[0]["file"] = "New";
	items[1]["name"] = "Quit";
	items[1]["file"] = "New";
	items[2]["name"] = "Shader";
	items[2]["file"] = "New";
	items[3]["name"] = "Set";
	items[3]["file"] = "New";

	// Build the workspace frame menu.
	m_MenuWidget = new MenuWidget(this, &m_WorkspaceFrame, items, &m_Notebook);

	// Add the notebook to the editor frame.
	m_EditorFrame.add(m_Notebook);

	// Add the pane widget to our toplevel window
	add(m_VPaned);

	// Add the contents of the vertical pane
	m_VPaned.pack1(m_HPanedTop , true, true);
	m_VPaned.pack2(m_HPanedBottom , false, false);
	
	// Add functional widgets to the panes
	m_HPanedTop.pack1(m_WorkspaceFrame , false, true);
	m_HPanedTop.pack2(m_EditorFrame , true, true);
	m_HPanedBottom.pack1(m_PreviewFrame , false, false);
	m_HPanedBottom.pack2(m_OutputFrame , true, true);
	
	// Set the frames' labels
	m_WorkspaceFrame.set_label("Workspace frame");
	m_EditorFrame.set_label("Editor frame");
	m_PreviewFrame.set_label("Preview frame");
	m_OutputFrame.set_label("Output frame");
	
	// Set frame sizes
	int fWindowSizeX, fWindowSizeY;
	get_size(fWindowSizeX, fWindowSizeY);
	m_WorkspaceFrame.set_size_request((int)(fWindowSizeX * 0.2f), 0);
	m_PreviewFrame.set_size_request((int)(fWindowSizeY * 0.4f), (int)(fWindowSizeY * 0.4f));

	m_PreviewDrawingArea.set_app_paintable(true);
	m_PreviewFrame.add(m_PreviewDrawingArea);

	// Set up callbacks
	m_PreviewDrawingArea.signal_realize().connect(sigc::mem_fun(this, &MainWindow::OnCreate));

	// Show all children
	show_all_children();
}

MainWindow::~MainWindow()
{
	Renderer::GetInstance()->DestroyInstance();
}

void MainWindow::OnCreate()
{
	Glib::signal_idle().connect(sigc::mem_fun(this, &MainWindow::OnUpdate));

	Renderer::CreateInstance(Renderer::API_DX9);
	HGDIOBJ handle = GDK_WINDOW_HWND(get_window()->gobj());
	
	Renderer::GetInstance()->Initialize(
		(void*)handle,
		m_PreviewDrawingArea.get_allocation().get_width(),
		m_PreviewDrawingArea.get_allocation().get_height()
	);
}

bool MainWindow::OnUpdate()
{
	Renderer *renderer = Renderer::GetInstance();
	RenderData renderData = renderer->GetRenderData();

	renderData.backBufferSize = Vec2i(
		m_PreviewDrawingArea.get_allocation().get_width(),
		m_PreviewDrawingArea.get_allocation().get_height()
	);
	renderData.dstRect.topLeft = Vec2i(
		m_PreviewDrawingArea.get_allocation().get_x(),
		m_PreviewDrawingArea.get_allocation().get_y()
	);
	renderData.dstRect.bottomRight = renderData.dstRect.topLeft +
		Vec2i(
			m_PreviewDrawingArea.get_allocation().get_width(),
			m_PreviewDrawingArea.get_allocation().get_height()
		);
	renderer->SetRenderData(renderData);
	renderer->RenderScene();

	return true;
}
