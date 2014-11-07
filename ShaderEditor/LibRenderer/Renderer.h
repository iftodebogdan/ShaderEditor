#ifndef RENDERER_H
#define RENDERER_H

#ifdef LIBRENDERER_EXPORTS
#define LIBRENDERER_DLL __declspec(dllexport) 
#else
#define LIBRENDERER_DLL __declspec(dllimport) 
#endif

#include "RenderData.h"

namespace LibRendererDll
{
	class Renderer
	{
	protected:
		virtual		~Renderer();

	public:
		enum API
		{
			API_DX9,
		};

		/* Create an instance of the Renderer object based on API of choice */
		static	LIBRENDERER_DLL			void					CreateInstance(API eApi);
		/* Destroys the instance of the Renderer object */
		static	LIBRENDERER_DLL			void					DestroyInstance();
		/* Retrieves the instance of the Renderer object */
		static	LIBRENDERER_DLL			Renderer*				GetInstance();

		/* After you create an application window, you are ready to initialize the graphics
		object that you will use to render the scene. This process includes creating the
		object, setting the presentation parameters, and finally creating the device. */
		virtual	LIBRENDERER_DLL			void					Initialize(void* hWnd, int backBufferWidth = 0, int backBufferHeight = 0) = 0;

		/* Get presentation parameters, global states, etc. */
		virtual	LIBRENDERER_DLL	const	RenderData&				GetRenderData() const;
		/* Set presentation parameters, global states, etc. */
		virtual	LIBRENDERER_DLL			void					SetRenderData(const RenderData& renderData) = 0;
		
		virtual	LIBRENDERER_DLL			void					RenderScene() = 0;

	protected:
		static	Renderer*			m_pInstance;
				RenderData			m_RenderData;
	};
}

#endif	//RENDERER_H
