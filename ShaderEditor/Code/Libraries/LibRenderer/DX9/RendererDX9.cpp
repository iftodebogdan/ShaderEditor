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
#include "RenderStateDX9.h"
#include "SamplerStateDX9.h"
using namespace LibRendererDll;

#include <d3dx9.h>

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

void RendererDX9::Initialize(void* hWnd)
{
	// Create the D3D object, which is needed to create the D3DDevice.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	assert(m_pD3D);

	// Verify supported adapter modes
	int nMode = 0;
	D3DDISPLAYMODE d3ddm;
	int nMaxAdaptorModes = m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8);
	for (nMode = 0; nMode < nMaxAdaptorModes; ++nMode)
	{
		if (FAILED(m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT, D3DFMT_X8R8G8B8, nMode, &d3ddm)))
		{
			// TO DO: Respond to failure of EnumAdapterModes
			assert(false);
			return;
		}

		DeviceCaps::ScreenFormat sf;
		sf.nWidth = d3ddm.Width;
		sf.nHeight = d3ddm.Height;
		sf.nRefreshRate = d3ddm.RefreshRate;
		m_tDeviceCaps.arrSupportedScreenFormats.push_back(sf);
	}

	// Can we get a 32-bit back buffer?
	if (FAILED(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DFMT_X8R8G8B8,
		D3DFMT_X8R8G8B8,
		FALSE)))
	{
		// TODO: Handle lack of support for a 32-bit back buffer...
		assert(false);
		return;
	}

	// Can we get a z-buffer that's at least 16 bits?
	if (FAILED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DFMT_X8R8G8B8,
		D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE,
		D3DFMT_D16)))
	{
		// TODO: Handle lack of support for a 16-bit z-buffer...
		assert(false);
		return;
	}

	// Query the device for its capabilities.
	D3DCAPS9 deviceCaps;
	HRESULT hr = m_pD3D->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&deviceCaps);
	assert(SUCCEEDED(hr));

	m_tDeviceCaps.bCanAutoGenMipmaps = (deviceCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP) != 0;
	m_tDeviceCaps.bDynamicTextures = (deviceCaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES) != 0;
	m_tDeviceCaps.bPresentIntervalImmediate = (deviceCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE) != 0;
	m_tDeviceCaps.bPresentIntervalOne = (deviceCaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE) != 0;
	m_tDeviceCaps.bPresentIntervalTwo = (deviceCaps.PresentationIntervals & D3DPRESENT_INTERVAL_TWO) != 0;
	m_tDeviceCaps.bPresentIntervalThree = (deviceCaps.PresentationIntervals & D3DPRESENT_INTERVAL_THREE) != 0;
	m_tDeviceCaps.bPresentIntervalFour = (deviceCaps.PresentationIntervals & D3DPRESENT_INTERVAL_FOUR) != 0;
	m_tDeviceCaps.bMrtIndependentBitDepths = (deviceCaps.PrimitiveMiscCaps & D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS) != 0;
	m_tDeviceCaps.bMrtPostPixelShaderBlending = (deviceCaps.PrimitiveMiscCaps & D3DPMISCCAPS_MRTPOSTPIXELSHADERBLENDING) != 0;
	m_tDeviceCaps.bAnisotropicFiltering = (deviceCaps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY) != 0;
	m_tDeviceCaps.bDepthBias = (deviceCaps.RasterCaps & D3DPRASTERCAPS_DEPTHBIAS) != 0;
	m_tDeviceCaps.bSlopeScaledDepthBias = (deviceCaps.RasterCaps & D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS) != 0;
	m_tDeviceCaps.bMipmapLodBias = (deviceCaps.RasterCaps & D3DPRASTERCAPS_MIPMAPLODBIAS) != 0;
	m_tDeviceCaps.bWBuffer = (deviceCaps.RasterCaps & D3DPRASTERCAPS_WBUFFER) != 0;
	m_tDeviceCaps.bTexturePow2 = (deviceCaps.TextureCaps & D3DPTEXTURECAPS_POW2) != 0;
	m_tDeviceCaps.nMaxTextureWidth = deviceCaps.MaxTextureWidth;
	m_tDeviceCaps.nMaxTextureHeight = deviceCaps.MaxTextureHeight;
	m_tDeviceCaps.nMaxTextureDepth = deviceCaps.MaxVolumeExtent;
	m_tDeviceCaps.nVertexShaderVersionMajor = D3DSHADER_VERSION_MAJOR(deviceCaps.VertexShaderVersion);
	m_tDeviceCaps.nVertexShaderVersionMinor = D3DSHADER_VERSION_MINOR(deviceCaps.VertexShaderVersion);
	m_tDeviceCaps.nPixelShaderVersionMajor = D3DSHADER_VERSION_MAJOR(deviceCaps.PixelShaderVersion);
	m_tDeviceCaps.nPixelShaderVersionMinor = D3DSHADER_VERSION_MINOR(deviceCaps.PixelShaderVersion);
	m_tDeviceCaps.nNumSimultaneousRTs = deviceCaps.NumSimultaneousRTs;

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
	d3dpp.BackBufferWidth = m_vViewportSize[0];
	d3dpp.BackBufferHeight = m_vViewportSize[1];
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

	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = m_vViewportSize[0];
	vp.Height = m_vViewportSize[1];
	vp.MinZ = 0.f;
	vp.MaxZ = 1.f;

	hr = m_pd3dDevice->SetViewport(&vp);
	assert(SUCCEEDED(hr));

	m_pRenderState = new RenderStateDX9();
	m_pSamplerState = new SamplerStateDX9();

	CreateResources();
}

#include <Importer.hpp> // C++ importer interface
#include <scene.h> // Output data structure
#include <postprocess.h> // Post processing flags

// Create an instance of the Importer class
Assimp::Importer importer;
static const aiScene* scene = nullptr;

#include "RenderTargetDX9.h"
#include "ShaderTemplate.h"
#include "ShaderProgramDX9.h"
#include "ShaderInput.h"
#include <fstream>
static VertexFormatDX9* vf = nullptr;
static VertexBufferDX9* vb = nullptr;
static IndexBufferDX9* ib = nullptr;
static TextureDX9* tex = nullptr;
static TextureDX9* tex2 = nullptr;
static ShaderProgramDX9* VProg = nullptr;
static ShaderProgramDX9* PProg = nullptr;
static ShaderTemplate* VTemp = nullptr;
static ShaderTemplate* PTemp = nullptr;
static RenderTargetDX9* rt = nullptr;

#include "Utility/TextureLoader.h"

void RendererDX9::CreateResources()
{
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// propably to request more postprocessing than we do in this example.
	if (scene == nullptr)
		scene = importer.ReadFile(
			"models/COLLADA/teapot_instancenodes.DAE",
			//"models/COLLADA/duck.dae",
			//"sponza_scene/sponza.obj",
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords);

	//HRESULT hr;
	// Turn off culling, so we see the front and back of the triangle
	//hr = m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Turn on the zbuffer
	//hr = m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//assert(SUCCEEDED(hr));
	GetRenderStateManager()->SetZEnable(ZB_ENABLED);

	TextureLoader::ImageDesc desc = TextureLoader::LoadImageFile("sky-cubemap.dds", true);
	tex = new TextureDX9(desc.format, desc.type, desc.width, desc.height, desc.depth);
	TextureLoader::CopyImageData(tex);
	
	desc = TextureLoader::LoadImageFile("sponza_ceiling_a_diff.tga", true);
	tex2 = new TextureDX9(desc.format, desc.type, desc.width, desc.height, desc.depth, 1, BU_DYNAMIC);
	TextureLoader::CopyImageData(tex2);

	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GetSamplerStateManager()->SetFilter(0, SF_MIN_MAG_LINEAR_MIP_LINEAR);

	vf = new VertexFormatDX9(3);
	vf->Initialize(
		VAU_POSITION, VAT_FLOAT3, 0
		, VAU_NORMAL, VAT_FLOAT3, 0
		, VAU_TEXCOORD, VAT_FLOAT2, 0
		//, VAU_COLOR, VAT_UBYTE4, 0
		);
	vf->Update();

	unsigned int totalNumVertices = 0, totalNumIndices = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		totalNumIndices		+= scene->mMeshes[i]->mNumFaces * 3;
		totalNumVertices	+= scene->mMeshes[i]->mNumVertices;
	}

	//create our IB
	ib = new IndexBufferDX9(totalNumIndices, IBF_INDEX32);
	//create our VB
	vb = new VertexBufferDX9(vf, totalNumVertices, ib);

	ib->Lock(BL_WRITE_ONLY);
	vb->Lock(BL_WRITE_ONLY);

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
			//vb->Color<D3DCOLOR>(iterVertices, 0) = D3DCOLOR_XRGB(255, 255, 255);
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

	std::ifstream t("test.hlsl");
	int length;
	t.seekg(0, std::ios::end);			// go to the end
	length = (int)t.tellg();			// report location (this is the length)
	t.seekg(0, std::ios::beg);			// go back to the beginning
	char* buffer = new char[length];    // allocate memory for a buffer of appropriate dimension
	t.read(buffer, length);				// read the whole file into the buffer
	t.close();							// close file handle
	buffer[length - 1] = '\0';

	VProg = new ShaderProgramDX9(SPT_VERTEX, buffer);
	PProg = new ShaderProgramDX9(SPT_PIXEL, buffer);

	VTemp = new ShaderTemplate(VProg);
	PTemp = new ShaderTemplate(PProg);

	delete[] buffer;

	rt = new RenderTargetDX9(1, tex2->GetTextureFormat(), tex2->GetWidth(), tex2->GetHeight(), false, true);
}

void RendererDX9::ReleaseResources()
{
	delete vb;
	delete vf;
	delete ib;
	delete tex;
	delete tex2;
	delete VProg;
	delete PProg;
	delete VTemp;
	delete PTemp;
	delete rt;
}

void RendererDX9::SetViewport(const Vec2i size, const Vec2i offset)
{
	if (size == m_vViewportSize && offset == m_vViewportOffset)
		return;

	Vec2i oldVPSize = m_vViewportSize;
	Renderer::SetViewport(size, offset);

	if (!m_pd3dDevice)
		return;

	HRESULT hr;
	if (size != oldVPSize)
	{
		// Get swap chain
		LPDIRECT3DSWAPCHAIN9 sc;
		hr = m_pd3dDevice->GetSwapChain(0, &sc);
		assert(SUCCEEDED(hr));

		// Get present parameters
		D3DPRESENT_PARAMETERS pp;
		hr = sc->GetPresentParameters(&pp);
		assert(SUCCEEDED(hr));

		ULONG refCount = 0;
		refCount = sc->Release();
		assert(refCount == 0);

		// Set back buffer size
		pp.BackBufferWidth = m_vViewportSize[0];
		pp.BackBufferHeight = m_vViewportSize[1];

		// Destroy resources
		ReleaseResources();

		// Reset the device
		hr = m_pd3dDevice->Reset(&pp);
		assert(SUCCEEDED(hr));

		CreateResources();
	}

	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = size[0];
	vp.Height = size[1];
	vp.MinZ = 0.f;
	vp.MaxZ = 1.f;

	hr = m_pd3dDevice->SetViewport(&vp);
	assert(SUCCEEDED(hr));
}

void RendererDX9::RenderScene()
{
	assert(m_pd3dDevice);

	HRESULT hr;

	// Clear the backbuffer and the zbuffer
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0);
	
	// Begin the scene
	if (SUCCEEDED(m_pd3dDevice->BeginScene()))
	{
		// Set up world matrix
		D3DXMATRIXA16 matWorldRot, matWorldPos, matWorld;
		D3DXMatrixRotationX(&matWorldPos, -Math::PI_OVER_2);
		D3DXMatrixRotationY(&matWorldRot, (float)GetTickCount() / 5000.0f);

		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the
		// origin, and define "up" to be in the y-direction.
		D3DXVECTOR3 vEyePt(200.f * cosf(GetTickCount() / 1000.f), 100.f, 200.f* sinf(GetTickCount() / 1000.f));
		//D3DXVECTOR3 vEyePt(0.f, 10.f, 0.f);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		//D3DXVECTOR3 vLookatPt(100.0f, 30.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMATRIXA16 matProj;
		float aspectRatio = (float)m_vViewportSize[0] / (float)m_vViewportSize[1];
		D3DXMatrixPerspectiveFovLH(&matProj, 55 * D3DX_PI / 180, aspectRatio, 20.0f, 2000.0f);
		
		ShaderInput VInput(VTemp);
		ShaderInput PInput(PTemp);
		Matrix44f matWVP[3];
		for (unsigned int i = 0; i < 3; i++)
		{
			D3DXMATRIX mat;
			switch (i)
			{
			case 0:
				mat = matWorldPos;
				//mat = matWorldRot;
				break;
			case 1:
				mat = matView;
				break;
			case 2:
				mat = matProj;
			}
			matWVP[i](0, 0) = mat._11; matWVP[i](0, 1) = mat._12; matWVP[i](0, 2) = mat._13; matWVP[i](0, 3) = mat._14;
			matWVP[i](1, 0) = mat._21; matWVP[i](1, 1) = mat._22; matWVP[i](1, 2) = mat._23; matWVP[i](1, 3) = mat._24;
			matWVP[i](2, 0) = mat._31; matWVP[i](2, 1) = mat._32; matWVP[i](2, 2) = mat._33; matWVP[i](2, 3) = mat._34;
			matWVP[i](3, 0) = mat._41; matWVP[i](3, 1) = mat._42; matWVP[i](3, 2) = mat._43; matWVP[i](3, 3) = mat._44;
		}
		unsigned int handle;
		VInput.GetInputHandleByName("matWVP", handle);
		VInput.SetMatrixArray<4, 4>(handle, matWVP);

		assert(VInput.GetMatrix4x4(handle, 0) == matWVP[0]);
		assert(VInput.GetMatrix4x4(handle, 1) == matWVP[1]);
		assert(VInput.GetMatrix4x4(handle, 2) == matWVP[2]);

		Matrix33f matTest33;
		//matTest33[1][0] = 2.f;
		VInput.GetInputHandleByName("matTest33", handle);
		VInput.SetMatrix3x3(handle, matTest33);
		assert(VInput.GetMatrix3x3(handle) == matTest33);

		Matrix22f matTest22;
		//matTest22[0][1] = 1.f;
		VInput.GetInputHandleByName("matTest22", handle);
		VInput.SetMatrix<2, 2>(handle, matTest22);
		assert((VInput.GetMatrix<2, 2>(handle)) == matTest22);

		//Matrix32f matTest32;
		////matTest32[2][0] = -1.f;
		//VInput.GetInputHandleByName("matTest32", handle);
		//VInput.SetMatrix<3, 2>(handle, matTest32);
		//assert((VInput.GetMatrix<3, 2>(handle)) == matTest32);

		bool bData[3] = { true, true, true };
		bool b1 = true;
		Vec<bool, 2> b2 = Vec<bool, 2>(true, true);
		Vec<bool, 3> b3 = Vec<bool, 3>(true, true, true);
		Vec<bool, 4> b4 = Vec<bool, 4>(true, true, true, true);
		VInput.GetInputHandleByName("bArr", handle);
		VInput.SetBoolArray(handle, bData);
		assert(VInput.GetBool(handle, 0) == bData[0]);
		assert(VInput.GetBool(handle, 1) == bData[1]);
		assert(VInput.GetBool(handle, 2) == bData[2]);
		VInput.GetInputHandleByName("b1", handle);
		VInput.SetBool(handle, b1);
		assert(VInput.GetBool(handle) == b1);
		VInput.GetInputHandleByName("b2", handle);
		VInput.SetBool2(handle, b2);
		assert(VInput.GetBool2(handle) == b2);
		VInput.GetInputHandleByName("b3", handle);
		VInput.SetBool3(handle, b3);
		assert(VInput.GetBool3(handle) == b3);
		VInput.GetInputHandleByName("b4", handle);
		VInput.SetBool4(handle, b4);
		assert(VInput.GetBool4(handle) == b4);
		
		int iData[3] = { 1, 2, 3 };
		int i1 = 4;
		Vec<int, 2> i2 = Vec<int, 2>(5, 6);
		Vec<int, 3> i3 = Vec<int, 3>(7, 8, 9);
		Vec<int, 4> i4 = Vec<int, 4>(10, 11, 12, 13);
		VInput.GetInputHandleByName("iArr", handle);
		VInput.SetIntArray(handle, iData);
		assert(VInput.GetInt(handle, 0) == iData[0]);
		assert(VInput.GetInt(handle, 1) == iData[1]);
		assert(VInput.GetInt(handle, 2) == iData[2]);
		VInput.GetInputHandleByName("i1", handle);
		VInput.SetInt(handle, i1);
		assert(VInput.GetInt(handle) == i1);
		VInput.GetInputHandleByName("i2", handle);
		VInput.SetInt2(handle, i2);
		assert(VInput.GetInt2(handle) == i2);
		VInput.GetInputHandleByName("i3", handle);
		VInput.SetInt3(handle, i3);
		assert(VInput.GetInt3(handle) == i3);
		VInput.GetInputHandleByName("i4", handle);
		VInput.SetInt4(handle, i4);
		assert(VInput.GetInt4(handle) == i4);
		
		float fData[9] = { 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f, 21.f, 22.f };
		float f1 = 23.f;
		Vec<float, 2> f2 = Vec<float, 2>(24.f, 25.f);
		Vec<float, 3> f3 = Vec<float, 3>(26.f, 27.f, 28.f);
		Vec<float, 4> f4 = Vec<float, 4>(29.f, 30.f, 31.f, 32.f);
		VInput.GetInputHandleByName("fArr", handle);
		VInput.SetFloatArray(handle, fData);
		assert(VInput.GetFloat3(handle, 0)[0] == fData[0]);
		assert(VInput.GetFloat3(handle, 0)[1] == fData[1]);
		assert(VInput.GetFloat3(handle, 0)[2] == fData[2]);
		assert(VInput.GetFloat3(handle, 1)[0] == fData[3]);
		assert(VInput.GetFloat3(handle, 1)[1] == fData[4]);
		assert(VInput.GetFloat3(handle, 1)[2] == fData[5]);
		assert(VInput.GetFloat3(handle, 2)[0] == fData[6]);
		assert(VInput.GetFloat3(handle, 2)[1] == fData[7]);
		assert(VInput.GetFloat3(handle, 2)[2] == fData[8]);
		VInput.GetInputHandleByName("f1", handle);
		VInput.SetFloat(handle, f1);
		assert(VInput.GetFloat(handle) == f1);
		VInput.GetInputHandleByName("f2", handle);
		VInput.SetFloat2(handle, f2);
		assert(VInput.GetFloat2(handle) == f2);
		VInput.GetInputHandleByName("f3", handle);
		VInput.SetFloat3(handle, f3);
		assert(VInput.GetFloat3(handle) == f3);
		VInput.GetInputHandleByName("f4", handle);
		VInput.SetFloat4(handle, f4);
		assert(VInput.GetFloat4(handle) == f4);

		PInput.GetInputHandleByName("s2D", handle);
		PInput.SetTexture(handle, tex2);
		PInput.GetInputHandleByName("sCube", handle);
		PInput.SetTexture(handle, tex);

		vb->Enable();
		VTemp->Enable(VInput);
		PTemp->Enable(PInput);

		rt->Enable();
		m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vb->GetElementCount(), 0, ib->GetElementCount() / 3);
		rt->Disable();

		Texture* tex3 = tex2;
		rt->CopyColorBuffer(0, tex3);
		tex2->Lock(0, BL_WRITE_ONLY);
		tex2->Update();
		tex2->Unlock();
		tex2 = (TextureDX9*)tex3;
		//tex2->GenerateMipmaps();

		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, vb->GetElementCount(), 0, ib->GetElementCount() / 3);
		
		PTemp->Disable();
		VTemp->Disable();
		vb->Disable();

		// End the scene
		hr = m_pd3dDevice->EndScene();
		assert(SUCCEEDED(hr));
	}

	// Present the backbuffer contents to the display
	RECT dstRect;
	dstRect.left = m_vViewportOffset[0];
	dstRect.top = m_vViewportOffset[1];
	dstRect.right = m_vViewportSize[0] + m_vViewportOffset[0];
	dstRect.bottom = m_vViewportSize[1] + m_vViewportOffset[1];
	
	hr = m_pd3dDevice->Present(NULL, &dstRect, NULL, NULL);
	assert(SUCCEEDED(hr));
}
