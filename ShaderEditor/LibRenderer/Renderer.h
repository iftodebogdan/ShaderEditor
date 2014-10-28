#ifndef RENDERER_H
#define RENDERER_H

#ifdef LIBRENDERER_EXPORTS
#define LIBRENDERER_DLL __declspec(dllexport) 
#else
#define LIBRENDERER_DLL __declspec(dllimport) 
#endif

#include <gmtl/gmtl.h>
using namespace gmtl;

namespace LibRendererDll
{
	class Renderer
	{
	public:
		enum API
		{
			API_DX9,
		};

		struct tRenderParameters
		{
			Vec2i		backBufferSize;
			
			struct { Vec2i topLeft; Vec2i bottomRight; }
						dstRect;
		};

		/* Create an instance of the Renderer object based on API of choice */
		static	LIBRENDERER_DLL	const	bool					CreateInstance(API eApi);
		/* Destroys the instance of the Renderer object */
		static	LIBRENDERER_DLL			void					DestroyInstance();
		/* Retrieves the instance of the Renderer object */
		static	LIBRENDERER_DLL			Renderer*				GetInstance();

		/* After you create an application window, you are ready to initialize the graphics
		object that you will use to render the scene. This process includes creating the
		object, setting the presentation parameters, and finally creating the device. */
		virtual	LIBRENDERER_DLL	const	bool					Initialize(void* hWnd) = 0;
		virtual	LIBRENDERER_DLL			void					Cleanup() const = 0;

		virtual	LIBRENDERER_DLL	const	tRenderParameters&		GetRenderParameters() const;
		virtual	LIBRENDERER_DLL	const	bool					SetRenderParameters(const tRenderParameters& renderParams) = 0;

		virtual	LIBRENDERER_DLL			void					RenderScene() const = 0;

protected:
		static	Renderer*		m_pInstance;
		tRenderParameters		m_renderParams;

		virtual	const	bool	GetBackBufferSize(int& sizeX, int& sizeY) = 0;
		virtual	const	bool	SetBackBufferSize(int sizeX, int sizeY) = 0;
	};
}

#endif	//RENDERER_H
