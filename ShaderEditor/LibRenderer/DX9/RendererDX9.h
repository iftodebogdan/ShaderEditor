#ifndef RENDERERDX9_H
#define RENDERERDX9_H

#include "../Renderer.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace LibRendererDll
{
	class VertexFormatDX9;
	class VertexBufferDX9;

	class RendererDX9 : public Renderer
	{
		// Used to create the D3DDevice
		IDirect3D9*			m_pD3D;
		// Our rendering device
		IDirect3DDevice9*	m_pd3dDevice;

		VertexFormatDX9*	vf;
		VertexBufferDX9*	vb;

		void						CreateResources();
		void						ReleaseResources();

		void						GetBackBufferSize(Vec2i& backBufferSize);
		void						SetBackBufferSize(const Vec2i& backBufferSize);
		void						GetViewport(RenderData::Viewport& viewport);
		void						SetViewport(const RenderData::Viewport& viewport);

	public:
		RendererDX9();
		~RendererDX9();

		static	RendererDX9*		GetInstance() { return (RendererDX9*)m_pInstance; };

		void						Initialize(void* hWnd, int backBufferWidth = 0, int backBufferHeight = 0);
		void						SetRenderData(const RenderData& renderData);
		void						RenderScene();

		IDirect3DDevice9*			GetDevice() const { return m_pd3dDevice; };
	};
}

#endif	//RENDERDX9_H