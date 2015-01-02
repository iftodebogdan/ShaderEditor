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

#include "VertexFormatDX9.h"
#include "VertexBufferDX9.h"
#include "IndexBufferDX9.h"
#include "TextureDX9.h"
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

void RendererDX9::Initialize(void* hWnd, const int backBufferWidth, const int backBufferHeight)
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

#include <Importer.hpp> // C++ importer interface
#include <scene.h> // Output data structure
#include <postprocess.h> // Post processing flags

// Create an instance of the Importer class
Assimp::Importer importer;
static const aiScene* scene = nullptr;

void RendererDX9::CreateResources()
{
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	if (scene == nullptr)
		scene = importer.ReadFile(
			"models/COLLADA/teapot_instancenodes.DAE",
			//"models-nonbsd/OBJ/rifle.obj",
			//"sponza_scene/sponza.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords);

	HRESULT hr;
	// Turn off culling, so we see the front and back of the triangle
	hr = m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Turn on the zbuffer
	hr = m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//assert(SUCCEEDED(hr));
	// Turn off lighting
	hr = m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	hr = m_pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_PHONG);
	hr = m_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	assert(SUCCEEDED(hr));

	// Set render parameters
	GetBackBufferSize(m_RenderData.backBufferSize);
	GetViewport(m_RenderData.viewport);
	m_RenderData.dstRect.topLeft = Vec2i(0, 0);
	m_RenderData.dstRect.bottomRight = Vec2i(MAXINT, MAXINT);

	tex = new TextureDX9(Texture::TF_A8R8G8B8, Texture::TT_2D, 256, 256, 16, 0, Buffer::BU_TEXTURE);
	for (unsigned int face = 0; face < (tex->GetTextureType() == Texture::TT_CUBE ? 6u : 1u); face++)
		for (unsigned int level = 0, levelCount = tex->GetMipmapLevelCount(); level < levelCount; level++)
		{
			byte* pTex;
			if (tex->GetTextureType() == Texture::TT_CUBE)
				pTex = tex->GetMipmapLevelData(face, level);
			else
				pTex = tex->GetMipmapLevelData(level);
			pTex = tex->GetData();
			int faceOffset = face * (tex->GetTextureType() == Texture::TT_CUBE ? tex->GetCubeFaceOffset() : 0);
			int mipoffset = tex->GetMipmapLevelOffset(level);
			pTex += faceOffset + mipoffset;
			for (unsigned int pixelZ = 0; pixelZ < tex->GetDepth(level); pixelZ++)
				for (unsigned int pixelY = 0; pixelY < tex->GetHeight(level); pixelY++)
					for (unsigned int pixelX = 0; pixelX < tex->GetWidth(level); pixelX++)
					{
						unsigned int pixel =
							pixelX + pixelY * tex->GetWidth(level) +
							pixelZ * tex->GetWidth(level) * tex->GetHeight(level);

						//Color test
						*(pTex + pixel * tex->GetPixelSize()) = (char)((float)pixelZ / (float)tex->GetDepth(level) * 255.f);
						*(pTex + pixel * tex->GetPixelSize() + 1) = (char)((float)pixelY / (float)tex->GetHeight(level) * 255.f);
						*(pTex + pixel * tex->GetPixelSize() + 2) = (char)((float)pixelX / (float)tex->GetWidth(level) * 255.f);
						*(pTex + pixel * tex->GetPixelSize() + 3) = 255;

						//mip test
						//*(pTex + pixel * tex->GetPixelSize() + 3)	= 255; //alpha
						//*(pTex + pixel * tex->GetPixelSize() + 2)	= (level % 6 == 0 || level % 6 == 3 || level % 6 == 3) * 255; //red
						//*(pTex + pixel * tex->GetPixelSize() + 1)	= (level % 6 == 1 || level % 6 == 3 || level % 6 == 5) * 255; //green
						//*(pTex + pixel * tex->GetPixelSize())		= (level % 6 == 2 || level % 6 == 4 || level % 6 == 5) * 255; //blue

						//cubemap test
						//*(pTex + pixel * tex->GetPixelSize())		= ((face == 0) || (face == 3) || (face == 4)) * 255;
						//*(pTex + pixel * tex->GetPixelSize() + 1)	= ((face == 1) || (face == 3) || (face == 5)) * 255;
						//*(pTex + pixel * tex->GetPixelSize() + 2)	= ((face == 2) || (face == 4) || (face == 5)) * 255;
						//*(pTex + pixel * tex->GetPixelSize() + 3)	= 255;
					}

			if (tex->GetTextureType() == Texture::TT_CUBE)
			{
				tex->Lock(face, level, Buffer::BL_WRITE_ONLY);
				tex->Update(face, level);
				tex->Unlock(face, level);
			}
			else
			{
				tex->Lock(level, Buffer::BL_WRITE_ONLY);
				tex->Update(level);
				tex->Unlock(level);
			}
		}

	//m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	m_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

	if (tex->GetTextureType() == Texture::TT_CUBE)
	{
		m_pd3dDevice->SetRenderState(D3DRS_LOCALVIEWER, FALSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT3);
	}
	else
	{
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_SPHEREMAP);
	}

	vf = new VertexFormatDX9(4);
	vf->Initialize(
		VertexFormat::VAU_POSITION, VertexFormat::VAT_FLOAT3, 0
		, VertexFormat::VAU_NORMAL, VertexFormat::VAT_FLOAT3, 0
		, VertexFormat::VAU_TEXCOORD, VertexFormat::VAT_FLOAT2, 0
		, VertexFormat::VAU_COLOR, VertexFormat::VAT_UBYTE4, 0
		);
	vf->Update();

	unsigned int totalNumVertices = 0, totalNumIndices = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		totalNumIndices		+= scene->mMeshes[i]->mNumFaces * 3;
		totalNumVertices	+= scene->mMeshes[i]->mNumVertices;
	}

	//create our IB
	ib = new IndexBufferDX9(totalNumIndices, IndexBuffer::IBF_INDEX32);
	//create our VB
	vb = new VertexBufferDX9(vf, totalNumVertices, ib);

	ib->Lock(Buffer::BL_WRITE_ONLY);
	vb->Lock(Buffer::BL_WRITE_ONLY);

	unsigned int iterIndices = 0, iterVertices = 0, indexOffset = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		// Populate our IB
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				assert(scene->mMeshes[i]->mFaces[j].mNumIndices == 3);
				ib->SetIndex(iterIndices++, scene->mMeshes[i]->mFaces[j].mIndices[k] + indexOffset);
			}
		}
		indexOffset += scene->mMeshes[i]->mNumVertices;

		// Populate our VB
		for (unsigned int j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
		{
			vb->Position<Vec3f>(iterVertices) = Vec3f(
				scene->mMeshes[i]->mVertices[j].x,
				scene->mMeshes[i]->mVertices[j].y,
				scene->mMeshes[i]->mVertices[j].z);
			vb->Normal<Vec3f>(iterVertices) = Vec3f(
				scene->mMeshes[i]->mNormals[j].x,
				scene->mMeshes[i]->mNormals[j].y,
				scene->mMeshes[i]->mNormals[j].z);
			vb->Color<D3DCOLOR>(iterVertices, 0) = D3DCOLOR_XRGB(255, 255, 255);
			vb->TexCoord<Vec2f>(iterVertices++, 0) = Vec2f(
				scene->mMeshes[i]->mTextureCoords[0][j].x,
				scene->mMeshes[i]->mTextureCoords[0][j].y);
		}
	}
	assert(iterIndices == totalNumIndices && iterVertices == totalNumVertices);

	ib->Update();
	ib->Unlock();
	vb->Update();
	vb->Unlock();
}

void RendererDX9::ReleaseResources()
{
	delete vb;
	delete vf;
	delete ib;
	delete tex;
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
	pp.BackBufferWidth	= backBufferSize[0];
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
	//((RenderData&)renderParams).backBufferSize[0] = ((RenderData&)renderParams).backBufferSize[1] = 512;
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
						D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	
	// Begin the scene
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		// Set up a material. The material here just has the diffuse and ambient 	
		// colors set to yellow. Note that only one material can be used at a time. 	
		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.r = 1.0f;
		mtrl.Diffuse.g = 1.0f;
		mtrl.Diffuse.b = 1.0f;
		mtrl.Ambient.r = 0.3f;
		mtrl.Ambient.g = 0.3f;
		mtrl.Ambient.b = 0.3f;
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
		light.Ambient.r = 0.5f;
		light.Ambient.g = 0.5f;
		light.Ambient.b = 0.5f;
		vecDir = D3DXVECTOR3(/*cosf(GetTickCount() / 350.0f)*/ 1.f, -1.0f, 0.f /*sinf(GetTickCount() / 350.0f)*/);
		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
		light.Range = 5000.0f;
		m_pd3dDevice->SetLight(0, &light);
		m_pd3dDevice->LightEnable(0, TRUE);
		
		// Set up world matrix
		D3DXMATRIXA16 matWorldRot, matWorldPos, matWorld;
		D3DXMatrixIdentity(&matWorldRot);
		D3DXMatrixIdentity(&matWorldPos);
		D3DXMatrixRotationY(&matWorldRot, (float)GetTickCount() / 1000.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the
		// origin, and define "up" to be in the y-direction.
		D3DXVECTOR3 vEyePt(200.f * cosf(GetTickCount() / 1000.f), 100.f, 200.f* sinf(GetTickCount() / 1000.f));
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
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, aspectRatio, 1.0f, 5000.0f);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
		
		if (tex->GetTextureType() == Texture::TT_CUBE)
		{
			D3DXMATRIXA16 matInvView;
			D3DXMatrixInverse(&matInvView, NULL, &matView);
			matInvView._41 = 0;
			matInvView._42 = 0;
			matInvView._43 = 0;
			matInvView._44 = 0;
			m_pd3dDevice->SetTransform(D3DTS_TEXTURE0, &matInvView);
		}
		
		tex->Enable(0);

		//D3DXMatrixTranslation(&matWorldPos, -5.f, -5.f, 0.f);
		//D3DXMatrixMultiply(&matWorld, &matWorldRot, &matWorldPos);
		//m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//
		//LPD3DXMESH pMesh;
		//D3DXCreateSphere(m_pd3dDevice, 5, 32, 32, &pMesh, NULL);
		//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		//pMesh->DrawSubset(0);
		//pMesh->Release();
		//
		//D3DXMatrixTranslation(&matWorldPos, 5.f, 5.f, 0.f);
		//D3DXMatrixMultiply(&matWorld, &matWorldRot, &matWorldPos);
		//m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
		//
		//D3DXCreateTorus(m_pd3dDevice, 2, 5, 32, 32, &pMesh, NULL);
		//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//pMesh->DrawSubset(0);
		//pMesh->Release();
		
		D3DXMatrixTranslation(&matWorldPos, 0.f, 0.f, 0.f);
		D3DXMatrixRotationX(&matWorldPos, -Math::PI_OVER_2);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorldPos);
		vb->Enable();
		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vb->GetElementCount(), 0, ib->GetElementCount() / 3);
		vb->Disable();

		tex->Disable(0);

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
