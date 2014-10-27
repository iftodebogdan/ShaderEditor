#ifndef RENDERER_H
#define RENDERER_H

#ifdef LIBRENDERER_EXPORTS
#define LIBRENDERER_DLL __declspec(dllexport) 
#else
#define LIBRENDERER_DLL __declspec(dllimport) 
#endif

namespace LibRendererDll
{
	class Renderer
	{
	protected:
		static					Renderer*		m_pInstance;

	public:
		enum API
		{
			API_DX9,
		};

		/*Create an instance of the Renderer object based on API of choice*/
		static	LIBRENDERER_DLL	bool			CreateInstance(API eApi);
		static	LIBRENDERER_DLL	Renderer*		GetInstance();

		/*After you create an application window, you are ready to initialize the graphics
		object that you will use to render the scene. This process includes creating the
		object, setting the presentation parameters, and finally creating the device.*/
		virtual	LIBRENDERER_DLL	bool			Initialize(void* hWnd) = 0;

		virtual	LIBRENDERER_DLL	void			SetBackBufferSize(int sizeX, int sizeY) = 0;
		virtual	LIBRENDERER_DLL	void			SetDestRect(int left, int right, int top, int bottom) = 0;

		virtual	LIBRENDERER_DLL	void			RenderScene() = 0;
	};
}

#endif	//RENDERER_H
