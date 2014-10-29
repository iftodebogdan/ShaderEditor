#include "stdafx.h"

#include <d3dx9.h>

#include "RendererDX9.h"
using namespace LibRendererDll;

// A structure for our custom vertex type. We added a normal, and omitted the
// color (which is provided by the material)
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; // The 3D position for the vertex
	D3DXVECTOR3 normal;   // The surface normal for the vertex
	D3DCOLOR	color;    // The color
	FLOAT		u, v;   // The texture coordinates
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

RendererDX9::RendererDX9()
	: m_pD3D(nullptr)
	, m_pd3dDevice(nullptr)
	, m_pVertexBuffer(nullptr)
	, m_pTexture(nullptr)
{}

RendererDX9::~RendererDX9()
{
	ReleaseResources();

	if (m_pd3dDevice)
		m_pd3dDevice->Release();

	if (m_pD3D)
		m_pD3D->Release();
}

const bool RendererDX9::Initialize(void* hWnd, int backBufferWidth, int backBufferHeight)
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
	d3dpp.BackBufferWidth = backBufferWidth;
	d3dpp.BackBufferHeight = backBufferHeight;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	
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

	CreateResources();
	
	return true;
}

const bool RendererDX9::CreateResources()
{
	// Use D3DX to create a texture from a file based image
	if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, L"banana.bmp", &m_pTexture)))
	{
		// If texture is not in current folder, try parent folder
		if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, L"..\\banana.bmp", &m_pTexture)))
			return false;
	}

	// Create the vertex buffer.
	if (FAILED(m_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL)))
		return false;

	// Turn off culling, so we see the front and back of the triangle
	if (FAILED(m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
		return false;
	// Turn on the zbuffer
	if (FAILED(m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE)))
		return false;
	// Normalize normals
	if (FAILED(m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE)))
		return false;

	// Set render parameters
	if (!GetBackBufferSize(m_RenderData.backBufferSize))
		return false;
	if (!GetViewport(m_RenderData.viewport))
		return false;
	m_RenderData.dstRect.topLeft = Vec2i(0, 0);
	m_RenderData.dstRect.bottomRight = Vec2i(MAXINT, MAXINT);

	return true;
}

const bool RendererDX9::ReleaseResources()
{

	if (m_pTexture && FAILED(m_pTexture->Release()))
		return false;

	if (m_pVertexBuffer && FAILED(m_pVertexBuffer->Release()))
		return false;

	return true;
}

const bool RendererDX9::GetBackBufferSize(Vec2i& backBufferSize)
{
	if (m_pd3dDevice == nullptr)
		return false;

	// Get swap chain
	LPDIRECT3DSWAPCHAIN9 sc;
	if (FAILED(m_pd3dDevice->GetSwapChain(0, &sc)))
		return false;

	// Get present parameters
	D3DPRESENT_PARAMETERS pp;
	if (FAILED(sc->GetPresentParameters(&pp)))
		return false;

	// Get back buffer size
	backBufferSize[0] = pp.BackBufferWidth;
	backBufferSize[1] = pp.BackBufferHeight;

	sc->Release();

	return true;
}

const bool RendererDX9::SetBackBufferSize(const Vec2i& backBufferSize)
{
	if (m_pd3dDevice == nullptr)
		return false;

	// Get swap chain
	LPDIRECT3DSWAPCHAIN9 sc;
	if (FAILED(m_pd3dDevice->GetSwapChain(0, &sc)))
		return false;
	
	// Get present parameters
	D3DPRESENT_PARAMETERS pp;
	if (FAILED(sc->GetPresentParameters(&pp)))
		return false;

	sc->Release();

	// Set back buffer size
	pp.BackBufferWidth = backBufferSize[0];
	pp.BackBufferHeight = backBufferSize[1];
	
	// Destroy resources
	ReleaseResources();

	// Reset the device
	if (FAILED(m_pd3dDevice->Reset(&pp)))
		return false;

	CreateResources();

	return true;
}

const bool RendererDX9::GetViewport(RenderData::Viewport& viewport)
{
	D3DVIEWPORT9 vp;
	if (FAILED(m_pd3dDevice->GetViewport(&vp)))
		return false;

	viewport.topLeft = Vec2i(vp.X, vp.Y);
	viewport.sizeWH = Vec2i(vp.Width, vp.Height);
	viewport.minMaxZ = Vec2f(vp.MinZ, vp.MaxZ);
	
	return true;
}

const bool RendererDX9::SetViewport(const RenderData::Viewport& viewport)
{
	if (m_pd3dDevice == nullptr)
		return false;

	D3DVIEWPORT9 vp;

	vp.X = viewport.topLeft[0];
	vp.Y = viewport.topLeft[1];
	vp.Width = viewport.sizeWH[0];
	vp.Height = viewport.sizeWH[1];
	vp.MinZ = viewport.minMaxZ[0];
	vp.MaxZ = viewport.minMaxZ[1];

	return SUCCEEDED(m_pd3dDevice->SetViewport(&vp));
}

const bool RendererDX9::SetRenderData(const RenderData& renderParams)
{
	// Update the backbuffer
	if (renderParams.backBufferSize != m_RenderData.backBufferSize)
		if (!SetBackBufferSize(renderParams.backBufferSize))
			return false;

	// Update the viewport
	if (m_RenderData.viewport.topLeft != renderParams.viewport.topLeft ||
		m_RenderData.viewport.sizeWH != renderParams.viewport.sizeWH ||
		m_RenderData.viewport.minMaxZ != renderParams.viewport.minMaxZ)
		if (!SetViewport(m_RenderData.viewport))
			return false;

	m_RenderData = renderParams;

	return true;
}

const bool RendererDX9::RenderScene()
{
	if (!m_pd3dDevice || !m_pVertexBuffer)
		return false;

	// Clear the backbuffer and the zbuffer
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	// Fill the vertex buffer. We are setting the tu and tv texture
	// coordinates, which range from 0.0 to 1.0
	CUSTOMVERTEX* pVertices;
	if (FAILED(m_pVertexBuffer->Lock(0, 0, (void**)&pVertices, 0)))
		return false;
	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);

		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
		pVertices[2 * i + 0].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
		pVertices[2 * i + 0].color = 0xffffffff;
		pVertices[2 * i + 0].u = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 0].v = 1.0f;

		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
		pVertices[2 * i + 1].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
		pVertices[2 * i + 1].color = 0xff808080;
		pVertices[2 * i + 1].u = ((FLOAT)i) / (50 - 1);
		pVertices[2 * i + 1].v = 0.0f;
	}
	m_pVertexBuffer->Unlock();

	// Begin the scene
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		// Set up a material. The material here just has the diffuse and ambient
		// colors set to yellow. Note that only one material can be used at a time.
		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		m_pd3dDevice->SetMaterial(&mtrl);

		// Set up a white, directional light, with an oscillating direction.
		// Note that many Lights may be active at a time (but each one slows down
		// the rendering of our scene). However, here we are just using one. Also,
		// we need to set the D3DRS_LIGHTING renderstate to enable lighting
		D3DXVECTOR3 vecDir;
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		vecDir = D3DXVECTOR3(cosf(GetTickCount() / 350.0f),
			1.0f,
			sinf(GetTickCount() / 350.0f));
		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
		light.Range = 1000.0f;
		m_pd3dDevice->SetLight(0, &light);
		m_pd3dDevice->LightEnable(0, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// Finally, turn on some ambient light.
		m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);

		// Set up world matrix
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixRotationX(&matWorld, GetTickCount() / 1000.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the
		// origin, and define "up" to be in the y-direction.
		D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMATRIXA16 matProj;
		float aspectRatio = (float)m_RenderData.backBufferSize[0] / (float)m_RenderData.backBufferSize[1];
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, aspectRatio, 1.0f, 100.0f);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		// Setup our texture. Using Textures introduces the texture stage states,
		// which govern how Textures get blended together (in the case of multiple
		// Textures) and lighting information. In this case, we are modulating
		// (blending) our texture with the diffuse color of the vertices.
		m_pd3dDevice->SetTexture(0, m_pTexture);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		// Render the vertex buffer contents
		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CUSTOMVERTEX));
		m_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);

		// End the scene
		if (FAILED(m_pd3dDevice->EndScene()))
			return false;
	}
	else
		return false;

	// Present the backbuffer contents to the display
	RECT dstRect;
	dstRect.left = m_RenderData.dstRect.topLeft[0];
	dstRect.top = m_RenderData.dstRect.topLeft[1];
	dstRect.right = m_RenderData.dstRect.bottomRight[0];
	dstRect.bottom = m_RenderData.dstRect.bottomRight[1];
	
	if (FAILED(m_pd3dDevice->Present(NULL, &dstRect, NULL, NULL)))
		return false;

	return true;
}
