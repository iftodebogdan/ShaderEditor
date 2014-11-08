//////////////////////////////////////////////////////////////////////////
// This file is part of the "LibRenderer" 3D graphics library           //
//                                                                      //
// Copyright (C) 2014 - Iftode Bogdan-Marius <iftode.bogdan@gmail.com>  //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "RendererDX9.h"

#include "Resources/VertexFormatDX9.h"
#include "Resources/VertexBufferDX9.h"
using namespace LibRendererDll;

RendererDX9::RendererDX9()
	: m_pD3D(nullptr)
	, m_pd3dDevice(nullptr)
{}

RendererDX9::~RendererDX9()
{
	ReleaseResources();

	ULONG refCount = 0;

	if (m_pd3dDevice)
		refCount = m_pd3dDevice->Release();
	assert(refCount == 0);

	if (m_pD3D)
		refCount = m_pD3D->Release();
	assert(refCount == 0);
}

void RendererDX9::Initialize(void* hWnd, int backBufferWidth, int backBufferHeight)
{
	// Create the D3D object, which is needed to create the D3DDevice.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	assert(m_pD3D);

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
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	
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
		m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (HWND)hWnd,
			BehaviorFlags,
			&d3dpp, &m_pd3dDevice);
	}

	assert(m_pd3dDevice);

	CreateResources();
}

void RendererDX9::CreateResources()
{
	// Turn off culling, so we see the front and back of the triangle
	HRESULT hr = m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	assert(SUCCEEDED(hr));
	// Turn off lighting
	hr = m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	assert(SUCCEEDED(hr));

	// Set render parameters
	GetBackBufferSize(m_RenderData.backBufferSize);
	GetViewport(m_RenderData.viewport);
	m_RenderData.dstRect.topLeft = Vec2i(0, 0);
	m_RenderData.dstRect.bottomRight = Vec2i(MAXINT, MAXINT);

	vf = new VertexFormatDX9(2);
	vf->Initialize(
		VertexFormat::VAU_POSITION, VertexFormat::VAT_FLOAT3, 0,
		VertexFormat::VAU_COLOR, VertexFormat::VAT_UBYTE4, 0);
	vf->Update();

	vb = new VertexBufferDX9(vf, 3);
	vb->Position<Vec3f>(0) = Vec3f(-1.f, 0.f, 0.f);
	vb->Color<D3DCOLOR>(0, 0) = D3DCOLOR_XRGB(255, 0, 0);
	vb->Position<Vec3f>(1) = Vec3f(1.f, 0.f, 0.f);
	vb->Color<D3DCOLOR>(1, 0) = D3DCOLOR_XRGB(0, 255, 0);
	vb->Position<Vec3f>(2) = Vec3f(0.f, 1.f, 0.f);
	vb->Color<D3DCOLOR>(2, 0) = D3DCOLOR_XRGB(0, 0, 255);
	vb->Lock(VertexBuffer::BL_READ_WRITE);
	vb->Update();
	vb->Unlock();
}

void RendererDX9::ReleaseResources()
{
	delete vb;
	delete vf;
}

void RendererDX9::GetBackBufferSize(Vec2i& backBufferSize)
{
	assert(m_pd3dDevice);

	// Get swap chain
	LPDIRECT3DSWAPCHAIN9 sc;
	HRESULT hr = m_pd3dDevice->GetSwapChain(0, &sc);
	assert(SUCCEEDED(hr));

	// Get present parameters
	D3DPRESENT_PARAMETERS pp;
	hr = sc->GetPresentParameters(&pp);
	assert(SUCCEEDED(hr));

	// Get back buffer size
	backBufferSize[0] = pp.BackBufferWidth;
	backBufferSize[1] = pp.BackBufferHeight;

	ULONG refCount = 0;
	refCount = sc->Release();
	assert(refCount == 0);
}

void RendererDX9::SetBackBufferSize(const Vec2i& backBufferSize)
{
	assert(m_pd3dDevice);

	// Get swap chain
	LPDIRECT3DSWAPCHAIN9 sc;
	HRESULT hr = m_pd3dDevice->GetSwapChain(0, &sc);
	assert(SUCCEEDED(hr));
	
	// Get present parameters
	D3DPRESENT_PARAMETERS pp;
	hr = sc->GetPresentParameters(&pp);
	assert(SUCCEEDED(hr));

	ULONG refCount = 0;
	refCount = sc->Release();
	assert(refCount == 0);

	// Set back buffer size
	pp.BackBufferWidth = backBufferSize[0];
	pp.BackBufferHeight = backBufferSize[1];
	
	// Destroy resources
	ReleaseResources();

	// Reset the device
	hr = m_pd3dDevice->Reset(&pp);
	assert(SUCCEEDED(hr));

	CreateResources();
}

void RendererDX9::GetViewport(RenderData::Viewport& viewport)
{
	D3DVIEWPORT9 vp;
	HRESULT hr = m_pd3dDevice->GetViewport(&vp);
	assert(SUCCEEDED(hr));

	viewport.topLeft = Vec2i(vp.X, vp.Y);
	viewport.sizeWH = Vec2i(vp.Width, vp.Height);
	viewport.minMaxZ = Vec2f(vp.MinZ, vp.MaxZ);
}

void RendererDX9::SetViewport(const RenderData::Viewport& viewport)
{
	assert(m_pd3dDevice);

	D3DVIEWPORT9 vp;

	vp.X = viewport.topLeft[0];
	vp.Y = viewport.topLeft[1];
	vp.Width = viewport.sizeWH[0];
	vp.Height = viewport.sizeWH[1];
	vp.MinZ = viewport.minMaxZ[0];
	vp.MaxZ = viewport.minMaxZ[1];

	HRESULT hr = m_pd3dDevice->SetViewport(&vp);
	assert(SUCCEEDED(hr));
}

void RendererDX9::SetRenderData(const RenderData& renderParams)
{
	// Update the backbuffer
	if (renderParams.backBufferSize != m_RenderData.backBufferSize)
		SetBackBufferSize(renderParams.backBufferSize);

	// Update the viewport
	if (m_RenderData.viewport.topLeft != renderParams.viewport.topLeft ||
		m_RenderData.viewport.sizeWH != renderParams.viewport.sizeWH ||
		m_RenderData.viewport.minMaxZ != renderParams.viewport.minMaxZ)
		SetViewport(m_RenderData.viewport);

	m_RenderData = renderParams;
}

void RendererDX9::RenderScene()
{
	assert(m_pd3dDevice);

	HRESULT hr;

	// Clear the backbuffer and the zbuffer
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	// Begin the scene
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		// Set up world matrix
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixRotationY(&matWorld, (float)GetTickCount() / 1000.0f);
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
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, aspectRatio, 1.0f, 50.0f);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
		
		vb->Enable();
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		vb->Disable();

		// End the scene
		hr = m_pd3dDevice->EndScene();
		assert(SUCCEEDED(hr));
	}

	// Present the backbuffer contents to the display
	RECT dstRect;
	dstRect.left = m_RenderData.dstRect.topLeft[0];
	dstRect.top = m_RenderData.dstRect.topLeft[1];
	dstRect.right = m_RenderData.dstRect.bottomRight[0];
	dstRect.bottom = m_RenderData.dstRect.bottomRight[1];
	
	hr = m_pd3dDevice->Present(NULL, &dstRect, NULL, NULL);
	assert(SUCCEEDED(hr));
}
