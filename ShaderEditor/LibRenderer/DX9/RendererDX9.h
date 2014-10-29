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
		// Our texture
		LPDIRECT3DTEXTURE9      m_pTexture;

		const	bool		CreateResources();
		const	bool		ReleaseResources();

		const	bool		GetBackBufferSize(Vec2i& backBufferSize);
		const	bool		SetBackBufferSize(const Vec2i& backBufferSize);
		const	bool		GetViewport(RenderData::Viewport& viewport);
		const	bool		SetViewport(const RenderData::Viewport& viewport);

	public:
							RendererDX9();
							~RendererDX9();

		const	bool		Initialize(void* hWnd, int backBufferWidth = 0, int backBufferHeight = 0);
		const	bool		SetRenderData(const RenderData& renderData);
		const	bool		RenderScene();
};

#endif	//RENDERDX9_H