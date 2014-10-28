#ifndef RENDERERDX9_H
#define RENDERERDX9_H

#include "../Renderer.h"
using namespace LibRendererDll;

#include <d3d9.h>

class RendererDX9 : public Renderer
	{
		// Used to create the D3DDevice
		LPDIRECT3D9         m_pD3D;
		// Our rendering device
		LPDIRECT3DDEVICE9   m_pd3dDevice;
		// Buffer to hold vertices
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;

		const	bool		CreateResources();
		const	bool		ReleaseResources();

		const	bool		GetBackBufferSize(Vec2i& backBufferSize);
		const	bool		SetBackBufferSize(const Vec2i& backBufferSize);
		const	bool		GetViewport(tRenderParameters::tViewport& viewport);
		const	bool		SetViewport(const tRenderParameters::tViewport& viewport);

	public:
							RendererDX9();
							~RendererDX9();

		const	bool		Initialize(void* hWnd, int backBufferWidth = 0, int backBufferHeight = 0);
		const	bool		SetRenderParameters(const tRenderParameters& renderParams);
		const	bool		RenderScene();
};

#endif	//RENDERDX9_H