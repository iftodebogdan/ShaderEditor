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
		// Destination rectangle
		RECT				m_pDstRect;

	public:
							RendererDX9();
		virtual				~RendererDX9();

		virtual	bool		Initialize(void* hWnd);
		virtual	void		SetBackBufferSize(int sizeX, int sizeY);
		virtual	void		SetDestRect(int left, int right, int top, int bottom);
		virtual	void		RenderScene();
};

#endif	//RENDERDX9_H