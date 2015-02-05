#include "stdafx.h"

#include "Menu.h"
#include "MainWindow.h"
#include "AddStepPrompt.h"
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
	, m_AddStepButton("Add Step")
	, m_RemoveStepButton("Remove Step")
{
	// Set window properties
	set_title("ShdEd");
	set_border_width(10);

	// Ask to maximize, then show the window so that the width/height
	// are valid later when we retrieve them
	maximize();
	show();
	
	// Add the menu widget.
	this->m_MenuWidget = new MenuWidget(this, &m_MenuContainer, m_MenuItems, &m_Notebook);
	
	// Add the menu container to the menu box.
	m_MenuBox.pack_start(m_MenuContainer);

	// Add the Add & Remove buttons.
	m_AddStepButton.get_allocation().set_height(20);
	m_AddStepButton.get_allocation().set_width(20);
	m_AddStepButton.signal_clicked().connect(sigc::mem_fun(this, &MainWindow::OnAddStepButtonClicked));
	m_MenuBox.pack_start(m_AddStepButton, Gtk::PACK_SHRINK);

	m_RemoveStepButton.get_allocation().set_height(20);
	m_RemoveStepButton.get_allocation().set_width(20);
	m_RemoveStepButton.signal_clicked().connect(sigc::mem_fun(this, &MainWindow::OnRemoveStepButtonClicked));
	m_MenuBox.pack_start(m_RemoveStepButton, Gtk::PACK_SHRINK);

	// Add the menu box to the workspace frame.
	m_WorkspaceFrame.add(m_MenuBox);

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

	// The resource manager is the only way to
	// allocate resources to be used by the renderer
	// NB: when creating a resource, the resource manager
	// will return an index, with which you can retrieve
	// a pointer to the actual resource
	ResourceManager* resMan = renderer->GetResourceManager();

	// First of all, create a vertex buffer from a model file
	// A vertex buffer contains all the points we need to form
	// our triangles later when drawing
	const unsigned int vbIdx = resMan->CreateVertexBuffer(
		"models/COLLADA/teapot_instancenodes.DAE"
		//"sponza_scene/sponza.obj"
		);
	// Retrieve the actual resource
	m_pVB = resMan->GetVertexBuffer(vbIdx);

	// Create a texture from an image file
	const unsigned int tex1Idx = resMan->CreateTexture("sponza_curtain_diff.tga");
	m_pTex1 = resMan->GetTexture(tex1Idx);

	// Now, create a blank texture which we will be using later.
	// PF_A8R8G8B8 is the pixel format: 8 bits per channel in the order Alpha, Red, Green, Blue (the order counts!).
	// TT_2D means that it will be a 2D texture (as opposed  to 1D, 3D or cubemap textures).
	// 512 x 512 x 1 are the width, height and depth (just like a 1D texture is a 2D texture
	// with a height of 1, a 2D texture is a 3D texture with a depth of 1).
	// Next is the mipmap count, which is 1. Mipmaps are a sequence of textures, each a 
	// progressively lower resolution representation of the original image (mip 0).
	// Mipmaps are used to increase rendering speed (lower resolution textures increase cache coherency
	// because they are smaller in size, less texels are required to cover the same area and so
	// cache misses are less frequent).
	// Finally, BU_DYNAMIC means that the resource is dynamic, or in other words
	// will be often accessed by the CPU for reading/writing (it's not cheap
	// to copy memory from video RAM to system RAM and back again).
	// As such, we request the driver to allocate it in AGP memory, which is a part of the
	// system memory in which the CPU can write quickly and the GPU can read relatively quickly.
	const unsigned int tex2Idx = resMan->CreateTexture(PF_A8R8G8B8, TT_2D, 512, 512, 1, 1, BU_DYNAMIC);
	m_pTex2 = resMan->GetTexture(tex2Idx);

	// Read the contents of the file
	std::ifstream t("test.hlsl");
	int length;
	t.seekg(0, std::ios::end);			// go to the end
	length = (int)t.tellg();			// report location (this is the length)
	t.seekg(0, std::ios::beg);			// go back to the beginning
	char* buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
	t.read(buffer, length);				// read the whole file into the buffer
	t.close();							// close file handle
	buffer[t.gcount()] = '\0';			// maybe there where less characters than expected (CRLF converted to LF)

	// Compile and create the vertex program
	const unsigned int vspIdx = resMan->CreateShaderProgram(SPT_VERTEX, buffer);
	// Compile and create the pixel program
	const unsigned int pspIdx = resMan->CreateShaderProgram(SPT_PIXEL, buffer);
	// Create a shader template for our vertex program
	const unsigned int vstIdx = resMan->CreateShaderTemplate(resMan->GetShaderProgram(vspIdx));
	m_pVertexShader = resMan->GetShaderTemplate(vstIdx);
	// Create a shader template for our pixel program
	const unsigned int pstIdx = resMan->CreateShaderTemplate(resMan->GetShaderProgram(pspIdx));
	m_pPixelShader = resMan->GetShaderTemplate(pstIdx);

	// Shader inputs are separated from templates so that one could use the same
	// shader with different inputs, without having to create multiple copies of
	// the same template. A ShaderTemplate is a descriptor for what the shader expects
	// as input, whereas a ShaderInput is the instance of those inputs.
	const unsigned int vsiIdx = resMan->CreateShaderInput(m_pVertexShader);
	m_pVSInput = resMan->GetShaderInput(vsiIdx);
	const unsigned int psiIdx = resMan->CreateShaderInput(m_pPixelShader);
	m_pPSInput = resMan->GetShaderInput(psiIdx);

	// Create a render target
	// If the function parameters remind you of the ones from CreateTexture(),
	// it's because a render target is also a texture! ...sort of.
	// Unlike regular textures, the renderer doesn't store a copy of the data
	// in system memory (nor does the driver for that matter), since it is
	// composited by the GPU in GPU memory. Copying the data to a system RAM
	// is VERY expensive and should definitely not be done on a per-frame
	// basis (we're going to do it anyway!).
	// A render target can contain a maximum of 4 color buffer and 1 depth
	// buffer (which you can visualise as 5 textures). A pixel shader can
	// render to all these buffers simultaneously with a single draw call!
	const unsigned int rtIdx = resMan->CreateRenderTarget(1, PF_A8R8G8B8, 512, 512, false, true);
	m_pRT = resMan->GetRenderTarget(rtIdx);

	delete[] buffer;
}

bool MainWindow::OnUpdate()
{
	// After creating the resources required for compositing the scene, let's build our frame
	Renderer *renderer = Renderer::GetInstance();
	// First off, set our viewport to the size and offset of the widget containing our renderer
	Vec2i vp = Vec2i(m_PreviewDrawingArea.get_allocation().get_width() + 1, m_PreviewDrawingArea.get_allocation().get_height() + 1);
	Vec2i vpOffset = Vec2i(m_PreviewDrawingArea.get_allocation().get_x() - 1, m_PreviewDrawingArea.get_allocation().get_y() - 1);
	renderer->SetViewport(vp, vpOffset);

	unsigned int handle;
	//Obtain a handle for our shader inputs
	if (m_pVSInput->GetInputHandleByName("matWVP", handle))
	{
		// Here we will build 3 matrices:
		// *the world matrix which positions/rotates/scales our object in the world
		// *the view matrix which determines the position and rotation of the camera (i.e. transforms world coordinates into local camera coordinates)
		// *the projection matrix which determines the kind of projection (orthographic or perspective)
		Matrix44f matWVP[3];

		// First, we build a world matrix by composing a rotation matrix with a translation matrix.
		// The order in which we compose the matrices matter, so here, since GMTL matrices are column-major,
		// the final world matrix will FIRST rotate the object by 90 degrees on the X axis and then
		// translate it 200 units on the Z axis.
		matWVP[0] = makeTrans(Vec3f(0, 0, 200), Type2Type<Matrix44f>()) * makeRot(AxisAnglef(Math::PI_OVER_2, Vec3f(1.f, 0.f, 0.f)), Type2Type<Matrix44f>());

		// Now create our view matrix, which actually is the INVERSE world matrix of the camera.
		// So we could either build a world matrix for the camera, then invert it, or, as we do here,
		// we build a matrix with inverse translation and rotation from the get-go. In other words,
		// if we want our camera to be at position x, y, z then we need to build a matrix that
		// translates to -x, -y, -z
		normalize(m_vMove);
		m_tCamera.vPos -= 
			Vec3f(m_tCamera.mRot[2][0] * m_vMove[2] * m_fSpeedFactor, m_tCamera.mRot[2][1] * m_vMove[2] * m_fSpeedFactor, m_tCamera.mRot[2][2] * m_vMove[2] * m_fSpeedFactor) +
			Vec3f(m_tCamera.mRot[0][0] * m_vMove[0] * m_fSpeedFactor, m_tCamera.mRot[0][1] * m_vMove[0] * m_fSpeedFactor, m_tCamera.mRot[0][2] * m_vMove[0] * m_fSpeedFactor);
		matWVP[1] = m_tCamera.mRot * makeTrans(m_tCamera.vPos, Type2Type<Matrix44f>());

		// Create the projection matrix according to the convention of the rendering API used.
		// A projection matrix transforms view-space coordinates into perspective-correct (if
		// using a perspective matrix) clip-space coordinates. What it does is transform a
		// frustum into a cuboid (cube in OpenGL) with edges ranging between (-1, 1)
		// for X and Y coordinates and (0, 1) for Z coordinate. Any geometry that falls
		// outside of this range after application of the projection matrix gets clipped 
		// (this is why it's called clip-space) before rasterization.
		renderer->CreateProjectionMatrix(matWVP[2], Math::deg2Rad(55.f), (float)vp[0] / (float)vp[1], 20.f, 2000.f);
		
		// Set our matrices as input to the vertex shader, which will then transform
		// each vertex from the vertex buffer
		m_pVSInput->SetMatrixArray<4, 4>(handle, matWVP);
	}

	if (m_pPSInput->GetInputHandleByName("tex1", handle))
	{
		// Set the texture as an input to the pixel shader
		m_pPSInput->SetTexture(handle, m_pTex1);
		// Also set the sampling filter to linearly interpolate between texels and mips
		renderer->GetSamplerStateManager()->SetFilter(m_pPSInput->GetInputDesc(handle).nRegisterIndex, SF_MIN_MAG_LINEAR_MIP_LINEAR);
	}
	if (m_pPSInput->GetInputHandleByName("tex2", handle))
	{
		// Set the second texture as an input to the pixel shader
		// Remember that this texture is blank (for the moment!)
		m_pPSInput->SetTexture(handle, m_pTex2);
	}
	
	// Clear our backbuffer so that we have a nice black background
	// and a depth value that's as far away as possible (at our Z far, which is 2000.f)
	renderer->Clear(Vec4f(255.f, 255.f, 255.f, 255.f), 1.f, 0);
	// If we couldn't begin the frame, something might have gone wrong.
	// Usually everything goes smooth as butter, but sometimes, in DX9,
	// the device can get lost (Alt+Tab, Win+L, Ctrl+Alt+Delete, etc.).
	// When this happens, all video memory allocated by the renderer
	// has to be freed, then the device reset. The renderer takes care
	// of that for you. All you have to do is remember not to try and
	// render anything while the device is in limbo, so check the
	// return value of BeginFrame() before submiting draw calls!
	if (renderer->BeginFrame())
	{
		// Set the vertex and pixel shaders used to render the scene
		m_pVertexShader->Enable(m_pVSInput);
		m_pPixelShader->Enable(m_pPSInput);
		
		// Enable rendering into an off-screen render target
		// which we will use later
		m_pRT->Enable();
		// Clear the render target, just like we cleared our backbuffer
		renderer->Clear(Vec4f(255.f, 255.f, 255.f, 255.f), 1.f, 0);
		// Draw our scene in the off-screen render target
		renderer->DrawVertexBuffer(m_pVB);
		// Restore our backbuffer
		m_pRT->Disable();
		
		// This isn't really required here, but for
		// consistency sake, we unset the shaders.
		// If we had different shaders with
		// different shader inputs for our second
		// pass, this would have been a must.
		m_pVertexShader->Disable();
		m_pPixelShader->Disable();

		// Lock our blank texture.
		// Remember when we created this texture with
		// dynamic usage? This is why: we are locking
		// it and writing in it on a per-frame basis
		// which isn't a smart thing to do!
		// For demonstration purposes it'll have to do.
		m_pTex2->Lock(0, BL_WRITE_ONLY);
		// Copy the contents of our off-screen
		// render target to the dynamnic texture
		m_pRT->CopyColorBuffer(0, m_pTex2);
		// Commit changes
		m_pTex2->Update();
		// Unlock it, copying to GPU-accessible memory
		// (note how I didn't say video memory, because
		// our dynamic texture is in AGP memory!)
		m_pTex2->Unlock();

		// Set our vertex and pixel shader
		m_pVertexShader->Enable(m_pVSInput);
		m_pPixelShader->Enable(m_pPSInput);

		// Draw the contents of the vertex buffer, this time
		// on the backbuffer. The model will be drawn using
		// both the static and dynamic textures resulting in
		// a teapot-ception (if you haven't seen the movie
		// 'Inception', you must live in a cave... on another
		// planet... in another solar system) effect.
		// Pretty cool, huh?
		renderer->DrawVertexBuffer(m_pVB);

		// Disable shaders
		m_pVertexShader->Disable();
		m_pPixelShader->Disable();

		// End the frame
		renderer->EndFrame();
		// I'm sure you are aware of the fact that that pixels on your screen are progressively
		// refreshed and not all at once. Imagine that during that refreshing time, we render into
		// the backbuffer. The image on the screen will contain a part of the old backbuffer and
		// a part of the new one, resulting in what is known as screen tearing. To avoid this,
		// we use a double buffered system in which we allocate two buffers: a backbuffer in
		// which we render to, and a frontbuffer which is presented on the screen. When we finish
		// rendering our scene into our backbuffer, we swap them at a time when the screen is not
		// refreshing. By doing this, we avoid screen tearing.
		renderer->SwapBuffers();
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

/**
 * "Add step" button click event handler.
 */
void MainWindow::OnAddStepButtonClicked()
{
	AddStepPrompt stepPrompt;
	std::map<std::string, std::string> newItem;
	std::map<int, std::map<std::string, std::string>>::iterator itemsIterator;

	// Show the 'Add step' prompt, which lets the user define a rendering step.
	Gtk::Main::run(stepPrompt);

	// Fetch the newly defined step.
	newItem["name"] = stepPrompt.getItemName();
	newItem["file"] = stepPrompt.getItemName();

	// If the menu item already exists, do not rebuild the menu.
	for (itemsIterator = this->m_MenuItems.begin(); itemsIterator != this->m_MenuItems.end(); ++itemsIterator) {
		if (itemsIterator->second["name"] == newItem["name"]) {
			return;
		}
	}
	
	// Add the new item into the menu items list.
	this->m_MenuItems[this->m_MenuItems.size()] = newItem;

	// Rebuild the menu bar.
	delete this->m_MenuWidget;
	this->m_MenuWidget = new MenuWidget(this, &m_MenuContainer, m_MenuItems, &m_Notebook);
}

void MainWindow::OnRemoveStepButtonClicked()
{
	std::string removedPageName;
	int nrPages, i;
	PageWidget* widget;

	removedPageName = this->m_MenuItems[this->m_MenuItems.size() - 1]["name"];
	nrPages = this->m_Notebook.get_n_pages();
	for (i = 0; i < nrPages; i++) {
		widget = (PageWidget*) this->m_Notebook.pages()[i].get_child();
		if (removedPageName == widget->getName()) {
			this->m_Notebook.remove(*widget);
			
			break;
		}
	}

	this->m_MenuItems.erase(this->m_MenuItems.size() - 1);

	// Rebuild the menu bar.
	delete this->m_MenuWidget;
	this->m_MenuWidget = new MenuWidget(this, &m_MenuContainer, m_MenuItems, &m_Notebook);
}