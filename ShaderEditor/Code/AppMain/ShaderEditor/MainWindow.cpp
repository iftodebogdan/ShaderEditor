#include "stdafx.h"

#include "Menu.h"
#include "MainWindow.h"
#include <gdk/gdkwin32.h>
#include <fstream>


MainWindow::MainWindow()
	: m_Notebook()
	, m_pVB(nullptr)
	, m_pTex1(nullptr)
	, m_pTex2(nullptr)
	, m_pVertexShader(nullptr)
	, m_pPixelShader(nullptr)
	, m_pVSInput(nullptr)
	, m_pPSInput(nullptr)
	, m_pRT(nullptr)
	, m_bLeftClick(false)
	, m_bRightClick(false)
	, m_fSpeedFactor(1.f)
{
	// Set window properties
	set_title("ShdEd");
	set_border_width(10);

	// Ask to maximize, then show the window so that the width/height
	// are valid later when we retrieve them
	maximize();
	show();

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

	// Add and configure output label
	m_OutputFrame.add(m_OutputLabel);
	m_OutputLabel.set_selectable();
	m_OutputLabel.set_line_wrap();
	m_OutputLabel.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);

	// Add the notebook to the editor frame.
	m_EditorFrame.add(m_Notebook);

	// Add the pane widget to our toplevel window
	add(m_VPaned);
	//add(m_PreviewFrame);

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
	m_PreviewDrawingArea.signal_show().connect(sigc::mem_fun(this, &MainWindow::OnCreate));

	// Show all children
	show_all_children();
}

MainWindow::~MainWindow()
{
	Renderer::GetInstance()->DestroyInstance();
}

void MainWindow::OnCreate()
{
	// Set up a callback that will function as our main loop
	Glib::signal_idle().connect(sigc::mem_fun(this, &MainWindow::OnUpdate));

	// Set up the rendering area for user interactivity
	m_PreviewDrawingArea.set_flags(Gtk::CAN_FOCUS);
	m_PreviewDrawingArea.add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK | Gdk::KEY_PRESS_MASK);
	m_PreviewDrawingArea.signal_button_press_event().connect(sigc::mem_fun(this, &MainWindow::OnButtonPress));
	m_PreviewDrawingArea.signal_button_release_event().connect(sigc::mem_fun(this, &MainWindow::OnButtonRelease));
	m_PreviewDrawingArea.signal_motion_notify_event().connect(sigc::mem_fun(this, &MainWindow::OnMouseMove));
	m_PreviewDrawingArea.signal_key_press_event().connect(sigc::mem_fun(this, &MainWindow::OnKeyPress));
	m_PreviewDrawingArea.signal_key_release_event().connect(sigc::mem_fun(this, &MainWindow::OnKeyRelease));

	// Create an instance of the renderer
	Renderer::CreateInstance(API_DX9);
	HGDIOBJ handle = GDK_WINDOW_HWND(get_window()->gobj());
	Renderer *renderer = Renderer::GetInstance();
	renderer->Initialize((void*)handle);
}

void SetInput(ShaderInput* const shdInput, const unsigned int handle, const unsigned int passIdx, const unsigned int constIdx)
{
	MenuWidget::ShaderPassDesc::ShaderConstants constDesc = MenuWidget::m_arrShaderPassDesc[passIdx].constants[constIdx];
	switch (constDesc.inputType)
	{
	case IT_BOOL:
	case IT_FLOAT:
	case IT_INT:
		if (constDesc.rows == 1)
		{
			shdInput->SetFloatArray(handle, constDesc.valueFloat);
		}
		else
		{
			if (constDesc.rows == 3 && constDesc.cols == 3)
			{
				Matrix33f mat;
				for (unsigned int r = 0; r < constDesc.rows; r++)
					for (unsigned int c = 0; c < constDesc.cols; c++)
					{
						mat[r][c] = constDesc.valueFloat[r * constDesc.cols + c];
					}
				shdInput->SetMatrix3x3(handle, mat);
			}
			else if (constDesc.rows == 4 && constDesc.cols == 4)
			{
				Matrix44f mat;
				for (unsigned int r = 0; r < constDesc.rows; r++)
					for (unsigned int c = 0; c < constDesc.cols; c++)
					{
						mat[r][c] = constDesc.valueFloat[r * constDesc.cols + c];
					}
				shdInput->SetMatrix4x4(handle, mat);
			}
		}
		break;
	case IT_SAMPLER:
	case IT_SAMPLER1D:
	case IT_SAMPLER2D:
	case IT_SAMPLER3D:
	case IT_SAMPLERCUBE:
		if (constDesc.sampler.boundTexture)
			shdInput->SetTexture(handle, constDesc.sampler.boundTexture);
		Renderer::GetInstance()->GetSamplerStateManager()->SetFilter(shdInput->GetInputDesc(handle).nRegisterIndex, SF_MIN_MAG_LINEAR_MIP_LINEAR);
		break;
	default:
		return;
	}
}

bool MainWindow::OnUpdate()
{
	for (unsigned int pass = 0; pass < MenuWidget::m_arrShaderPassDesc.size(); pass++)
	{
		// declare local variables
		Matrix44f worldMat, viewMat, projMat, worldViewProjMat;
		unsigned int vsInputIdx = ~0u, psInputIdx = ~0u;
		ShaderInput *vsInput = nullptr, *psInput = nullptr;

		// get pointer to renderer instance and resource manager
		Renderer *renderer = Renderer::GetInstance();
		ResourceManager* resMan = renderer->GetResourceManager();

		// set viewport
		Vec2i vp = Vec2i(m_PreviewDrawingArea.get_allocation().get_width() + 1, m_PreviewDrawingArea.get_allocation().get_height() + 1);
		Vec2i vpOffset = Vec2i(m_PreviewDrawingArea.get_allocation().get_x() - 1, m_PreviewDrawingArea.get_allocation().get_y() - 1);
		renderer->SetViewport(vp, vpOffset);

		// create shader inputs managers
		if (MenuWidget::m_arrShaderPassDesc[pass].vertexShader.program)
		{
			vsInputIdx = resMan->CreateShaderInput(MenuWidget::m_arrShaderPassDesc[pass].vertexShader.program);
			vsInput = resMan->GetShaderInput(vsInputIdx);
		}
		if (MenuWidget::m_arrShaderPassDesc[pass].pixelShader.program)
		{
			psInputIdx = resMan->CreateShaderInput(MenuWidget::m_arrShaderPassDesc[pass].pixelShader.program);
			psInput = resMan->GetShaderInput(psInputIdx);
		}

		// calculate transform matrices
		worldMat = makeTrans(Vec3f(0, 0, 200), Type2Type<Matrix44f>());// *makeRot(AxisAnglef(Math::PI_OVER_2, Vec3f(1.f, 0.f, 0.f)), Type2Type<Matrix44f>());
		normalize(m_vMove);
		m_tCamera.vPos -=
			Vec3f(m_tCamera.mRot[2][0] * m_vMove[2] * m_fSpeedFactor, m_tCamera.mRot[2][1] * m_vMove[2] * m_fSpeedFactor, m_tCamera.mRot[2][2] * m_vMove[2] * m_fSpeedFactor) +
			Vec3f(m_tCamera.mRot[0][0] * m_vMove[0] * m_fSpeedFactor, m_tCamera.mRot[0][1] * m_vMove[0] * m_fSpeedFactor, m_tCamera.mRot[0][2] * m_vMove[0] * m_fSpeedFactor);
		viewMat = m_tCamera.mRot * makeTrans(m_tCamera.vPos, Type2Type<Matrix44f>());
		setPerspective(projMat, 55.f, (float)vp[0] / (float)vp[1], 20.f, 2000.f);
		if (Renderer::GetAPI() == API_DX9)
			Renderer::ConvertOGLProjMatToD3D(projMat);
		worldViewProjMat = projMat * viewMat * worldMat;
		
		// set transform matrices
		unsigned int handle;
		if (vsInput && vsInput->GetInputHandleByName("worldMat", handle))
		{
			vsInput->SetMatrix4x4(handle, worldMat);
		}
		if (psInput && psInput->GetInputHandleByName("worldMat", handle))
		{
			psInput->SetMatrix4x4(handle, worldMat);
		}
		if (vsInput && vsInput->GetInputHandleByName("viewMat", handle))
		{
			vsInput->SetMatrix4x4(handle, viewMat);
		}
		if (psInput && psInput->GetInputHandleByName("viewMat", handle))
		{
			psInput->SetMatrix4x4(handle, viewMat);
		}
		if (vsInput && vsInput->GetInputHandleByName("projMat", handle))
		{
			vsInput->SetMatrix4x4(handle, projMat);
		}
		if (psInput && psInput->GetInputHandleByName("projMat", handle))
		{
			psInput->SetMatrix4x4(handle, projMat);
		}
		if (vsInput && vsInput->GetInputHandleByName("worldViewProjMat", handle))
		{
			vsInput->SetMatrix4x4(handle, worldViewProjMat);
		}
		if (psInput && psInput->GetInputHandleByName("worldViewProjMat", handle))
		{
			psInput->SetMatrix4x4(handle, worldViewProjMat);
		}

		// set other constants
		for (unsigned int i = 0; i < MenuWidget::m_arrShaderPassDesc[pass].constants.size(); i++)
		{
			if (MenuWidget::m_arrShaderPassDesc[pass].constants[i].name == "worldMat" ||
				MenuWidget::m_arrShaderPassDesc[pass].constants[i].name == "viewMat" ||
				MenuWidget::m_arrShaderPassDesc[pass].constants[i].name == "projMat" ||
				MenuWidget::m_arrShaderPassDesc[pass].constants[i].name == "worldViewProjMat"
				)
				continue;

			if (vsInput && vsInput->GetInputHandleByName(MenuWidget::m_arrShaderPassDesc[pass].constants[i].name.c_str(), handle))
			{
				SetInput(vsInput, handle, pass, i);
			}
			if (psInput && psInput->GetInputHandleByName(MenuWidget::m_arrShaderPassDesc[pass].constants[i].name.c_str(), handle))
			{
				SetInput(psInput, handle, pass, i);
			}
		}

		// clear backbuffer
		renderer->Clear(Vec4f(0.f, 0.f, 0.f, 0.f), 1.f, 0);

		// begin frame
		if (renderer->BeginFrame())
		{
			// bind shaders
			if (vsInput && MenuWidget::m_arrShaderPassDesc[pass].vertexShader.program)
				MenuWidget::m_arrShaderPassDesc[pass].vertexShader.program->Enable(vsInput->GetInputCount() ? vsInput : nullptr);
			if (psInput && MenuWidget::m_arrShaderPassDesc[pass].pixelShader.program)
				MenuWidget::m_arrShaderPassDesc[pass].pixelShader.program->Enable(psInput->GetInputCount() ? psInput : nullptr);

			// draw vertex buffer
			if (MenuWidget::m_arrShaderPassDesc[pass].model)
				renderer->DrawVertexBuffer(MenuWidget::m_arrShaderPassDesc[pass].model);

			// unbind shaders
			if (MenuWidget::m_arrShaderPassDesc[pass].vertexShader.program)
				MenuWidget::m_arrShaderPassDesc[pass].vertexShader.program->Disable();
			if (MenuWidget::m_arrShaderPassDesc[pass].pixelShader.program)
				MenuWidget::m_arrShaderPassDesc[pass].pixelShader.program->Disable();

			// end frame and present backbuffer
			renderer->EndFrame();
			renderer->SwapBuffers();
		}

		if (vsInputIdx != ~0u)
			resMan->ReleaseShaderInput(vsInputIdx);
		if (psInputIdx != ~0u)
			resMan->ReleaseShaderInput(psInputIdx);
	}

	return true;
}

void MainWindow::CenterPointerInWidget(Gtk::Widget* widget)
{
	Vec2i absPos;
	get_position(absPos[0], absPos[1]);
	widget->get_window()->get_origin(absPos[0], absPos[1]);

	GdkDisplay *display = NULL;
	GdkScreen *screen = NULL;
	/* get default display and screen */
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);

	/* set new cusor position */
	gdk_display_warp_pointer(
		display, screen,
		absPos[0] + widget->get_width() / 2,
		absPos[1] + widget->get_height() / 2
		);
}

bool MainWindow::OnButtonPress(GdkEventButton* ev)
{
	if (ev->type == GDK_BUTTON_PRESS && ev->button == 1)
		m_bLeftClick = true;
	if (ev->type == GDK_BUTTON_PRESS && ev->button == 3)
		m_bRightClick = true;

	m_PreviewDrawingArea.grab_focus();
	m_PreviewDrawingArea.get_window()->set_cursor(Gdk::Cursor(gdk_cursor_new(GDK_BLANK_CURSOR)));

	CenterPointerInWidget(&m_PreviewDrawingArea);

	return true;
}

bool MainWindow::OnButtonRelease(GdkEventButton* ev)
{
	if (ev->type == GDK_BUTTON_RELEASE && ev->button == 1)
		m_bLeftClick = false;
	if (ev->type == GDK_BUTTON_RELEASE && ev->button == 3)
		m_bRightClick = false;
	m_PreviewDrawingArea.get_window()->set_cursor();
	return true;
}

bool MainWindow::OnMouseMove(GdkEventMotion* ev)
{
	Vec2i delta = Vec2i((int)ev->x, (int)ev->y) - Vec2i(m_PreviewDrawingArea.get_width() / 2, m_PreviewDrawingArea.get_height() / 2);
	if (m_bLeftClick)
	{
		std::cout << delta[0] << " " << delta[1] << std::endl;
		m_tCamera.mRot = makeRot(EulerAngleXYZf(Math::deg2Rad(-(float)delta[1] / 10.f), Math::deg2Rad(-(float)delta[0] / 10.f), 0), Type2Type<Matrix44f>()) * m_tCamera.mRot;

		CenterPointerInWidget(&m_PreviewDrawingArea);
	}
	else if (m_bRightClick)
	{
		m_tCamera.mRot = makeRot(EulerAngleXYZf(0.f, 0.f, Math::deg2Rad((float)delta[0] / 10.f)), Type2Type<Matrix44f>()) * m_tCamera.mRot;
		
		CenterPointerInWidget(&m_PreviewDrawingArea);
	}
	return true;
}

bool MainWindow::OnKeyPress(GdkEventKey* ev)
{
	if (ev->keyval == GDK_w || ev->keyval == GDK_W)
		m_vMove[2] = 1.f;
	if (ev->keyval == GDK_s || ev->keyval == GDK_S)
		m_vMove[2] = -1.f;
	if (ev->keyval == GDK_a || ev->keyval == GDK_A)
		m_vMove[0] = -1.f;
	if (ev->keyval == GDK_d || ev->keyval == GDK_D)
		m_vMove[0] = 1.f;
	if (ev->keyval == GDK_Shift_L || ev->keyval == GDK_Shift_R)
		m_fSpeedFactor = 5.f;
	return true;
}

bool MainWindow::OnKeyRelease(GdkEventKey* ev)
{
	if (ev->keyval == GDK_w || ev->keyval == GDK_W)
		m_vMove[2] = 0.f;
	if (ev->keyval == GDK_s || ev->keyval == GDK_S)
		m_vMove[2] = 0.f;
	if (ev->keyval == GDK_a || ev->keyval == GDK_A)
		m_vMove[0] = 0.f;
	if (ev->keyval == GDK_d || ev->keyval == GDK_D)
		m_vMove[0] = 0.f;
	if (ev->keyval == GDK_Shift_L || ev->keyval == GDK_Shift_R)
		m_fSpeedFactor = 1.f;
	return true;
}

void MainWindow::CompileShader()
{
	Renderer *renderer = Renderer::GetInstance();
	ResourceManager* resMan = renderer->GetResourceManager();

	for (unsigned int i = 0; i < m_MenuWidget->m_arrShaderPassDesc.size(); i++)
	{
		char vertexErrors[2048] = "";
		char pixelErrors[2048] = "";
		// Compile and create the vertex program
		const unsigned int vspIdx = resMan->CreateShaderProgram(SPT_VERTEX, m_MenuWidget->m_arrShaderPassDesc[i].vertexShader.src.c_str(), vertexErrors);
		// Compile and create the pixel program
		const unsigned int pspIdx = resMan->CreateShaderProgram(SPT_PIXEL, m_MenuWidget->m_arrShaderPassDesc[i].pixelShader.src.c_str(), pixelErrors);

		if (strlen(vertexErrors) || strlen(pixelErrors))
		{
			std::cout << "Error compiling shaders!" << std::endl;
			std::string vErr = vertexErrors, pErr = pixelErrors;
			m_OutputLabel.set_text(
				(vErr.length() ? ("Error in pass \"" +
				m_MenuWidget->m_arrShaderPassDesc[i].name +
				"\" - Vertex shader:\n") : "") +
				vErr +
				((vErr.length() && pErr.length()) ? "\n" : "") +
				(pErr.length() ? ("Error in pass \"" +
				m_MenuWidget->m_arrShaderPassDesc[i].name +
				"\" - Pixel shader:\n") : "") +
				pErr
				);
			return;
		}
		else
		{
			std::cout << "Successfully compiled shaders!" << std::endl;
			m_OutputLabel.set_text("Successfully compiled shaders!");
		}

		// Create a shader template for our vertex program
		const unsigned int vstIdx = resMan->CreateShaderTemplate(resMan->GetShaderProgram(vspIdx));
		m_MenuWidget->m_arrShaderPassDesc[i].vertexShader.program = resMan->GetShaderTemplate(vstIdx);
		// Create a shader template for our pixel program
		const unsigned int pstIdx = resMan->CreateShaderTemplate(resMan->GetShaderProgram(pspIdx));
		m_MenuWidget->m_arrShaderPassDesc[i].pixelShader.program = resMan->GetShaderTemplate(pstIdx);

		std::vector<ShaderInputDesc> inputsDesc;
		std::vector<ShaderInputDesc> vertexInputsDesc = resMan->GetShaderTemplate(vstIdx)->GetConstantTable();
		std::vector<ShaderInputDesc> pixelInputsDesc = resMan->GetShaderTemplate(pstIdx)->GetConstantTable();
		inputsDesc.insert(inputsDesc.end(), vertexInputsDesc.begin(), vertexInputsDesc.end());
		inputsDesc.insert(inputsDesc.end(), pixelInputsDesc.begin(), pixelInputsDesc.end());
		m_MenuWidget->UpdateConstantTable(i, inputsDesc);
	}
}

void MainWindow::RunShader()
{

}