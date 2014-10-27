#include "stdafx.h"

#include "RendererDX9.h"
using namespace LibRendererDll;

RendererDX9::RendererDX9()
	:
	m_pD3D(nullptr),
	m_pd3dDevice(nullptr)
{
	m_pDstRect.bottom = 0;
	m_pDstRect.left = 0;
	m_pDstRect.right = 0;
	m_pDstRect.top = 0;
}

RendererDX9::~RendererDX9()
{
	if (m_pd3dDevice != nullptr)
		m_pd3dDevice->Release();

	if (m_pD3D != nullptr)
		m_pD3D->Release();
}

bool RendererDX9::Initialize(void* hWnd)
{
	// Create the D3D object, which is needed to create the D3DDevice.
	if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return false;

	// Set up the structure used to create the D3DDevice. Most parameters are
	// zeroed out. We set Windowed to TRUE, since we want to do D3D in a
	// window, and then set the SwapEffect to "discard", which is the most
	// efficient method of presenting the back buffer to the display. And 
	// we request a back buffer format that matches the current desktop display 
	// format.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	
	// Create the Direct3D device. Here we are using the default adapter (most
	// systems only have one, unless they have multiple graphics hardware cards
	// installed) and requesting the HAL (which is saying we want the hardware
	// device rather than a software one). Software vertex processing is 
	// specified since we know it will work on all cards. On cards that support 
	// hardware vertex processing, though, we would see a big performance gain 
	// by specifying hardware vertex processing.

	//################# EXPERIMENTAL FIX ################################
	//-----------------------------------------------------------------------------
	// There is a problem with the function in GTK's graphics library, Cairo:
	//		static inline cairo_fixed_t _cairo_fixed_from_double (double d)
	// It relies on some floating point math that stops working when
	// computations are done with lower accuracy. Windows changes some
	// floating point precision flags when initializing a Direct3D device,
	// causing Cairo to malfunction and stop rendering our window. This
	// behaviour has only been encountered in the 32-bit version of the app.
	// Unfortunately, using D3DCREATE_FPU_PRESERVE may reduce Direct3D performance
	// in double-precision mode and could also cause undefined behavior.
	// http://msdn.microsoft.com/en-us/library/bb172527(VS.85).aspx
#if defined(_WIN32) && !defined(_WIN64)
	DWORD BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE;
#elif defined(_WIN32) && defined(_WIN64)
	DWORD BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
#endif
	//------------------------------------------------------------------------------

	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWnd,
		BehaviorFlags,
		&d3dpp, &m_pd3dDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWnd,
			BehaviorFlags,
			&d3dpp, &m_pd3dDevice)))
		{
			return false;
		}
	}

	// Device state would normally be set here

	return true;
}

void RendererDX9::SetBackBufferSize(int sizeX, int sizeY)
{
	if (m_pd3dDevice == nullptr)
		return;

	// Get swap chain
	LPDIRECT3DSWAPCHAIN9 sc;
	m_pd3dDevice->GetSwapChain(0, &sc);

	// Get present parameters
	D3DPRESENT_PARAMETERS pp;
	sc->GetPresentParameters(&pp);

	// Set back bufer size
	pp.BackBufferHeight = sizeX;
	pp.BackBufferWidth = sizeY;

	// Reset the device
	m_pd3dDevice->Reset(&pp);
}

void RendererDX9::SetDestRect(int left, int right, int top, int bottom)
{
	m_pDstRect.bottom = bottom;
	m_pDstRect.left = left;
	m_pDstRect.right = right;
	m_pDstRect.top = top;
}

void RendererDX9::RenderScene()
{
	if (m_pd3dDevice == nullptr)
		return;

	// Clear the backbuffer to a blue color
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		// Rendering of scene objects can happen here

		struct CUSTOMVERTEX { FLOAT X, Y, Z; };
		CUSTOMVERTEX vertices[] =
		{
			{  0.0f,  0.6f, 0.f, },
			{  0.6f, -0.6f, 0.f, },
			{ -0.6f, -0.6f, 0.f, },
		};

		LPDIRECT3DVERTEXBUFFER9 v_buffer = NULL;    // the pointer to the vertex buffer

		// create a vertex buffer interface called v_buffer
		m_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
			0,
			D3DFVF_XYZ,
			D3DPOOL_MANAGED,
			&v_buffer,
			NULL);

		VOID* pVoid;    // a void pointer

		// lock v_buffer and load the vertices into it
		v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, vertices, sizeof(vertices));
		v_buffer->Unlock();

		// select which vertex format we are using
		m_pd3dDevice->SetFVF(D3DFVF_XYZ);

		// select the vertex buffer to display
		m_pd3dDevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));

		// copy the vertex buffer to the back buffer
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		// End the scene
		m_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	if (m_pDstRect.bottom == 0 && m_pDstRect.left == 0 && m_pDstRect.right == 0 && m_pDstRect.top == 0)
		m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	else
		m_pd3dDevice->Present(NULL, &m_pDstRect, NULL, NULL);
}
