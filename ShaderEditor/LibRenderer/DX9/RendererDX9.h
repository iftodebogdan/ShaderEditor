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

		const	bool		GetBackBufferSize(int& sizeX, int& sizeY);
		const	bool		SetBackBufferSize(int sizeX, int sizeY);

	public:
							RendererDX9();
							~RendererDX9();

		const	bool		Initialize(void* hWnd);
				void		Cleanup() const;
		const	bool		SetRenderParameters(const tRenderParameters& renderParams);
				void		RenderScene() const;
};

#endif	//RENDERDX9_H